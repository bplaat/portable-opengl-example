#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "math/Matrix4.h"
#include "math/Vector4.h"

class Object3D {
  public:
    Vector4 position;
    Vector4 rotation;
    Vector4 scale;
    Matrix4 matrix;

    Object3D();

    void updateMatrix();
};

#endif
