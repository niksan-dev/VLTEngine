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

        std::cout
            << "VLTEngine initializing..."
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
        // Initialize Display Manager
        // -------------------------------------------------

        if (!m_displayManager.initialize())
        {
            SDL_Quit();
            return false;
        }

        // -------------------------------------------------
        // Print detected displays
        // -------------------------------------------------

        std::cout
            << "Detected displays: "
            << m_displayManager.getDisplayCount()
            << std::endl;

        for (const auto &display :
             m_displayManager.getDisplays())
        {
            std::cout
                << "  Display "
                << display.id + 1
                << ": "
                << display.name
                << " | "
                << display.width
                << "x"
                << display.height
                << " | Position: ("
                << display.x
                << ", "
                << display.y
                << ")";

            if (display.primary)
                std::cout << " | PRIMARY";

            std::cout << std::endl;
        }

        // -------------------------------------------------
        // Create Window
        // -------------------------------------------------

        if (!m_windowManager.initialize(
                "VLTEngine",
                1280,
                720))
        {
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

        SDL_Event event{};

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                // -------------------------------------------------
                // Application quit
                // -------------------------------------------------

                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

                // -------------------------------------------------
                // Display events
                // -------------------------------------------------

                m_displayManager.handleEvent(&event);
            }

            // -------------------------------------------------
            // Update
            // -------------------------------------------------

            // Future:
            // Scene update
            // Entity update
            // Physics
            // Animation
            // Game logic

            // -------------------------------------------------
            // Render
            // -------------------------------------------------

            // Future:
            // Renderer
            // Render layers
            // Display routing
        }
    }

    void Engine::shutdown()
    {
        if (!m_initialized)
            return;

        std::cout
            << "VLTEngine shutting down..."
            << std::endl;

        // Reverse initialization order

        m_windowManager.shutdown();

        m_displayManager.shutdown();

        SDL_Quit();

        m_initialized = false;
    }

} // namespace VLTEngine::Core