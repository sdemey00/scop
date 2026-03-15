#pragma once

#include "GlShader.hpp"
#include <stdexcept>

/* link Shader*/

class GlProgram {
    private:
        GLuint _id;
    public:
        GlProgram(const GlProgram&)             = delete;
        GlProgram& operator=(const GlProgram&)  = delete;
        GlProgram(GlProgram&& o) noexcept : _id(o._id) { o._id = 0; }
        GlProgram& operator=(GlProgram&& o) noexcept {
            if (this != &o) {
                if (_id)
                    glDeleteProgram(_id);
                _id = o._id;
                o._id = 0;
            }
            return (*this);
        }
        GlProgram(const GlShader& vs, const GlShader& fs) {
            _id = glCreateProgram();
            glAttachShader(_id, vs.getId());
            glAttachShader(_id, fs.getId());
            glLinkProgram(_id);
            glDetachShader(_id, vs.getId());
            glDetachShader(_id, fs.getId());
            
            int success;
            glGetProgramiv(_id, GL_LINK_STATUS, &success);
            if (!success) {
                char log[512];
                glGetProgramInfoLog(_id, 512, NULL, log);
                throw std::runtime_error(log);
            }
        }
        void    use() const {
            glUseProgram(_id);
        }
        GLint   uniform(const char *name) const {
            return (glGetUniformLocation(_id, name));
        }
        GLint   getAttribLocation (const char *name) const {
            return (glGetAttribLocation(_id, name));
        }
        ~GlProgram() {
            if (_id)
                glDeleteProgram(_id);
        }
        GLuint getId() const { return (_id); }
};