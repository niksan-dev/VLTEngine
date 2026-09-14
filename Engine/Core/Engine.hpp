#pragma once

#include <Display/DisplayManager.hpp>
#include <Window/WindowManager.hpp>
#include <Graphics/Renderer.hpp>

#include <memory>

namespace VLTEngine::Core
{

    class Engine
    {
    public:
        Engine();
        ~Engine();

        Engine(const Engine &) = delete;
        Engine &operator=(const Engine &) = delete;

        bool initialize();
        void run();
        void shutdown();

    private:
        bool m_initialized;

        VLTEngine::Display::DisplayManager m_displayManager;
        VLTEngine::Window::WindowManager m_windowManager;

        std::unique_ptr<VLTEngine::Graphics::Renderer> m_renderer;
    };

} // namespace VLTEngine::Core