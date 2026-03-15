#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdexcept>

#include "Gl.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include "Window.hpp"
#include "GlShader.hpp"
#include "GlProgram.hpp"
#include "Mesh.hpp"

// TEMPORARY for testing
// triangle
static const Vertex vertices[3] = {
    { -0.6f, -0.4f, 0.f,   0.f, 0.f, 1.f },
    {  0.6f, -0.4f, 0.f,   0.f, 0.f, 1.f },
    {  0.0f,  0.6f, 0.f,   0.f, 0.f, 1.f },
};
// Very basic shaders to compile (stored in a const C string)
static const char* vertex_shader_text =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"uniform mat4 MVP;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(aPos, 1.0);\n"
"    color = aNormal * 0.5 + 0.5;\n"
"}\n";
static const char* fragment_shader_text =
"#version 330 core\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";
static float g_rotation_speed = 0.0f;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)scancode;(void)mods;
	std::cout << "Pressed: " << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
	else if ((key == GLFW_KEY_UP || key == GLFW_KEY_RIGHT)
		&& (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		g_rotation_speed += 0.5f;
		std::cout << "Rotation speed: " << g_rotation_speed << "x" << std::endl;
	}
	else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT)
	&& (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		g_rotation_speed -= 0.5f;
		std::cout << "Rotation speed: " << g_rotation_speed << "x" << std::endl;
	}
}

int main(void) {
	try {
		Window	w;
		glfwSetKeyCallback(w.window, key_callback);
		if (!gladLoadGL(glfwGetProcAddress))
			throw std::runtime_error("Failed to initialize GLAD");
		glfwSwapInterval(1);

		GlShader vs(GL_VERTEX_SHADER, vertex_shader_text);
		GlShader fs(GL_FRAGMENT_SHADER, fragment_shader_text);
		GlProgram shaderProgram(vs, fs);
		
		const GLint mvp_location = shaderProgram.uniform("MVP");
		if (mvp_location < 0)
			throw std::runtime_error("MVP uniform not found");

		std::vector<Vertex> verts(vertices, vertices + 3);
		Mesh mesh(verts);

		while (!glfwWindowShouldClose(w.window)) {
			int width, height;
			glfwGetFramebufferSize(w.window, &width, &height);
			const float ratio = width / (float)height;

			glViewport(0, 0, width, height); // -> window
			glClear(GL_COLOR_BUFFER_BIT);	// same

			// math
			mat4x4 m, p, mvp;
			mat4x4_identity(m);
			mat4x4_rotate_Z(m, m, (float) glfwGetTime() * g_rotation_speed);
			mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
			mat4x4_mul(mvp, p, m);

			shaderProgram.use();
			glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
			mesh.draw();

			glfwSwapBuffers(w.window);
			glfwPollEvents();					// listen for event
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}