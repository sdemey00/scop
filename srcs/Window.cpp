#include "Window.hpp"

Window::Window() : Window(1920, 1080) {}

Window::Window(int w, int h) {
    if (w <= 0 || h <= 0)
        throw std::runtime_error("Invalid window dimension");
    width = w; height = h;
    if (!glfwInit())
        throw std::runtime_error("Failed to init GLFW");
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, "Scop", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed creating window");
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");
    glfwSwapInterval(1);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setUserPointer(void* ptr)      { glfwSetWindowUserPointer(window, ptr); }
void Window::setKeyCallback(GLFWkeyfun cb)  { glfwSetKeyCallback(window, cb); }
void Window::swapBuffers()  const           { glfwSwapBuffers(window); }
void Window::pollEvents()   const           { glfwPollEvents(); }
bool Window::shouldClose()  const           { return (glfwWindowShouldClose(window)); }
void Window::close()                        { glfwSetWindowShouldClose(window, GLFW_TRUE); }
