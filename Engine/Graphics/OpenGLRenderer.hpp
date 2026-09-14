#pragma once

#include <Graphics/Renderer.hpp>

#include <SDL2/SDL.h>

#include <vector>

namespace VLTEngine::Graphics
{

    struct OpenGLContextInfo
    {
        int displayId = -1;
        SDL_GLContext context = nullptr;
    };

    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override;

        OpenGLRenderer(const OpenGLRenderer &) = delete;
        OpenGLRenderer &operator=(const OpenGLRenderer &) = delete;

        bool initialize(
            const std::vector<SDL_Window *> &windows) override;

        void shutdown() override;

        void render() override;

        bool makeCurrent(int displayId);

    private:
        std::vector<OpenGLContextInfo> m_contexts;
        std::vector<SDL_Window *> m_windows;

        bool m_initialized;
    };

} // namespace VLTEngine::Graphics