#include "Camera.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "glm/ext.hpp"



glm::vec3 scale(float scaleFactor) {
    return {scaleFactor, scaleFactor, scaleFactor};
}

// Model matrix (Identity matrix)
glm::mat4 identityMatrix(glm::vec3 rotation, glm::vec3 scaling) {

    glm::mat4 identityMatrix(1.0f);

    // Translation
    identityMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

    // Rotation along the x-axis
    identityMatrix = glm::rotate(identityMatrix, rotation.x, {0.0f, 1.0f, 0.0f});

    // Rotation along the y-axis
    identityMatrix = glm::rotate(identityMatrix, rotation.y, {1.0f, 0.0f, 0.0f});

    // Rotation along the z-axis
    identityMatrix = glm::rotate(identityMatrix, rotation.z, {0.0f, 0.0f, 1.0f});

    // Scaling
    identityMatrix = glm::scale(identityMatrix, scaling);

    return identityMatrix;
}


void OrthographicCamera::RecalculateMatrix() {

    // Projection matrix
    this->ProjectionMatrix = glm::ortho(
            this->CameraFrustrum.left, this->CameraFrustrum.right,
            this->CameraFrustrum.bottom, this->CameraFrustrum.top,
            this->CameraFrustrum.near, this->CameraFrustrum.far);
    
    // View matrix
    this->ViewMatrix = glm::lookAt(this->Position, CAMERA_TARGET, UPVECTOR);


    // Update matrix with the transformation
    this->ViewProjectionMatrix = ProjectionMatrix * ViewMatrix * identityMatrix(this->CameraFrustrum.rotation);

}


void PerspectiveCamera::RecalculateMatrix() {

    // Projection matrix
    this->ProjectionMatrix = glm::perspective(
            glm::radians(this->CameraFrustrum.angle),
            this->CameraFrustrum.width / this->CameraFrustrum.height,
            this->CameraFrustrum.near,
            this->CameraFrustrum.far);

    this->ViewMatrix = glm::lookAt(this->Position, this->LookAt, this->UpVector);

    // Update matrix with the transformation
    this->ViewProjectionMatrix = ProjectionMatrix * ViewMatrix * identityMatrix(this->CameraFrustrum.rotation, this->CameraFrustrum.scaling);

}
