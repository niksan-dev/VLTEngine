#include <Scene/Scene.hpp>

namespace VLTEngine::Scene
{

    Scene::Scene()
        : m_nextEntityId(1)
    {
    }

    Scene::~Scene()
    {
        clear();
    }

    VLTEngine::Entity::Entity &
    Scene::createEntity(
        const std::string &name)
    {
        const auto entityId =
            m_nextEntityId++;

        auto entity =
            std::make_unique<
                VLTEngine::Entity::Entity>(
                entityId,
                name);

        auto *entityPtr =
            entity.get();

        m_entities.emplace(
            entityId,
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

        for (auto listIterator =
                 m_entityList.begin();
             listIterator !=
             m_entityList.end();
             ++listIterator)
        {
            if (*listIterator == entity)
            {
                m_entityList.erase(
                    listIterator);

                break;
            }
        }

        m_entities.erase(iterator);

        return true;
    }

    VLTEngine::Entity::Entity *
    Scene::getEntity(
        VLTEngine::Entity::EntityId id)
    {
        auto iterator =
            m_entities.find(id);

        if (iterator == m_entities.end())
            return nullptr;

        return iterator->second.get();
    }

    const VLTEngine::Entity::Entity *
    Scene::getEntity(
        VLTEngine::Entity::EntityId id) const
    {
        auto iterator =
            m_entities.find(id);

        if (iterator == m_entities.end())
            return nullptr;

        return iterator->second.get();
    }

    const std::vector<
        VLTEngine::Entity::Entity *> &
    Scene::getEntities() const
    {
        return m_entityList;
    }

    std::size_t Scene::getEntityCount() const
    {
        return m_entities.size();
    }

    void Scene::clear()
    {
        m_entityList.clear();
        m_entities.clear();
    }

}