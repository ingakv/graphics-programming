#include "IndexBuffer.h"
#include "BufferLayout.h"


IndexBuffer::IndexBuffer(GLuint *indices, GLsizei count, const BufferLayout& layout) {

    Count = count*ShaderDataTypeSize(ShaderDataType::Int);

    glGenBuffers(1, &IndexBufferID);
    Bind();

    // Create a vertex attribute pointer for each attribute in the buffer layout
    int i = 0;
    for (auto &attribute: layout.GetAttributes()) {

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,
            ShaderDataTypeComponentCount(attribute.Type),
            ShaderDataTypeToOpenGLBaseType(attribute.Type),
            attribute.Normalized,
            layout.GetStride(),
            (void*)attribute.Offset);
        i++;
    }

    // Populate the index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count, indices, GL_STATIC_DRAW);


}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &IndexBufferID);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
