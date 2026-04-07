#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdexcept>
#include <optional>
#include <vector>

#include "Gl.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"

#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "ObjLoader.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "FpsCounter.hpp"
#include "Framebuffer.hpp"
#include "GlfwInputHandler.hpp"
#include "GlTexture.hpp"

int main(int ac, char **av) {
	try {
		if (ac < 2) {
			throw std::runtime_error("Usage: ./scope <file.obj>");
		}
		Window			w;
		Framebuffer 	fb(w.window);
		Camera			camera(1.0472f, 0.1f, 100.f);
		ObjScene		scene = ObjLoader::loadScene(av[1]);
		Shader			shader("shaders/default.vert", "shaders/default.frag");
		Model			model;
		FpsCounter		fps;
		GlfwInputHandler	input(w, model, camera);
		std::vector<std::optional<GlTexture>> textures;

		textures.reserve(scene.submeshes.size());
		for (const ObjSubmesh& submesh : scene.submeshes) {
			textures.emplace_back();
			if (submesh.diffuseTexturePath)
				textures.back().emplace(*submesh.diffuseTexturePath);
		}

		glEnable(GL_DEPTH_TEST);
		input.setup();

		float lastTime = (float)glfwGetTime();
		while (!w.shouldClose()) {
			float	now = (float)glfwGetTime();
            float	dt  = now - lastTime;
            lastTime  = now;

			fb.bind();
			fb.clear();
			fps.tick(w.window, dt);
			input.poll(dt);
			model.update(dt);
            mat4x4 modelMat, mvp;
            model.getMatrix(modelMat);
            camera.getMVP(mvp, fb.aspectRatio(), modelMat);
			shader.use();
			shader.setMat4("MVP", mvp);
			for (size_t i = 0; i < scene.submeshes.size(); ++i) {
				shader.setInt("uTexture", 0);
				shader.setBool("uHasTexture", textures[i].has_value());
				if (textures[i])
					textures[i]->bind(0);
				scene.submeshes[i].mesh.draw();
			}
			w.swapBuffers();
			w.pollEvents();
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
