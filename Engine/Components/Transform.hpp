#pragma once

#include <Entity/Component.hpp>

#include <glm/glm.hpp>

namespace VLTEngine::Components
{

    class Transform
        : public VLTEngine::Entity::Component
    {
    public:
        explicit Transform(
            VLTEngine::Entity::Entity *entity = nullptr);

        ~Transform() override = default;

        Transform(const Transform &) = default;
        Transform &operator=(const Transform &) = default;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 getModelMatrix() const;
    };

}