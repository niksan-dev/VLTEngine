#pragma once

#include <Scene/Scene.hpp>

#include <SDL2/SDL.h>

#include <vector>

namespace VLTEngine::Graphics
{

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;

        virtual bool initialize(
            const std::vector<SDL_Window *> &windows,
            VLTEngine::Scene::Scene &scene) = 0;

        virtual void shutdown() = 0;

        virtual void render(
            VLTEngine::Scene::Scene &scene) = 0;

    protected:
        Renderer() = default;
    };

}