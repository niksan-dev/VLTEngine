#pragma once

#include <Entity/Entity.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace VLTEngine::Scene
{

    class Scene
    {
    public:
        Scene();

        ~Scene();

        Scene(const Scene &) = delete;
        Scene &operator=(const Scene &) = delete;

        VLTEngine::Entity::Entity &createEntity(
            const std::string &name = "Entity");

        bool destroyEntity(
            VLTEngine::Entity::EntityId id);

        VLTEngine::Entity::Entity *getEntity(
            VLTEngine::Entity::EntityId id);

        const VLTEngine::Entity::Entity *getEntity(
            VLTEngine::Entity::EntityId id) const;

        VLTEngine::Entity::Entity *getEntityByName(
            const std::string &name);

        const VLTEngine::Entity::Entity *getEntityByName(
            const std::string &name) const;

        const std::vector<
            VLTEngine::Entity::Entity *> &
        getEntities() const;

        std::size_t getEntityCount() const;

        void clear();

        void update(
            float deltaTime);

        void setActiveCamera(
            VLTEngine::Entity::Entity *entity);

        VLTEngine::Entity::Entity *getActiveCamera();

        const VLTEngine::Entity::Entity *getActiveCamera() const;

    private:
        VLTEngine::Entity::EntityId m_nextEntityId;

        std::unordered_map<
            VLTEngine::Entity::EntityId,
            std::unique_ptr<
                VLTEngine::Entity::Entity>>
            m_entities;

        std::vector<
            VLTEngine::Entity::Entity *>
            m_entityList;

        VLTEngine::Entity::Entity *m_activeCamera;
    };

}