#include <Core/Engine.hpp>

#include <Graphics/OpenGLRenderer.hpp>

#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>
#include <vector>

namespace VLTEngine::Core
{

    Engine::Engine()
        : m_initialized(false)
    {
    }

    Engine::~Engine()
    {
        shutdown();
    }

    bool Engine::initialize()
    {
        if (m_initialized)
            return true;

        if (SDL_Init(
                SDL_INIT_VIDEO |
                SDL_INIT_AUDIO |
                SDL_INIT_EVENTS) != 0)
        {
            std::cerr
                << "SDL initialization failed: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        if (!m_displayManager.initialize())
        {
            std::cerr
                << "DisplayManager initialization failed."
                << std::endl;

            SDL_Quit();

            return false;
        }

        std::vector<SDL_Rect> displayBounds;

        for (const auto &display :
             m_displayManager.getDisplays())
        {
            SDL_Rect bounds;

            bounds.x = display.x;
            bounds.y = display.y;
            bounds.w = display.width;
            bounds.h = display.height;

            displayBounds.push_back(
                bounds);
        }

        if (!m_windowManager.initialize(
                "VLTEngine",
                displayBounds))
        {
            std::cerr
                << "WindowManager initialization failed."
                << std::endl;

            m_displayManager.shutdown();
            SDL_Quit();

            return false;
        }

        m_scene =
            std::make_unique<
                VLTEngine::Scene::Scene>();

        /*
         * Runtime
         *
         * Creates and configures scene content.
         */
        if (!m_runtime.initialize(
                *m_scene))
        {
            std::cerr
                << "Runtime initialization failed."
                << std::endl;

            m_scene.reset();
            m_windowManager.shutdown();
            m_displayManager.shutdown();
            SDL_Quit();

            return false;
        }

        std::vector<SDL_Window *> windows;

        for (const auto &windowInfo :
             m_windowManager.getWindows())
        {
            if (windowInfo.window != nullptr)
            {
                windows.push_back(
                    windowInfo.window);
            }
        }

        m_renderer =
            std::make_unique<
                VLTEngine::Graphics::OpenGLRenderer>();

        if (!m_renderer->initialize(
                windows,
                *m_scene))
        {
            std::cerr
                << "Renderer initialization failed."
                << std::endl;

            m_renderer.reset();

            m_runtime.shutdown();

            m_scene.reset();

            m_windowManager.shutdown();
            m_displayManager.shutdown();

            SDL_Quit();

            return false;
        }

        m_initialized = true;

        std::cout
            << "VLTEngine initialized successfully!"
            << std::endl;

        return true;
    }

    void Engine::run()
    {
        if (!m_initialized)
            return;

        bool running = true;

        using Clock =
            std::chrono::steady_clock;

        auto previousTime =
            Clock::now();

        while (running)
        {
            const auto currentTime =
                Clock::now();

            const std::chrono::duration<float>
                elapsed =
                    currentTime -
                    previousTime;

            const float deltaTime =
                elapsed.count();

            previousTime =
                currentTime;

            SDL_Event event;

            while (SDL_PollEvent(&event))
            {
                if (event.type ==
                    SDL_QUIT)
                {
                    running = false;
                }

                if (event.type ==
                    SDL_DISPLAYEVENT)
                {
                    m_displayManager.handleEvent(
                        &event);
                }
            }

            if (m_scene)
            {
                m_scene->update(
                    deltaTime);
            }

            if (m_renderer)
            {
                m_renderer->render(
                    *m_scene);
            }
        }
    }

    void Engine::shutdown()
    {
        if (!m_initialized)
            return;

        if (m_renderer)
        {
            m_renderer->shutdown();

            m_renderer.reset();
        }

        m_runtime.shutdown();

        m_scene.reset();

        m_windowManager.shutdown();

        m_displayManager.shutdown();

        SDL_Quit();

        m_initialized = false;
    }

}