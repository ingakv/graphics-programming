#pragma clang diagnostic push
#pragma ide diagnostic ignored "VirtualCallInCtorOrDtor"
#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Camera.h"
#include <array>
#include "glm/ext.hpp"

class PerspectiveCamera : public Camera {
public:

    struct Frustrum {
        float angle;
        float width;
        float height;
        float near;
        float far;
        glm::vec3 scaling;
        glm::vec3 rotation;
    };


public:

    PerspectiveCamera(
        const Frustrum& frustrum = {45.0f, -1.0f, 1.0f, 1.0f, -1.0f, scale(1.0f), {0.0f,0.0f,0.0f}},
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& lookAt = glm::vec3(-1.0f),
        const glm::vec3& upVector = glm::vec3(0.0f, 1.0f, 0.0f)) {

        this->LookAt = lookAt;
        this->UpVector = upVector;
        this->CameraFrustrum = frustrum;

        this->Position = position;

        this->RecalculateMatrix();

    }

    ~PerspectiveCamera() = default;

    PerspectiveCamera(const PerspectiveCamera& camera) : Camera(camera) {
        this->LookAt = camera.LookAt;
        this->UpVector = camera.UpVector;
        this->CameraFrustrum = camera.CameraFrustrum;
    }

    void SetFrustrum(const Frustrum& frustrum) { this->CameraFrustrum = frustrum; this->RecalculateMatrix(); }

    void SetLookAt(const glm::vec3& lookAt) { this->LookAt = lookAt; this->RecalculateMatrix(); }

    void SetUpVector(const glm::vec3& upVector) { this->UpVector = upVector; this->RecalculateMatrix(); }

protected:
    void RecalculateMatrix() override;

protected:
    glm::vec3 LookAt;
    glm::vec3 UpVector;
    Frustrum CameraFrustrum;
};

#endif // PERSPECTIVECAMERA_H_

#pragma clang diagnostic pop