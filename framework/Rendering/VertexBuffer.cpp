#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(const void *vertices, const GLsizei size) {

    glGenBuffers(1, &VertexBufferID);
    Bind();

    // Populate the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Layout.GetAttributes().front().Type) * size, vertices, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(GL_ARRAY_BUFFER, &VertexBufferID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, -1);
}

void VertexBuffer::BufferSubData(const GLintptr offset, const GLsizeiptr size, const void *data) const {
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
