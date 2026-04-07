#include "GlTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

#include <stdexcept>

GlTexture::GlTexture(GlTexture&& o) noexcept : _id(o._id) {
    o._id = 0;
}

GlTexture& GlTexture::operator=(GlTexture&& o) noexcept {
    if (this != &o) {
        if (_id)
            glDeleteTextures(1, &_id);
        _id = o._id;
        o._id = 0;
    }
    return (*this);
}

GlTexture::GlTexture(const std::string& path) {
    int width = 0;
    int height = 0;
    int nrChannels = 0;

    stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
        throw std::runtime_error("Failed loading texture file: " + path);

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB8;
    if (nrChannels == 1) {
        format = GL_RED;
        internalFormat = GL_R8;
    } else if (nrChannels == 3) {
        format = GL_RGB;
        internalFormat = GL_RGB8;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    } else {
        stbi_image_free(data);
        throw std::runtime_error("Unsupported texture channel count in: " + path);
    }

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void GlTexture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

GlTexture::~GlTexture() {
    if (_id)
        glDeleteTextures(1, &_id);
}
