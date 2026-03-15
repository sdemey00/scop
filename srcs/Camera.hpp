#pragma once

#include "linmath.h"

/* view/projection math */

class Camera {
    private:
        float    _fov;
        float   _near;
        float   _far;
        float   _eye[3]    = {0.f, 0.f,  3.f};
        float   _center[3] = {0.f, 0.f,  0.f};
        float   _up[3]     = {0.f, 1.f,  0.f};

    public:
        Camera(float fov, float near, float far);

        void    move(float dx, float dy, float dz);
        void    getMVP(mat4x4 out, float aspect, const mat4x4 model) const;

        float speed = 5.0f;
};