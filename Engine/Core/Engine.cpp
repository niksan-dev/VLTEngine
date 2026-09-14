#include <Core/Engine.hpp>
#include <Graphics/OpenGLRenderer.hpp>

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>
#include <vector>

namespace VLTEngine::Core
{

    Engine::Engine()
        : m_initialized(false),
          m_renderer(nullptr)
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
            {
                std::cout
                    << " | PRIMARY";
            }

            std::cout << std::endl;
        }

        // -------------------------------------------------
        // Prepare window bounds
        // -------------------------------------------------

        std::vector<SDL_Rect> displayBounds;

        for (const auto &display :
             m_displayManager.getDisplays())
        {
            SDL_Rect bounds{};

            bounds.x = display.x;
            bounds.y = display.y;
            bounds.w = display.width;
            bounds.h = display.height;

            displayBounds.push_back(bounds);
        }

        // -------------------------------------------------
        // Create one window per display
        // -------------------------------------------------

        if (!m_windowManager.initialize(
                "VLTEngine",
                displayBounds))
        {
            m_displayManager.shutdown();
            SDL_Quit();

            return false;
        }

        // -------------------------------------------------
        // Collect SDL windows
        // -------------------------------------------------

        std::vector<SDL_Window *> windows;

        for (const auto &windowInfo :
             m_windowManager.getWindows())
        {
            windows.push_back(
                windowInfo.window);
        }

        // -------------------------------------------------
        // Create OpenGL renderer
        // -------------------------------------------------

        m_renderer =
            std::make_unique<
                VLTEngine::Graphics::OpenGLRenderer>();

        // -------------------------------------------------
        // Initialize renderer
        // -------------------------------------------------

        if (!m_renderer->initialize(windows))
        {
            m_renderer.reset();

            m_windowManager.shutdown();
            m_displayManager.shutdown();
            SDL_Quit();

            return false;
        }

        // -------------------------------------------------
        // Engine initialized
        // -------------------------------------------------

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
            // -------------------------------------------------
            // Process Events
            // -------------------------------------------------

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }

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

            if (m_renderer)
            {
                m_renderer->render();
            }
        }
    }

    void Engine::shutdown()
    {
        if (!m_initialized)
            return;

        std::cout
            << "VLTEngine shutting down..."
            << std::endl;

        // -------------------------------------------------
        // Reverse initialization order
        // -------------------------------------------------

        if (m_renderer)
        {
            m_renderer->shutdown();
            m_renderer.reset();
        }

        m_windowManager.shutdown();

        m_displayManager.shutdown();

        SDL_Quit();

        m_initialized = false;
    }

} // namespace VLTEngine::Core