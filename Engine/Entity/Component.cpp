#include <Entity/Component.hpp>

namespace VLTEngine::Entity
{

    Component::Component(Entity *entity)
        : m_entity(entity),
          m_enabled(true)
    {
    }


    Entity *Component::getEntity()
    {
        return m_entity;
    }

    const Entity *Component::getEntity() const
    {
        return m_entity;
    }

    bool Component::isEnabled() const
    {
        return m_enabled;
    }

    void Component::setEnabled(bool enabled)
    {
        m_enabled = enabled;
    }

}