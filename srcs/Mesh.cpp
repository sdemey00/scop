#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& vertices) {
    upload(vertices, {});
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : _indexCount(static_cast<GLsizei>(indices.size())), _useIndices(true)
{
    upload(vertices, indices);
}

Mesh::Mesh(Mesh&& o) noexcept : _vao(o._vao), _vbo(o._vbo), _ebo(o._ebo),
    _vertexCount(o._vertexCount), _indexCount(o._indexCount), _useIndices(o._useIndices)
{
    o._vao = o._vbo = o._ebo = 0;
    o._vertexCount = o._indexCount = 0;
    o._useIndices = false;
}

Mesh& Mesh::operator=(Mesh&& o) noexcept {
    if (this != &o) {
        destroy();
        _vao = o._vao;
        _vbo = o._vbo;
        _ebo = o._ebo;
        _vertexCount = o._vertexCount;
        _indexCount = o._indexCount;
        _useIndices = o._useIndices;
        o._vao = o._vbo = o._ebo = 0;
        o._vertexCount = o._indexCount = 0;
        o._useIndices = false;
    }
    return (*this);
}
Mesh::~Mesh() {
    destroy();
}

void Mesh::draw() const {
    glBindVertexArray(_vao);
    if (_useIndices)
        glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
}

void    Mesh::destroy() noexcept {
    if (_ebo) glDeleteBuffers(1, &_ebo);
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (_vao) glDeleteVertexArrays(1, &_vao);
    _vao = _vbo = _ebo = 0;
}

void    Mesh::upload(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices) {
        if (vertices.empty())
            throw std::runtime_error("Mesh: empty vertex array");
         _useIndices = !indices.empty();
        _vertexCount = static_cast<GLsizei>(vertices.size());
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glBindVertexArray(_vao);
        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW);
        // EBO
        if (_useIndices) {
            glGenBuffers(1, &_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                indices.size() * sizeof(unsigned int),
                indices.data(),
                GL_STATIC_DRAW);
        }
        // Deterministic mapping
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, x));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, nx));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, u));
        glBindVertexArray(0);    // unbind VAO before VBO/EBO to avoid accidental state changes   
}