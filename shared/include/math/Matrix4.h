#pragma once

struct __attribute__((aligned(16))) Matrix4 {
    float elements[16];

    void identity();

    void perspective(float fov, float aspect, float _near, float _far);

    void translate(float x, float y, float z);

    void rotateX(float x);

    void rotateY(float y);

    void rotateZ(float z);

    void scale(float x, float y, float z);

    void flat(float width, float height);

    void rect(float x, float y, float width, float height);

    Matrix4 &operator*=(Matrix4 const &rhs);
};
