#pragma once

#include <GLFW/glfw3.h>
#include "Window.hpp"
#include "Model.hpp"
#include "Camera.hpp"

/* Orchestration */

class GlfwInputHandler {
    public:
        GlfwInputHandler(Window& window, Model& model, Camera& camera)
            : _window(window), _model(model), _camera(camera) {}

        void setup() {
            _window.setUserPointer(this);
            _window.setKeyCallback([](GLFWwindow* win, int key, int, int action, int) {
                static_cast<GlfwInputHandler*>(
                    glfwGetWindowUserPointer(win))->onKey(key, action);
            });
        }

        void onKey(int key, int action) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                _window.close();
            _model.handleKey(key, action);
        }

        void poll(float dt) {
            const float v = _camera.speed * dt;
            GLFWwindow* win = _window.window;

            if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) _camera.move( 0,  0, -v);
            if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) _camera.move( 0,  0,  v);
            if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) _camera.move(-v,  0,  0);
            if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) _camera.move( v,  0,  0);
            if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) _camera.move( 0,  v,  0);
            if (glfwGetKey(win, GLFW_KEY_X)     == GLFW_PRESS) _camera.move( 0, -v,  0);
        }

    private:
        Window&  _window;
        Model&   _model;
        Camera&  _camera;
};