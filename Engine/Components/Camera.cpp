#include <Components/Camera.hpp>

namespace VLTEngine::Components
{

    Camera::Camera(
        VLTEngine::Entity::Entity *entity)
        : Component(entity),
          m_fieldOfView(1.04719755f), // 60 degrees in radians
          m_aspectRatio(16.0f / 9.0f),
          m_nearClip(0.1f),
          m_farClip(100.0f)
    {
    }

    void Camera::setPerspective(
        float fieldOfView,
        float aspectRatio,
        float nearClip,
        float farClip)
    {
        m_fieldOfView = fieldOfView;
        m_aspectRatio = aspectRatio;
        m_nearClip = nearClip;
        m_farClip = farClip;
    }

    float Camera::getFieldOfView() const
    {
        return m_fieldOfView;
    }

    float Camera::getAspectRatio() const
    {
        return m_aspectRatio;
    }

    float Camera::getNearClip() const
    {
        return m_nearClip;
    }

    float Camera::getFarClip() const
    {
        return m_farClip;
    }

}