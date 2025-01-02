#pragma clang diagnostic push
#pragma ide diagnostic ignored "VirtualCallInCtorOrDtor"
#ifndef ORTHOGRAPHICCAMERA_H_
#define ORTHOGRAPHICCAMERA_H_

#include <array>
#include "glm/ext.hpp"

class OrthographicCamera : public Camera {
public:

    struct Frustrum {
        float left;
        float right;
        float bottom;
        float top;
        float near;
        float far;
        glm::vec3 rotation;
    };

public:
    OrthographicCamera(const Frustrum& frustrum = {-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, {0.0f,0.0f,0.0f}},
                     const glm::vec3& position = glm::vec3(0.0f)) {

        this->CameraFrustrum = frustrum;

        this->Position = position;

        this->RecalculateMatrix();

    };

    ~OrthographicCamera() = default;

    OrthographicCamera(const OrthographicCamera& camera) : Camera(camera) {
        this->CameraFrustrum = camera.CameraFrustrum;
    }

    void SetFrustrum(const Frustrum& frustrum) { this->CameraFrustrum =frustrum; this->RecalculateMatrix(); }

protected:
    void RecalculateMatrix() override;

protected:
    Frustrum CameraFrustrum;
};

#endif // ORTHOGRAPHICCAMERA_H_

#pragma clang diagnostic pop