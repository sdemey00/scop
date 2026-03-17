#pragma once

#include "Gl.hpp"
#include <stdexcept>

/* Compile Shader */

class GlShader {
    private:
        GLuint  _id;
    public:
        GlShader(const GlShader&)               = delete;
        GlShader& operator=(const GlShader&)    = delete;
        GlShader(GlShader&& o) noexcept : _id(o._id) { o._id = 0; }
        GlShader& operator=(GlShader&& o) noexcept {
            if (this != &o) {
                if (_id)
                    glDeleteShader(_id);
                _id = o._id;
                o._id = 0;
            }
            return (*this);
        }
        GlShader(GLenum type, const char* src) {
            _id = glCreateShader(type);
            glShaderSource(_id, 1, &src, NULL);
            glCompileShader(_id);

            int success;
            glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
            if (!success) {
                char log[512];
                glGetShaderInfoLog(_id, 512, NULL, log);
                throw std::runtime_error(log);
            }
        }

        ~GlShader() {
            if (_id)
                glDeleteShader(_id);
        }

        GLuint getId() const { return (_id); }
};
