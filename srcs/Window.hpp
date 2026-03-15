#pragma once

#include "Gl.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

/* GLFW life cycle */

class Window {
	public:
		Window();
		Window(int w, int h);
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();

		void	setUserPointer(void* ptr);
		void	setKeyCallback(GLFWkeyfun cb);
		void	swapBuffers()  const;
		void	pollEvents()   const;
		bool	shouldClose()  const;
		void	close();
		
		template<typename T>
		T* getUserPointer() const {
			return static_cast<T*>(glfwGetWindowUserPointer(window));
		}

		int 		width = 1920;
		int			height = 1080;
		GLFWwindow*	window;
};