#include <Scene/Scene.hpp>

#include <algorithm>

namespace VLTEngine::Scene
{

    Scene::Scene()
        : m_nextEntityId(1), m_activeCamera(nullptr)
    {
    }

    Scene::~Scene()
    {
        clear();
    }

    VLTEngine::Entity::Entity &Scene::createEntity(
        const std::string &name)
    {
        const auto id = m_nextEntityId++;

        auto entity =
            std::make_unique<
                VLTEngine::Entity::Entity>(
                id,
                name);

        auto *entityPtr =
            entity.get();

        m_entities.emplace(
            id,
            std::move(entity));

        m_entityList.push_back(
            entityPtr);

        return *entityPtr;
    }

    bool Scene::destroyEntity(
        VLTEngine::Entity::EntityId id)
    {
        auto iterator =
            m_entities.find(id);

        if (iterator == m_entities.end())
            return false;

        auto *entity =
            iterator->second.get();

        if (entity == m_activeCamera)
        {
            m_activeCamera = nullptr;
        }

        m_entityList.erase(
            std::remove(
                m_entityList.begin(),
                m_entityList.end(),
                entity),
            m_entityList.end());

        m_entities.erase(iterator);

        return true;
    }

    VLTEngine::Entity::Entity *Scene::getEntity(
        VLTEngine::Entity::EntityId id)
    {
        auto iterator =
            m_entities.find(id);

        if (iterator == m_entities.end())
            return nullptr;

        return iterator->second.get();
    }

    const VLTEngine::Entity::Entity *Scene::getEntity(
        VLTEngine::Entity::EntityId id) const
    {
        auto iterator =
            m_entities.find(id);

        if (iterator == m_entities.end())
            return nullptr;

        return iterator->second.get();
    }

    VLTEngine::Entity::Entity *Scene::getEntityByName(
        const std::string &name)
    {
        for (auto *entity : m_entityList)
        {
            if (entity == nullptr)
                continue;

            if (entity->getName() == name)
                return entity;
        }

        return nullptr;
    }

    const VLTEngine::Entity::Entity *Scene::getEntityByName(
        const std::string &name) const
    {
        for (const auto *entity : m_entityList)
        {
            if (entity == nullptr)
                continue;

            if (entity->getName() == name)
                return entity;
        }

        return nullptr;
    }

    const std::vector<
        VLTEngine::Entity::Entity *> &
    Scene::getEntities() const
    {
        return m_entityList;
    }

    std::size_t Scene::getEntityCount() const
    {
        return m_entityList.size();
    }

    void Scene::clear()
    {
        m_entities.clear();

        m_entityList.clear();

        m_activeCamera = nullptr;
    }

    void Scene::update(
        float deltaTime)
    {
        for (auto *entity : m_entityList)
        {
            if (entity == nullptr)
                continue;

            if (!entity->isActive())
                continue;

            entity->update(deltaTime);
        }
    }

    void Scene::setActiveCamera(
        VLTEngine::Entity::Entity *entity)
    {
        if (entity == nullptr)
        {
            m_activeCamera = nullptr;
            return;
        }

        auto iterator =
            m_entities.find(
                entity->getId());

        if (iterator == m_entities.end())
        {
            return;
        }

        if (iterator->second.get() != entity)
        {
            return;
        }

        m_activeCamera = entity;
    }

    VLTEngine::Entity::Entity *Scene::getActiveCamera()
    {
        return m_activeCamera;
    }

    const VLTEngine::Entity::Entity *Scene::getActiveCamera() const
    {
        return m_activeCamera;
    }

}