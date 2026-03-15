#include "Camera.hpp"

Camera::Camera(float fov, float near, float far) 
    : _fov(fov), _near(near), _far(far) {}

void Camera::move(float dx, float dy, float dz) {
    _eye[0]    += dx;
    _eye[1]    += dy;
    _eye[2]    += dz;
    _center[0] += dx;
    _center[1] += dy;
    _center[2] += dz;
}

void    Camera::getMVP(mat4x4 out, float aspect, const mat4x4 model) const {
    float eye[3]    = {_eye[0],    _eye[1],    _eye[2]};
    float center[3] = {_center[0], _center[1], _center[2]};
    float up[3]     = {_up[0],     _up[1],     _up[2]};

    mat4x4  view, proj, mv;
    mat4x4_look_at(view, eye, center, up);
    mat4x4_perspective(proj, _fov, aspect, _near, _far);
    mat4x4_mul(mv,  view,  (float(*)[4])(model));
    mat4x4_mul(out, proj,  mv);
}
