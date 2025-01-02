#ifndef CAMERA_H_
#define CAMERA_H_

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include "glm/ext.hpp"


// Camera's target point
const glm::vec3 CAMERA_TARGET(0.0f, 0.0f, 0.0f);

// Up vector
const glm::vec3 UPVECTOR(0.0f, 1.0f, 0.0f);

glm::vec3 scale(float scaleFactor);

// Model matrix (Identity matrix)
glm::mat4 identityMatrix(glm::vec3 rotation = {0.0f, 0.0f, 0.0f}, glm::vec3 scaling = scale(1.0f));


class Camera {
public:
    Camera() = default;
    ~Camera() = default;

    // Get camera matrices
    const glm::mat4& GetProjectionMatrix() const { return this->ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return this->ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return this->ViewProjectionMatrix; }

    // Set/Get position
    const glm::vec3& GetPosition() const { return this->Position; }
    void SetPosition(const glm::vec3& pos) { this->Position = pos; this->RecalculateMatrix(); }

protected:
    virtual void RecalculateMatrix() = 0;

protected:
    Camera(const Camera& camera) {
      this->ProjectionMatrix = camera.ProjectionMatrix;
      this->ViewMatrix = camera.ViewMatrix;
      this->Position = camera.Position;
      this->ViewProjectionMatrix = camera.ViewProjectionMatrix;
    }

protected:
    glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
    glm::vec3 Position = glm::vec3(0.0f);
};

#endif // CAMERA_H_
