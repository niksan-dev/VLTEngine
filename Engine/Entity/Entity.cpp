#include <Entity/Entity.hpp>

namespace VLTEngine::Entity
{

    Entity::Entity(
        EntityId id,
        const std::string &name)
        : m_id(id),
          m_name(name),
          m_active(true)
    {
    }

    Entity::~Entity()
    {
        for (auto &[type, component] :
             m_components)
        {
            if (component)
            {
                component->onDestroy();
            }
        }

        m_components.clear();
    }

    EntityId Entity::getId() const
    {
        return m_id;
    }

    const std::string &Entity::getName() const
    {
        return m_name;
    }

    void Entity::setName(
        const std::string &name)
    {
        m_name = name;
    }

    bool Entity::isActive() const
    {
        return m_active;
    }

    void Entity::setActive(
        bool active)
    {
        m_active = active;
    }

}