#include "VertexArray.h"

// Create a vertex array
VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);
}

VertexArray::~VertexArray() {
    glDeleteBuffers(GL_ARRAY_BUFFER, &m_vertexArrayID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}


void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    VertexBuffers.push_back(vertexBuffer);
}


void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer){
    IdxBuffer = indexBuffer;
}


