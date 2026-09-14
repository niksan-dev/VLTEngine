#pragma once

#include <glm/glm.hpp>

namespace VLTEngine::Components
{

    class Transform
    {
    public:
        Transform();
        ~Transform() = default;

        Transform(const Transform &) = default;
        Transform &operator=(const Transform &) = default;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 getModelMatrix() const;
    };

}