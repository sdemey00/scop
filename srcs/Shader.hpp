#pragma once

#include "GlProgram.hpp"
#include "GlShader.hpp"
#include "linmath.h"
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

class Shader {
    private:
        GlProgram   _program;

        static GlShader loadShader(GLenum type, const std::string& path) {
            std::ifstream file(path);
            if (!file.is_open())
                throw std::runtime_error("Shader: cannot open file: " + path);
            std::ostringstream ss;
            ss << file.rdbuf();
            return (GlShader(type, ss.str().c_str()));
        }

    public:
        Shader(const Shader&)            = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(const std::string& vertPath, const std::string& fragPath)
            : _program(loadShader(GL_VERTEX_SHADER,   vertPath),
                    loadShader(GL_FRAGMENT_SHADER, fragPath)) {}

        void use() const {
            _program.use();
        }

        void setMat4(const char* name, const mat4x4 m) const {
            GLint loc = _program.uniform(name);
            if (loc < 0) {
                std::cout << "Shader: uniform not found: " << name << std::endl;
                return ;
            }
            glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)m);
        }
};