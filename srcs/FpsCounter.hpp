#pragma once

#include "linmath.h"

#include <GLFW/glfw3.h>
#include <iostream>

/* frame rate counter */

class FpsCounter {
    public:
        void tick(GLFWwindow* window, float dt) {
            _count += 1;
            _timer += dt;
            if (_timer >= 1.0f) {
                std::string title = "Scop  |  FPS: " + std::to_string(_count);
                glfwSetWindowTitle(window, title.c_str());
                std::cout << "FPS: " << _count << "\r" << std::flush;
                _count = 0;
                _timer -= 1.0f;
            }
        }

    private:
        float _timer = 0.f;
        int   _count = 0;
};
