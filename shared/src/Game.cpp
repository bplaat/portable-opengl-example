#include "Game.h"
#include "bindings.h"
#include "math/Random.h"
#include "math/utils.h"
#include "objects/Box.h"

#ifdef PLATFORM_WEB
const char *vertexShaderSource = "#version 300 es\n"
#endif
#ifdef PLATFORM_DESKTOP
    const char *vertexShaderSource = "#version 330 core\n"
#endif
                                     "in vec4 a_position;\n"
                                     "in vec2 a_texture_position;\n"
                                     "uniform mat4 u_matrix;\n"
                                     "uniform mat4 u_camera;\n"
                                     "out vec2 v_texture_position;\n"
                                     "void main() {\n"
                                     "    gl_Position = u_camera * u_matrix * a_position;\n"
                                     "    v_texture_position = a_texture_position;\n"
                                     "}";

#ifdef PLATFORM_WEB
const char *fragmentShaderSource = "#version 300 es\n"
#endif
#ifdef PLATFORM_DESKTOP
    const char *fragmentShaderSource = "#version 330 core\n"
#endif
                                       "precision highp float;\n"
                                       "uniform sampler2D u_texture;\n"
                                       "in vec2 v_texture_position;\n"
                                       "out vec4 o_color;\n"
                                       "void main() {\n"
                                       "    o_color = texture(u_texture, v_texture_position);\n"
                                       "}";

float planeVertices[] = {
    // Vertex position, Texture position
    -0.5, -0.5, 0, 0, 0, // First triangle
    0.5,  -0.5, 0, 1, 0, // First triangle
    0.5,  0.5,  0, 1, 1, // First triangle

    -0.5, -0.5, 0, 0, 0, // Second triangle
    0.5,  0.5,  0, 1, 1, // Second triangle
    -0.5, 0.5,  0, 0, 1  // Second triangle
};

float cubeVertices[] = {
    // Vertex position, Texture position
    -0.5, 0.5,  0.5,  1, 1, // Top face
    0.5,  0.5,  0.5,  0, 1, // Top face
    0.5,  0.5,  -0.5, 0, 0, // Top face
    -0.5, 0.5,  0.5,  1, 1, // Top face
    -0.5, 0.5,  -0.5, 1, 0, // Top face
    0.5,  0.5,  -0.5, 0, 0, // Top face

    -0.5, -0.5, -0.5, 1, 0, // Front face
    0.5,  -0.5, -0.5, 0, 0, // Front face
    0.5,  0.5,  -0.5, 0, 1, // Front face
    -0.5, -0.5, -0.5, 1, 0, // Front face
    -0.5, 0.5,  -0.5, 1, 1, // Front face
    0.5,  0.5,  -0.5, 0, 1, // Front face

    -0.5, -0.5, 0.5,  0, 0, // Back face
    0.5,  -0.5, 0.5,  1, 0, // Back face
    0.5,  0.5,  0.5,  1, 1, // Back face
    -0.5, -0.5, 0.5,  0, 0, // Back face
    -0.5, 0.5,  0.5,  0, 1, // Back face
    0.5,  0.5,  0.5,  1, 1, // Back face

    -0.5, -0.5, -0.5, 0, 0, // Left face
    -0.5, -0.5, 0.5,  1, 0, // Left face
    -0.5, 0.5,  0.5,  1, 1, // Left face
    -0.5, -0.5, -0.5, 0, 0, // Left face
    -0.5, 0.5,  -0.5, 0, 1, // Left face
    -0.5, 0.5,  0.5,  1, 1, // Left face

    0.5,  -0.5, -0.5, 1, 0, // Right face
    0.5,  -0.5, 0.5,  0, 0, // Right face
    0.5,  0.5,  0.5,  0, 1, // Right face
    0.5,  -0.5, -0.5, 1, 0, // Right face
    0.5,  0.5,  -0.5, 1, 1, // Right face
    0.5,  0.5,  0.5,  0, 1, // Right face

    -0.5, -0.5, -0.5, 0, 0, // Bottom face
    0.5,  -0.5, -0.5, 1, 0, // Bottom face
    0.5,  -0.5, 0.5,  1, 1, // Bottom face
    -0.5, -0.5, -0.5, 0, 0, // Bottom face
    -0.5, -0.5, 0.5,  0, 1, // Bottom face
    0.5,  -0.5, 0.5,  1, 1  // Bottom face
};

Game *Game::getInstance() {
    static Game game;
    return &game;
}

void Game::init() {
    random = new Random(time(NULL));

    // Print OpenGL version info
    print("Using ");
#ifdef PLATFORM_DESKTOP
    print("OpenGL ");
#endif
    print((const char *)glGetString(GL_VERSION));
    print(" on ");
    println((const char *)glGetString(GL_RENDERER));

    // Grow heap with 2 * 64 KiB pages
#ifdef PLATFORM_WEB
    __builtin_wasm_memory_grow(0, 2);
#endif

    // Create boxes
    boxesSize = 4 * 1024;
    boxes = (Box **)malloc(boxesSize * sizeof(Box *));
    for (int32_t i = 0; i < boxesSize; i++) {
        boxes[i] = new Box(random);
    }

    // Compile vertex shader
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int32_t success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        print("Vertex shader compilation error: ");
        println(infoLog);
        exit(EXIT_FAILURE);
        return;
    }

    // Compile fragment shader
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        print("Fragment shader compilation error: ");
        println(infoLog);
        exit(EXIT_FAILURE);
        return;
    }

    // Create shader program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Get uniforms
    matrixUniform = glGetUniformLocation(program, "u_matrix");
    cameraUniform = glGetUniformLocation(program, "u_camera");
    textureUniform = glGetUniformLocation(program, "u_texture");

    // Create plane vertex
    glGenVertexArrays(1, &planeVertrexArray);
    glBindVertexArray(planeVertrexArray);

    uint32_t planeBuffer;
    glGenBuffers(1, &planeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, planeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    uint32_t positionLocation = glGetAttribLocation(program, "a_position");
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(positionLocation);

    uint32_t texturePositionLocation = glGetAttribLocation(program, "a_texture_position");
    glVertexAttribPointer(texturePositionLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(texturePositionLocation);

    // Create cube vertex
    glGenVertexArrays(1, &cubeVertrexArray);
    glBindVertexArray(cubeVertrexArray);

    uint32_t cubeBuffer;
    glGenBuffers(1, &cubeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    positionLocation = glGetAttribLocation(program, "a_position");
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(positionLocation);

    texturePositionLocation = glGetAttribLocation(program, "a_texture_position");
    glVertexAttribPointer(texturePositionLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(texturePositionLocation);

    // Create camera
    camera = new PerspectiveCamera(radians(75), (float)width / (float)height, 0.1, 1000);
    camera->position.z = -3;
    camera->updateMatrix();

    rotation = 0;
}

void Game::resize(int32_t width, int32_t height, float scale) {
    this->width = width;
    this->height = height;
    this->scale = scale;

    if (camera != NULL) {
        camera->aspect = (float)width / (float)height;
        camera->updateMatrix();
    }
}

void Game::update(float delta) {
    // Update boxes
    for (int32_t i = 0; i < boxesSize; i++) {
        Box *box = boxes[i];
        box->update(delta);
    }

    // Increase rotation
    rotation += radians(90) * delta;
}

void Game::render() {
    glViewport(0, 0, width * scale, height * scale);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw boxes
    glEnable(GL_SCISSOR_TEST);
    for (int32_t i = 0; i < boxesSize; i++) {
        Box *box = boxes[i];
        box->render();
    }
    glDisable(GL_SCISSOR_TEST);

    // Draw flat rect
    glUseProgram(program);
    glBindVertexArray(planeVertrexArray);

    Matrix4 cameraMatrix;
    cameraMatrix.flat(width, height);
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, &cameraMatrix.elements[0]);

    Matrix4 itemMatrix;
    itemMatrix.rect(10, 10, 100, 100);
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &itemMatrix.elements[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw cubes
    glBindVertexArray(cubeVertrexArray);
    glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, &camera->matrix.elements[0]);

    Object3D plane;
    plane.rotation.y = rotation;
    plane.rotation.z = rotation;
    plane.updateMatrix();
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &plane.matrix.elements[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    plane.position.x = -2;
    plane.updateMatrix();
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &plane.matrix.elements[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    plane.position.x = 2;
    plane.updateMatrix();
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &plane.matrix.elements[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
