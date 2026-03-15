#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices) {
    _vertexCount = static_cast<GLsizei>(vertices.size());
    glGenVertexArrays(1, &_vertex_array);
    glGenBuffers(1, &_vertex_buffer);
    glBindVertexArray(_vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex), (void*)offsetof(Vertex, nx));
    glBindVertexArray(0);
}

Mesh::Mesh(Mesh&& o) noexcept : _vertex_array(o._vertex_array), _vertex_buffer(o._vertex_buffer), _vertexCount(o._vertexCount) {
    o._vertex_array = o._vertex_buffer = 0;
}

Mesh& Mesh::operator=(Mesh&& o) noexcept {
    if (this != &o) {
        destroy();
        _vertex_array = o._vertex_array;
        _vertex_buffer = o._vertex_buffer;
        _vertexCount = o._vertexCount;
        o._vertex_array = 0;
        o._vertex_buffer = 0;
        o._vertexCount = 0;
    }
    return (*this);
}
Mesh::~Mesh() {
    destroy();
}

void    Mesh::draw() const {
    glBindVertexArray(_vertex_array);
    glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
    glBindVertexArray(0); // clean gl state
}

void    Mesh::destroy() noexcept {
    if (_vertex_buffer) glDeleteBuffers(1, &_vertex_buffer);
    if (_vertex_array) glDeleteVertexArrays(1, &_vertex_array);
    _vertex_array = _vertex_buffer = 0;
}