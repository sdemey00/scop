#pragma once

#include "Gl.hpp"
#include <vector>
#include <stdexcept>

struct Vertex {
    float   x, y, z;
    float   nx, ny, nz;
};

class Mesh {
    private:
        GLuint  _vertex_array   = 0;
        GLuint  _vertex_buffer  = 0;
        GLsizei _vertexCount    = 0;

        void destroy() noexcept;
    public:
        explicit Mesh(const std::vector<Vertex>& vertices);
        Mesh(const Mesh&)               = delete;
        Mesh& operator=(const Mesh&)    = delete;
        Mesh(Mesh&& o) noexcept;
        Mesh& operator=(Mesh&& o) noexcept;
        ~Mesh ();

        void    draw() const;
};