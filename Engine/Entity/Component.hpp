#pragma once

namespace VLTEngine::Entity
{

    class Entity;

    class Component
    {
    public:
        virtual ~Component() = default;

        Component(const Component &) = delete;
        Component &operator=(const Component &) = delete;

        Entity *getEntity();

        const Entity *getEntity() const;

        bool isEnabled() const;

        void setEnabled(
            bool enabled);

    protected:
        explicit Component(
            Entity *entity);

        virtual void onCreate()
        {
        }

        virtual void onUpdate(
            float deltaTime)
        {
            (void)deltaTime;
        }

        virtual void onDestroy()
        {
        }

    private:
        Entity *m_entity;

        bool m_enabled;

        friend class Entity;
    };

}