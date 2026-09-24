#pragma once

#include <Scene/Scene.hpp>

namespace VLTEngine::Runtime
{

    class Runtime
    {
    public:
        Runtime();
        ~Runtime();

        Runtime(const Runtime &) = delete;
        Runtime &operator=(const Runtime &) = delete;

        bool initialize(
            VLTEngine::Scene::Scene &scene);

        void shutdown();

    private:
        bool m_initialized;
    };

}