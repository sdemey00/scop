#pragma once

#include "Gl.hpp"
#include <GLFW/glfw3.h>

class Framebuffer {
    public:
        explicit Framebuffer(GLFWwindow* window) : _window(window) {}

        void bind() const {
            int w, h;
            glfwGetFramebufferSize(_window, &w, &h);
            glViewport(0, 0, w, h);
        }

        void clear() const {
            glClearColor(_r, _g, _b, 1.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void setClearColor(float r, float g, float b) {
            _r = r; _g = g; _b = b;
        }

        float aspectRatio() const {
            int w, h;
            glfwGetFramebufferSize(_window, &w, &h);
            return (float)w / (float)h;
        }

    private:
        GLFWwindow* _window;
        float _r = 0.f, _g = 0.f, _b = 0.f;
};