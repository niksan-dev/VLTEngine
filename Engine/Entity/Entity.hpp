#pragma once

#include <Entity/Component.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace VLTEngine::Entity
{

    using EntityId = std::uint64_t;

    class Entity
    {
    public:
        Entity(
            EntityId id,
            const std::string &name = "Entity");

        ~Entity();

        Entity(const Entity &) = delete;
        Entity &operator=(const Entity &) = delete;

        // =========================================================
        // Identity
        // =========================================================

        EntityId getId() const;

        const std::string &getName() const;

        void setName(
            const std::string &name);

        // =========================================================
        // Active State
        // =========================================================

        bool isActive() const;

        void setActive(
            bool active);

        // =========================================================
        // Component Management
        // =========================================================

        template <typename T, typename... Args>
        T &addComponent(
            Args &&...args)
        {
            static_assert(
                std::is_base_of_v<
                    Component,
                    T>,
                "T must derive from Component.");

            const std::type_index type =
                std::type_index(
                    typeid(T));

            auto existing =
                m_components.find(type);

            if (existing != m_components.end())
            {
                return *static_cast<T *>(
                    existing->second.get());
            }

            auto component =
                std::make_unique<T>(
                    this,
                    std::forward<Args>(args)...);

            T *componentPtr =
                component.get();

            m_components.emplace(
                type,
                std::move(component));

            componentPtr->onCreate();

            return *componentPtr;
        }

        template <typename T>
        T *getComponent()
        {
            static_assert(
                std::is_base_of_v<
                    Component,
                    T>,
                "T must derive from Component.");

            const std::type_index type =
                std::type_index(
                    typeid(T));

            auto iterator =
                m_components.find(type);

            if (iterator == m_components.end())
                return nullptr;

            return static_cast<T *>(
                iterator->second.get());
        }

        template <typename T>
        const T *getComponent() const
        {
            static_assert(
                std::is_base_of_v<
                    Component,
                    T>,
                "T must derive from Component.");

            const std::type_index type =
                std::type_index(
                    typeid(T));

            auto iterator =
                m_components.find(type);

            if (iterator == m_components.end())
                return nullptr;

            return static_cast<const T *>(
                iterator->second.get());
        }

        template <typename T>
        bool hasComponent() const
        {
            static_assert(
                std::is_base_of_v<
                    Component,
                    T>,
                "T must derive from Component.");

            return m_components.find(
                       std::type_index(typeid(T))) != m_components.end();
        }

        template <typename T>
        bool removeComponent()
        {
            static_assert(
                std::is_base_of_v<
                    Component,
                    T>,
                "T must derive from Component.");

            const std::type_index type =
                std::type_index(
                    typeid(T));

            auto iterator =
                m_components.find(type);

            if (iterator == m_components.end())
                return false;

            iterator->second->onDestroy();

            m_components.erase(iterator);

            return true;
        }

        // =========================================================
        // Runtime Update
        // =========================================================

        void update(
            float deltaTime);

    private:
        EntityId m_id;

        std::string m_name;

        bool m_active;

        std::unordered_map<
            std::type_index,
            std::unique_ptr<Component>>
            m_components;
    };

}