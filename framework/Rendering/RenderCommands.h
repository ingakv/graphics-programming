#ifndef RENDERCOMMANDS_H_
#define RENDERCOMMANDS_H_

#include "VertexArray.h"
#include "glm/vec4.hpp"

namespace RenderCommands {
    inline void Clear(const GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
        glClear(mode);
    }

    inline void SetPolygonMode(const GLenum face, const GLenum mode)
    {
        glPolygonMode(face, mode);
    }

    inline void DrawIndex(const std::shared_ptr<VertexArray>& vao, const GLenum primitive)
    {
        glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    // This function sets the background color. Use the glm vector types to pass the color
    inline void SetClearColor(const glm::vec4 col)
    {
        glClearColor(col.x, col.y, col.z, col.w);
    }

    // This function sets the drawing mode to only render border lines (me)
    inline void SetWireframeMode()
    {
        SetPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // This function sets the drawing mode to fill polygons
    inline void SetSolidMode()
    {
        SetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}


#endif // RENDERCOMMANDS_H_
