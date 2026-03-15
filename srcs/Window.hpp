#pragma once

#include <GLFW/glfw3.h>
#include <stdexcept>

class Window {
	public:
		Window() : Window(1920, 1080) {}
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(int w, int h) {
			if (w <= 0 || h <= 0)
				throw std::runtime_error("Invalid window dimension");
			width = w; height = h;
			if (!glfwInit())
				throw std::runtime_error("Failed to init GLFW");
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			window = glfwCreateWindow(width, height, "Scop", NULL, NULL);
			if (!window) {
				glfwTerminate();
				throw std::runtime_error("Failed creating window");
			}
			glfwMakeContextCurrent(window);
		};
		
		~Window() {
			glfwDestroyWindow(window);
			glfwTerminate();
		};

		int 		width = 1920;
		int			height = 1080;
		GLFWwindow*	window;
};