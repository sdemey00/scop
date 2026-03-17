#pragma once

#include "Gl.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"	// TEMPORARY

class GlTexture {
	private:
		GLuint	_id;
	public:
		GlTexture(const GlTexture&)				= delete;
        GlTexture& operator=(const GlTexture&)	= delete;
        GlTexture(GlTexture&& o) noexcept : _id(o._id) { o._id = 0; }
        GlTexture& operator=(GlTexture&& o) noexcept {
            if (this != &o) {
                if (_id)
                    glDeleteTextures(1 ,&_id);
                _id = o._id;
                o._id = 0;
            }
            return (*this);
        }

		GlTexture(const std::string& path) {
			glGenTextures(1, &_id);
			glBindTexture(GL_TEXTURE_2D, _id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int width, height, nrChannels;
			unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
			if (!data) {
				throw std::runtime_error("Failed loading texture file");
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}

		void bind(unsigned int unit = 0) {
			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, _id);
		};

		~GlTexture() {
			if (_id)
				glDeleteTextures(1 ,&_id);
		}
};
