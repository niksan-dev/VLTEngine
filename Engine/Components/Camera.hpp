#pragma once

#include <Entity/Component.hpp>

#include <glm/glm.hpp>

namespace VLTEngine::Components
{

    class Camera
        : public VLTEngine::Entity::Component
    {
    public:
        explicit Camera(
            VLTEngine::Entity::Entity *entity = nullptr);

        ~Camera() override = default;

        Camera(const Camera &) = default;
        Camera &operator=(const Camera &) = default;

        void setPerspective(
            float fieldOfView,
            float aspectRatio,
            float nearClip,
            float farClip);

        void setPosition(
            const glm::vec3 &position);

        void setRotation(
            const glm::vec3 &rotation);

        const glm::vec3 &getPosition() const;
        const glm::vec3 &getRotation() const;

        float getFieldOfView() const;
        float getAspectRatio() const;
        float getNearClip() const;
        float getFarClip() const;

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

    private:
        glm::vec3 m_position;
        glm::vec3 m_rotation;

        float m_fieldOfView;
        float m_aspectRatio;
        float m_nearClip;
        float m_farClip;
    };

}