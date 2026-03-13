#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include "Window.hpp"

typedef struct Vertex
{
    vec2 pos;
    vec3 col;
} Vertex;
 
// triangle
static const Vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};
 

// Very basic shaders to compile (stored in a const C string)
static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
static const char* fragment_shader_text =
"#version 330\n"
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
		Window w;
		// event listenner init
		glfwSetKeyCallback(w.window, key_callback);
		// load open gl
		gladLoadGL(glfwGetProcAddress);
		// swap interval between buffer and window
		glfwSwapInterval(1);

		// create gl buffer
		GLuint vertex_buffer;
		glGenBuffers(1, &vertex_buffer);											// generate a buffer, referenced by an ID
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);								// bind buffer to type GL_ARRAY_BUFFER
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	//copies the previously defined vertex data into the buffer's memory
	

		/*	Modern OpenGL requires that we at least set up a vertex 
			and fragment shader if we want to do some rendering
			In order for OpenGL to use the shader it has to 
			dynamically compile it at run-time from its source code. */
		// set up vertex shader
		const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);				// create a shader object, again referenced by an ID
		glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);				// attach the shader source code to the shader object
		glCompileShader(vertex_shader);												// compile the shader
		// set up fragment shader (The fragment shader is all about calculating the color output of pixels)
		const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
		glCompileShader(fragment_shader);
		// You want to check if shader compilation succeed
		int  success;
		char infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			// free memory and return
		}
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			// free memory and return
		}
		// final linked version of multiple shaders combined
		const GLuint shaderProgram = glCreateProgram();			// creates a program and returns the ID reference
		glAttachShader(shaderProgram, vertex_shader);			// attach the previously compiled shaders
		glAttachShader(shaderProgram, fragment_shader);
		glLinkProgram(shaderProgram);							// link them
		// Again, you want to check if linking succeed
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			// free memory and return
		}
		// delete the shader objects after linking them into a program cause we no longer need them anymore
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	
		const GLint mvp_location = glGetUniformLocation(shaderProgram, "MVP");
		const GLint vpos_location = glGetAttribLocation(shaderProgram, "vPos");
		const GLint vcol_location = glGetAttribLocation(shaderProgram, "vCol");
	
		// VAO Initialization code (done once (unless your object frequently changes))
		GLuint vertex_array;
		// genereate VAO (vertex array obj)
		glGenVertexArrays(1, &vertex_array);
		// bind VAO 
		glBindVertexArray(vertex_array);
		// set vertex attributes pointers
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,	\
							sizeof(Vertex), (void*) offsetof(Vertex, pos));
		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,	\
							sizeof(Vertex), (void*) offsetof(Vertex, col));
		// drawing code follows (in render loop)

		while (!glfwWindowShouldClose(w.window)) {
			int width, height;
			glfwGetFramebufferSize(w.window, &width, &height);
			const float ratio = width / (float)height;

			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);

			mat4x4 m, p, mvp;
			mat4x4_identity(m);
			mat4x4_rotate_Z(m, m, (float) glfwGetTime() * g_rotation_speed);
			mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
			mat4x4_mul(mvp, p, m);

			// draw the object
			glUseProgram(shaderProgram);						// activate the program object created before for shaders
			glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) &mvp);
			glBindVertexArray(vertex_array);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(w.window);

			glfwPollEvents();					// listen for event
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	return (0);
}