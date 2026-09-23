#pragma once

#include <Entity/Component.hpp>

namespace VLTEngine::Components
{

class Camera : public VLTEngine::Entity::Component
{
public:
    explicit Camera(
        VLTEngine::Entity::Entity* entity = nullptr
    );

    ~Camera() override = default;

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    void setPerspective(
        float fieldOfView,
        float aspectRatio,
        float nearClip,
        float farClip
    );

    float getFieldOfView() const;
    float getAspectRatio() const;
    float getNearClip() const;
    float getFarClip() const;

private:
    float m_fieldOfView;
    float m_aspectRatio;
    float m_nearClip;
    float m_farClip;
};

}