#pragma once

#include "math/Matrix4.h"

struct __attribute__((aligned(16))) Vector4 {
    float x;
    float y;
    float z;
    float w;

    Vector4();

    Vector4(float x, float y, float z);

    Vector4(float x, float y, float z, float w);

    Vector4 &operator+=(float rhs);

    Vector4 &operator-=(float rhs);

    Vector4 &operator*=(float rhs);

    Vector4 &operator/=(float rhs);

    Vector4 &operator+=(Vector4 const &rhs);

    Vector4 &operator-=(Vector4 const &rhs);

    Vector4 &operator*=(Vector4 const &rhs);

    Vector4 &operator/=(Vector4 const &rhs);

    Vector4 &operator*=(Matrix4 const &rhs);
};
