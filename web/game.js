const canvas = document.getElementById('canvas');
const gl = canvas.getContext('webgl2');

const textEncoder = new TextEncoder();
const textDecoder = new TextDecoder();
function readString(ptr) {
    const buffer = new Uint8Array(instance.exports.memory.buffer, ptr);
    return textDecoder.decode(buffer.subarray(0, buffer.indexOf(0)));
}
function readStringFromPtr(ptr) {
    return readString(new Uint32Array(instance.exports.memory.buffer, ptr, 1)[0]);
}
function writeString(ptr, string, lengthPtr = 0, bufferLength = undefined) {
    const stringBuffer = new Uint8Array(instance.exports.memory.buffer, ptr);
    const stringBytes = textEncoder.encode(string);
    const stringLength = bufferLength != undefined ? Math.min(stringBytes.length, bufferLength) : stringBytes.length;
    if (lengthPtr != 0) {
        const lengthBuffer = new Uint32Array(instance.exports.memory.buffer, lengthPtr, 1);
        lengthBuffer[0] = stringLength;
    }
    let pos = 0;
    while (pos < stringLength) {
        stringBuffer[pos] = stringBytes[pos];
        pos++;
    }
    stringBuffer[pos] = 0;
}

const wrappers = [ undefined ];
function wrap(thing) {
    const wrapper = wrappers.length;
    wrappers[wrapper] = thing;
    return wrapper;
}

const fonts = {};
function renderText(text, fontName, size, color) {
    const canvas = document.createElement('canvas');
    const ctx = canvas.getContext('2d');

    const string = readString(text);
    ctx.font = size + 'px ' + fontName;
    canvas.width = ctx.measureText(string).width;
    canvas.height = size;

    ctx.font = size + 'px ' + fontName;
    ctx.textBaseline = 'top';
    ctx.fillStyle = `rgb(${color & 255}, ${(color >> 8) & 255}, ${(color >> 16) & 255})`;
    ctx.fillText(string, 0, 0);
    return canvas;
}

let running = true, printBuffer = '';
const bindings = {
    print(string) {
        printBuffer += readString(string);
    },
    println(string) {
        console.log(printBuffer + readString(string));
        printBuffer = '';
    },
    exit(status) {
        console.log(`Exited with status ${status}`);
        running = false;
    },
    sin: Math.sin,
    cos: Math.cos,
    tan: Math.tan,
    time(timer) {
        return Math.floor(Date.now() / 1000);
    },

    glGetString(name) {
        const string = gl.getParameter(name);
        const stringPtr = instance.exports.malloc(string.length + 1);
        writeString(stringPtr, string);
        return stringPtr;
    },
    glViewport(x, y, width, height) {
        gl.viewport(x, y, width, height);
    },
    glClearColor(red, green, blue, alpha) {
        gl.clearColor(red, green, blue, alpha);
    },
    glClear(mask) {
        gl.clear(mask);
    },
    glScissor(x, y, width, height) {
        gl.scissor(x, y, width, height);
    },
    glEnable(cap) {
        gl.enable(cap);
    },
    glDisable(cap) {
        gl.disable(cap);
    },
    glBlendFunc(sfactor, dfactor) {
        gl.blendFunc(sfactor, dfactor);
    },

    glCreateShader(shaderType) {
        return wrap(gl.createShader(shaderType));
    },
    glShaderSource(shader, count, string, length) {
        gl.shaderSource(wrappers[shader], readStringFromPtr(string));
    },
    glCompileShader(shader) {
        gl.compileShader(wrappers[shader]);
    },
    glGetShaderiv(shader, pname, params) {
        const buffer = new Uint32Array(instance.exports.memory.buffer, params, 1);
        buffer[0] = gl.getShaderParameter(wrappers[shader], pname);
    },
    glGetShaderInfoLog(shader, maxLength, length, infoLog) {
        writeString(infoLog, gl.getShaderInfoLog(wrappers[shader]), length, maxLength);
    },
    glDeleteShader(shader) {
        gl.deleteShader(wrappers[shader]);
        wrappers[shader] = undefined;
    },

    glCreateProgram() {
        return wrap(gl.createProgram());
    },
    glAttachShader(program, shader) {
        gl.attachShader(wrappers[program], wrappers[shader]);
    },
    glLinkProgram(program) {
        gl.linkProgram(wrappers[program]);
    },
    glUseProgram(program) {
        gl.useProgram(wrappers[program]);
    },

    glGenVertexArrays(n, arrays) {
        const buffer = new Uint32Array(instance.exports.memory.buffer, arrays, n);
        for (let i = 0; i < n; i++) buffer[i] = wrap(gl.createVertexArray());
    },
    glBindVertexArray(array) {
        gl.bindVertexArray(wrappers[array]);
    },

    glGenBuffers(n, buffers) {
        const buffer = new Uint32Array(instance.exports.memory.buffer, buffers, n);
        for (let i = 0; i < n; i++) buffer[i] = wrap(gl.createBuffer());
    },
    glBindBuffer(target, buffer) {
        gl.bindBuffer(target, wrappers[buffer]);
    },
    glBufferData(target, size, data, usage) {
        gl.bufferData(target, new Uint8Array(instance.exports.memory.buffer, data, size), usage);
    },

    glGetUniformLocation(program, name) {
        return wrap(gl.getUniformLocation(wrappers[program], readString(name)));
    },
    glUniformMatrix4fv(location, count, transpose, value) {
        gl.uniformMatrix4fv(wrappers[location], transpose, new Float32Array(instance.exports.memory.buffer, value, 16));
    },
    glGetAttribLocation(program, name) {
        return gl.getAttribLocation(wrappers[program], readString(name));
    },
    glVertexAttribPointer(index, size, type, normalized, stride, pointer) {
        gl.vertexAttribPointer(index, size, type, normalized, stride, pointer);
    },
    glEnableVertexAttribArray(index) {
        gl.enableVertexAttribArray(index);
    },
    glDrawArrays(mode, first, count) {
        gl.drawArrays(mode, first, count);
    },

    glGenTextures(n, textures) {
        const buffer = new Uint32Array(instance.exports.memory.buffer, textures, n);
        for (let i = 0; i < n; i++) buffer[i] = wrap(gl.createTexture());
    },
    glBindTexture(target, texture) {
        gl.bindTexture(target, wrappers[texture]);
    },
    glTexParameteri(target, pname, param) {
        gl.texParameteri(target, pname, param);
    },
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels) {
        gl.texImage2D(target, level, internalformat, width, height, border, format, type, wrappers[pixels]);
    },
    glGenerateMipmap(target) {
        gl.generateMipmap(target);
    },
    glDeleteTextures(n, textures) {
        const buffer = new Uint32Array(instance.exports.memory.buffer, textures, n);
        for (let i = 0; i < n; i++) {
            gl.deleteTexture(wrappers[buffer[i]]);
            wrappers[buffer[i]] = undefined;
        }
    },

    Texture_Load(path, ptr, callback) {
        const image = new Image();
        image.src = `build/${readString(path)}`;
        image.onload = () => {
            instance.exports.__indirect_function_table.get(callback)(ptr, image.width, image.height, wrap(image));
        };
    },
    Texture_Free(data) {
        wrappers[data] = undefined;
    },
    TextTexture_Render(text, font, size, color, ptr, callback) {
        const fontPathPtr = new Uint32Array(instance.exports.memory.buffer, font, 1)[0];
        const fontPath = readString(fontPathPtr);
        const fontPathParts = fontPath.split('/');
        const fontName = fontPathParts[fontPathParts.length - 1].split('.')[0];
        if (!(fontName in fonts)) {
            fonts[fontName] = new FontFace(fontName, `url(build/${fontPath})`);
            fonts[fontName].load().then(font => {
                document.fonts.add(font);
                const canvas = renderText(text, fontName, size, color);
                instance.exports.__indirect_function_table.get(callback)(ptr, canvas.width, canvas.height, wrap(canvas));
            });
        } else {
            const canvas = renderText(text, fontName, size, color);
            instance.exports.__indirect_function_table.get(callback)(ptr, canvas.width, canvas.height, wrap(canvas));
        }
    },
    TextTexture_Free(data) {
        wrappers[data] = undefined;
    }
};

const useSIMD = await WebAssembly.validate(new Uint8Array([0,97,115,109,1,0,0,0,1,5,1,96,0,1,123,3,2,1,0,10,10,1,8,0,65,0,253,15,253,98,11]));
const { instance } = await WebAssembly.instantiateStreaming(fetch(`build/game${useSIMD ? '-simd' : ''}.wasm`), { env: bindings });

function resize() {
    canvas.width = window.innerWidth * window.devicePixelRatio;
    canvas.height = window.innerHeight * window.devicePixelRatio;
    canvas.style.width = `${window.innerWidth}px`;
    canvas.style.height = `${window.innerHeight}px`;
    instance.exports.resize(window.innerWidth, window.innerHeight, window.devicePixelRatio);
}
window.addEventListener('resize', resize);
resize();

instance.exports.init();
let time = window.performance.now();
function loop() {
    if (!running) return;
    window.requestAnimationFrame(loop);
    const newTime = window.performance.now();
    instance.exports.update(Math.min((newTime - time) / 1000, 1));
    time = newTime;
    instance.exports.render();
}
loop();
