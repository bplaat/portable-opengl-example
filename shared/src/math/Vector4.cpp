#include "math/Vector4.h"
#ifdef __ARM_NEON__
    #include <arm_neon.h>
#endif
#ifdef __SSE2__
    #include <emmintrin.h>
#endif

Vector4::Vector4() : x(0), y(0), z(0), w(1) {}

Vector4::Vector4(float x, float y, float z) : x(x), y(y), z(z), w(1) {}

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4 &Vector4::operator+=(float rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vaddq_f32(vld1q_f32(&x), vmovq_n_f32(rhs)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_add_ps(_mm_load_ps(&x), _mm_set1_ps(rhs)));
#else
    x += rhs;
    y += rhs;
    z += rhs;
    w += rhs;
#endif
    return *this;
}

Vector4 &Vector4::operator-=(float rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vsubq_f32(vld1q_f32(&x), vmovq_n_f32(rhs)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_sub_ps(_mm_load_ps(&x), _mm_set1_ps(rhs)));
#else
    x -= rhs;
    y -= rhs;
    z -= rhs;
    w -= rhs;
#endif
    return *this;
}

Vector4 &Vector4::operator*=(float rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vmulq_f32(vld1q_f32(&x), vmovq_n_f32(rhs)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_mul_ps(_mm_load_ps(&x), _mm_set1_ps(rhs)));
#else
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
#endif
    return *this;
}

Vector4 &Vector4::operator/=(float rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vdivq_f32(vld1q_f32(&x), vmovq_n_f32(rhs)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_div_ps(_mm_load_ps(&x), _mm_set1_ps(rhs)));
#else
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
#endif
    return *this;
}

Vector4 &Vector4::operator+=(Vector4 const &rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vaddq_f32(vld1q_f32(&x), vld1q_f32(&rhs.x)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_add_ps(_mm_load_ps(&x), _mm_load_ps(&rhs.x)));
#else
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
#endif
    return *this;
}

Vector4 &Vector4::operator-=(Vector4 const &rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vsubq_f32(vld1q_f32(&x), vld1q_f32(&rhs.x)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_sub_ps(_mm_load_ps(&x), _mm_load_ps(&rhs.x)));
#else
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
#endif
    return *this;
}

Vector4 &Vector4::operator*=(Vector4 const &rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vmulq_f32(vld1q_f32(&x), vld1q_f32(&rhs.x)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_mul_ps(_mm_load_ps(&x), _mm_load_ps(&rhs.x)));
#else
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
#endif
    return *this;
}

Vector4 &Vector4::operator/=(Vector4 const &rhs) {
#ifdef __ARM_NEON__
    vst1q_f32(&x, vdivq_f32(vld1q_f32(&x), vld1q_f32(&rhs.x)));
#elif defined __SSE2__
    _mm_store_ps(&x, _mm_div_ps(_mm_load_ps(&x), _mm_load_ps(&rhs.x)));
#else
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    w /= rhs.w;
#endif
    return *this;
}

Vector4 &Vector4::operator*=(Matrix4 const &rhs) {
#ifdef __ARM_NEON__
    float32x4_t vec = vld1q_f32(&x);
    float32x4_t sum = vmulq_f32(vld1q_f32(&rhs.elements[0]), vmovq_n_f32(vec[0]));
    sum = vmlaq_f32(sum, vld1q_f32(&rhs.elements[4]), vmovq_n_f32(vec[1]));
    sum = vmlaq_f32(sum, vld1q_f32(&rhs.elements[8]), vmovq_n_f32(vec[2]));
    sum = vmlaq_f32(sum, vld1q_f32(&rhs.elements[12]), vmovq_n_f32(vec[3]));
    vst1q_f32(&x, sum);
#elif defined __SSE2__
    __m128 vec = _mm_load_ps(&x);
    __m128 sum = _mm_mul_ps(_mm_load_ps(&rhs.elements[0]), _mm_set1_ps(vec[0]));
    sum = _mm_add_ps(sum, _mm_mul_ps(_mm_load_ps(&rhs.elements[4]), _mm_set1_ps(vec[1])));
    sum = _mm_add_ps(sum, _mm_mul_ps(_mm_load_ps(&rhs.elements[8]), _mm_set1_ps(vec[2])));
    sum = _mm_add_ps(sum, _mm_mul_ps(_mm_load_ps(&rhs.elements[12]), _mm_set1_ps(vec[3])));
    _mm_store_ps(&x, sum);
#else
    x = rhs.elements[0] * x + rhs.elements[4] * y + rhs.elements[8] * z + rhs.elements[12] * w;
    y = rhs.elements[1] * x + rhs.elements[5] * y + rhs.elements[9] * z + rhs.elements[13] * w;
    z = rhs.elements[2] * x + rhs.elements[6] * y + rhs.elements[10] * z + rhs.elements[14] * w;
    w = rhs.elements[3] * x + rhs.elements[7] * y + rhs.elements[11] * z + rhs.elements[15] * w;
#endif
    return *this;
}
