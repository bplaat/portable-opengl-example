#pragma once

#include "objects/Object3D.h"

class PerspectiveCamera : public Object3D {
  public:
    float fov;
    float aspect;
    float near;
    float far;

    PerspectiveCamera(float fov, float aspect, float near, float far);

    void updateMatrix();
};
