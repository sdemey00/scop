#pragma once

#include "Gl.hpp"
#include <vector>
#include <stdexcept>

/* GPU geometry */

struct Vertex {
    float   x, y, z;
    float   nx, ny, nz;
    float   u, v;   // texture coordinates
};

class Mesh {
    private:
        GLuint  _vao            = 0;
        GLuint  _vbo            = 0;
        GLuint  _ebo            = 0;
        GLsizei _vertexCount    = 0;
        GLsizei _indexCount     = 0;
        bool    _useIndices     = false;

        void    upload(const std::vector<Vertex>& vertices,
                const std::vector<unsigned int>& indices);
        void    destroy() noexcept;

    public:
        Mesh(const Mesh&)               = delete;
        Mesh& operator=(const Mesh&)    = delete;
        explicit Mesh(const std::vector<Vertex>& vertices);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        Mesh(Mesh&& o) noexcept;
        Mesh& operator=(Mesh&& o) noexcept;
        ~Mesh ();

        void    draw() const;
};