#pragma once

#include "linmath.h"

#include <GLFW/glfw3.h>

/* transform state */

class Model {
    private:
        float _angle = 0.f;
    public:
        int   rotationAxis = 1;
        float rotationSpeed = 0.f;

        void handleKey(int key, int action) {
            if (action != GLFW_PRESS && action != GLFW_REPEAT) return;
            if (key == GLFW_KEY_UP    || key == GLFW_KEY_RIGHT) rotationSpeed += 0.5f;
            if (key == GLFW_KEY_DOWN  || key == GLFW_KEY_LEFT)  rotationSpeed -= 0.5f;
            if (key == GLFW_KEY_R) rotationSpeed = 0.f;
            if (key == GLFW_KEY_1) rotationAxis = 0; // X
            if (key == GLFW_KEY_2) rotationAxis = 1; // Y
            if (key == GLFW_KEY_3) rotationAxis = 2; // Z
        }

        void update(float dt) {
            _angle += dt * rotationSpeed;
        }

        void getMatrix(mat4x4 out) const {
            mat4x4_identity(out);
            if      (rotationAxis == 0) mat4x4_rotate_X(out, out, _angle);
            else if (rotationAxis == 1) mat4x4_rotate_Y(out, out, _angle);
            else if (rotationAxis == 2) mat4x4_rotate_Z(out, out, _angle);
        }
};