#include "objects/PerspectiveCamera.h"
#include "math/utils.h"

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : fov(fov), aspect(aspect), near(near), far(far) {}

void PerspectiveCamera::updateMatrix() {
    matrix.perspective(fov, aspect, near, far);
    Matrix4 tempMatrix;
    tempMatrix.rotateX(rotation.x);
    matrix *= tempMatrix;
    tempMatrix.rotateY(rotation.y);
    matrix *= tempMatrix;
    tempMatrix.rotateZ(rotation.z);
    matrix *= tempMatrix;
    tempMatrix.translate(position.x, position.y, position.z);
    matrix *= tempMatrix;
}
