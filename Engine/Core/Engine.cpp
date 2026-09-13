#include <Core/Engine.hpp>

#include <SDL2/SDL.h>
#include <iostream>

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

        std::cout << "VLTEngine initializing..."
                  << std::endl;

        // -------------------------------------------------
        // Initialize SDL
        // -------------------------------------------------

        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            std::cerr
                << "SDL initialization failed: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        // -------------------------------------------------
        // Create Window
        // -------------------------------------------------

        if (!m_windowManager.initialize(
                "VLTEngine",
                1280,
                720))
        {
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

        SDL_Event event;

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

            // Update
            // Render
        }
    }

    void Engine::shutdown()
    {
        if (!m_initialized)
            return;

        std::cout
            << "VLTEngine shutting down..."
            << std::endl;

        m_windowManager.shutdown();

        SDL_Quit();

        m_initialized = false;
    }

} // namespace VLTEngine::Core