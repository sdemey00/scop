#include <stdexcept>

class Window {
	public:
		Window() {

			if (!glfwInit())
				throw std::runtime_error("Failed init glfw");
			window = glfwCreateWindow(width, height, "Scop", NULL, NULL);
			if (!window) {
				glfwTerminate();
				throw std::runtime_error("Failed creating window");
			}
			glfwMakeContextCurrent(window);		// OpenGL context
		}

		Window(int w, int h) {
			if (w <= 0 || h <= 0)
				throw std::runtime_error("Invalid window");
			width = w;
			height = h;
			if (!glfwInit())
				throw std::runtime_error("Failed init glfw");
			window = glfwCreateWindow(width, height, "Scop", NULL, NULL);
			if (!window) {
				glfwTerminate();
				throw std::runtime_error("Failed creating window");
			}
			glfwMakeContextCurrent(window);		// OpenGL context
		};
		
		~Window() {
			glfwDestroyWindow(window);	// free window allocated memory
			glfwTerminate();			// free glfwInit allocated memory
		};

		int 		width = 1920;
		int			height = 1080;
		GLFWwindow*	window;
};