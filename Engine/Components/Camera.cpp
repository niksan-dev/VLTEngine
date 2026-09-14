#include <Components/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace VLTEngine::Components
{

    Camera::Camera()
        : m_position(0.0f, 0.0f, 3.0f),
          m_rotation(0.0f, 0.0f, 0.0f),
          m_fieldOfView(glm::radians(60.0f)),
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

    void Camera::setPosition(
        const glm::vec3 &position)
    {
        m_position = position;
    }

    void Camera::setRotation(
        const glm::vec3 &rotation)
    {
        m_rotation = rotation;
    }

    const glm::vec3 &Camera::getPosition() const
    {
        return m_position;
    }

    const glm::vec3 &Camera::getRotation() const
    {
        return m_rotation;
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

    glm::mat4 Camera::getViewMatrix() const
    {
        glm::mat4 rotation(1.0f);

        rotation =
            glm::rotate(
                rotation,
                -m_rotation.z,
                glm::vec3(0.0f, 0.0f, 1.0f));

        rotation =
            glm::rotate(
                rotation,
                -m_rotation.y,
                glm::vec3(0.0f, 1.0f, 0.0f));

        rotation =
            glm::rotate(
                rotation,
                -m_rotation.x,
                glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 translation =
            glm::translate(
                glm::mat4(1.0f),
                -m_position);

        return rotation * translation;
    }

    glm::mat4 Camera::getProjectionMatrix() const
    {
        return glm::perspective(
            m_fieldOfView,
            m_aspectRatio,
            m_nearClip,
            m_farClip);
    }

}