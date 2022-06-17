#include "math/Matrix4.h"
#include "std.h"
#ifdef __ARM_NEON__
    #include <arm_neon.h>
#endif
#ifdef __SSE2__
    #include <emmintrin.h>
#endif
#include "math/utils.h"

void Matrix4::identity() {
    elements[0] = 1;
    elements[1] = 0;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = 1;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = 0;
    elements[10] = 1;
    elements[11] = 0;
    elements[12] = 0;
    elements[13] = 0;
    elements[14] = 0;
    elements[15] = 1;
}

void Matrix4::perspective(float fov, float aspect, float _near, float _far) {
    float f = tan(M_PI * 0.5f - 0.5f * fov);
    float r = 1.0 / (_near - _far);
    elements[0] = f / aspect;
    elements[1] = 0;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = f;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = 0;
    elements[10] = (_near + _far) * r;
    elements[11] = -1;
    elements[12] = 0;
    elements[13] = 0;
    elements[14] = _near * _far * r * 2;
    elements[15] = 0;
}

void Matrix4::translate(float x, float y, float z) {
    elements[0] = 1;
    elements[1] = 0;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = 1;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = 0;
    elements[10] = 1;
    elements[11] = 0;
    elements[12] = x;
    elements[13] = y;
    elements[14] = z;
    elements[15] = 1;
}

void Matrix4::rotateX(float x) {
    float s = sin(x);
    float c = cos(x);
    elements[0] = 1;
    elements[1] = 0;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = c;
    elements[6] = s;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = -s;
    elements[10] = c;
    elements[11] = 0;
    elements[12] = 0;
    elements[13] = 0;
    elements[14] = 0;
    elements[15] = 1;
}

void Matrix4::rotateY(float y) {
    float s = sin(y);
    float c = cos(y);
    elements[0] = c;
    elements[1] = 0;
    elements[2] = -s;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = 1;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = s;
    elements[9] = 0;
    elements[10] = c;
    elements[11] = 0;
    elements[12] = 0;
    elements[13] = 0;
    elements[14] = 0;
    elements[15] = 1;
}

void Matrix4::rotateZ(float z) {
    float s = sin(z);
    float c = cos(z);
    elements[0] = c;
    elements[1] = s;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = -s;
    elements[5] = c;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = 0;
    elements[10] = 1;
    elements[11] = 0;
    elements[12] = 0;
    elements[13] = 0;
    elements[14] = 0;
    elements[15] = 1;
}

void Matrix4::scale(float x, float y, float z) {
    elements[0] = x;
    elements[1] = 0;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = y;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = 0;
    elements[10] = z;
    elements[11] = 0;
    elements[12] = 0;
    elements[13] = 0;
    elements[14] = 0;
    elements[15] = 1;
}

void Matrix4::flat(float width, float height) {
    elements[0] = 2 / width;
    elements[1] = 0;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = -2 / height;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = 0;
    elements[10] = 1;
    elements[11] = 0;
    elements[12] = -1;
    elements[13] = 1;
    elements[14] = 0;
    elements[15] = 1;
}

void Matrix4::rect(float x, float y, float width, float height) {
    elements[0] = width;
    elements[1] = 0;
    elements[2] = 0;
    elements[3] = 0;
    elements[4] = 0;
    elements[5] = height;
    elements[6] = 0;
    elements[7] = 0;
    elements[8] = 0;
    elements[9] = 0;
    elements[10] = 1;
    elements[11] = 0;
    elements[12] = x + width / 2;
    elements[13] = y + height / 2;
    elements[14] = 0;
    elements[15] = 1;
}

Matrix4 &Matrix4::operator*=(Matrix4 const &rhs) {
#ifdef __ARM_NEON__
    float32x4_t a0 = vld1q_f32(&elements[0]);
    float32x4_t a1 = vld1q_f32(&elements[4]);
    float32x4_t a2 = vld1q_f32(&elements[8]);
    float32x4_t a3 = vld1q_f32(&elements[12]);

    float32x4_t b = vld1q_f32(&rhs.elements[0]);
    float32x4_t sum = vmulq_f32(a0, vmovq_n_f32(b[0]));
    sum = vmlaq_f32(sum, a1, vmovq_n_f32(b[1]));
    sum = vmlaq_f32(sum, a2, vmovq_n_f32(b[2]));
    sum = vmlaq_f32(sum, a3, vmovq_n_f32(b[3]));
    vst1q_f32(&elements[0], sum);

    b = vld1q_f32(&rhs.elements[4]);
    sum = vmulq_f32(a0, vmovq_n_f32(b[0]));
    sum = vmlaq_f32(sum, a1, vmovq_n_f32(b[1]));
    sum = vmlaq_f32(sum, a2, vmovq_n_f32(b[2]));
    sum = vmlaq_f32(sum, a3, vmovq_n_f32(b[3]));
    vst1q_f32(&elements[4], sum);

    b = vld1q_f32(&rhs.elements[8]);
    sum = vmulq_f32(a0, vmovq_n_f32(b[0]));
    sum = vmlaq_f32(sum, a1, vmovq_n_f32(b[1]));
    sum = vmlaq_f32(sum, a2, vmovq_n_f32(b[2]));
    sum = vmlaq_f32(sum, a3, vmovq_n_f32(b[3]));
    vst1q_f32(&elements[8], sum);

    b = vld1q_f32(&rhs.elements[12]);
    sum = vmulq_f32(a0, vmovq_n_f32(b[0]));
    sum = vmlaq_f32(sum, a1, vmovq_n_f32(b[1]));
    sum = vmlaq_f32(sum, a2, vmovq_n_f32(b[2]));
    sum = vmlaq_f32(sum, a3, vmovq_n_f32(b[3]));
    vst1q_f32(&elements[12], sum);
#elif defined __SSE2__
    __m128 a0 = _mm_load_ps(&elements[0]);
    __m128 a1 = _mm_load_ps(&elements[4]);
    __m128 a2 = _mm_load_ps(&elements[8]);
    __m128 a3 = _mm_load_ps(&elements[12]);

    __m128 b = _mm_load_ps(&rhs.elements[0]);
    __m128 sum = _mm_mul_ps(a0, _mm_set1_ps(b[0]));
    sum = _mm_add_ps(sum, _mm_mul_ps(a1, _mm_set1_ps(b[1])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a2, _mm_set1_ps(b[2])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a3, _mm_set1_ps(b[3])));
    _mm_store_ps(&elements[0], sum);

    b = _mm_load_ps(&rhs.elements[4]);
    sum = _mm_mul_ps(a0, _mm_set1_ps(b[0]));
    sum = _mm_add_ps(sum, _mm_mul_ps(a1, _mm_set1_ps(b[1])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a2, _mm_set1_ps(b[2])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a3, _mm_set1_ps(b[3])));
    _mm_store_ps(&elements[4], sum);

    b = _mm_load_ps(&rhs.elements[8]);
    sum = _mm_mul_ps(a0, _mm_set1_ps(b[0]));
    sum = _mm_add_ps(sum, _mm_mul_ps(a1, _mm_set1_ps(b[1])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a2, _mm_set1_ps(b[2])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a3, _mm_set1_ps(b[3])));
    _mm_store_ps(&elements[8], sum);

    b = _mm_load_ps(&rhs.elements[12]);
    sum = _mm_mul_ps(a0, _mm_set1_ps(b[0]));
    sum = _mm_add_ps(sum, _mm_mul_ps(a1, _mm_set1_ps(b[1])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a2, _mm_set1_ps(b[2])));
    sum = _mm_add_ps(sum, _mm_mul_ps(a3, _mm_set1_ps(b[3])));
    _mm_store_ps(&elements[12], sum);
#else
    float a00 = elements[0];
    float a01 = elements[1];
    float a02 = elements[2];
    float a03 = elements[3];
    float a10 = elements[4];
    float a11 = elements[5];
    float a12 = elements[6];
    float a13 = elements[7];
    float a20 = elements[8];
    float a21 = elements[9];
    float a22 = elements[10];
    float a23 = elements[11];
    float a30 = elements[12];
    float a31 = elements[13];
    float a32 = elements[14];
    float a33 = elements[15];

    float b0 = rhs.elements[0];
    float b1 = rhs.elements[1];
    float b2 = rhs.elements[2];
    float b3 = rhs.elements[3];
    elements[0] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    elements[1] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    elements[2] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    elements[3] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

    b0 = rhs.elements[4];
    b1 = rhs.elements[5];
    b2 = rhs.elements[6];
    b3 = rhs.elements[7];
    elements[4] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    elements[5] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    elements[6] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    elements[7] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

    b0 = rhs.elements[8];
    b1 = rhs.elements[9];
    b2 = rhs.elements[10];
    b3 = rhs.elements[11];
    elements[8] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    elements[9] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    elements[10] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    elements[11] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;

    b0 = rhs.elements[12];
    b1 = rhs.elements[13];
    b2 = rhs.elements[14];
    b3 = rhs.elements[15];
    elements[12] = b0 * a00 + b1 * a10 + b2 * a20 + b3 * a30;
    elements[13] = b0 * a01 + b1 * a11 + b2 * a21 + b3 * a31;
    elements[14] = b0 * a02 + b1 * a12 + b2 * a22 + b3 * a32;
    elements[15] = b0 * a03 + b1 * a13 + b2 * a23 + b3 * a33;
#endif
    return *this;
}
