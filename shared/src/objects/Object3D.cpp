#include "objects/Object3D.h"

Object3D::Object3D() {
    scale.x = 1;
    scale.y = 1;
    scale.z = 1;
}

void Object3D::updateMatrix() {
    matrix.translate(position.x, position.y, position.z);
    Matrix4 tempMatrix;
    tempMatrix.rotateX(rotation.x);
    matrix *= tempMatrix;
    tempMatrix.rotateY(rotation.y);
    matrix *= tempMatrix;
    tempMatrix.rotateZ(rotation.z);
    matrix *= tempMatrix;
    tempMatrix.scale(scale.x, scale.y, scale.z);
    matrix *= tempMatrix;
}
