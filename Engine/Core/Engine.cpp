#include <Core/Engine.hpp>

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

        std::cout << "VLTEngine initializing..." << std::endl;

        // Engine initialization will be expanded here:
        //
        // 1. Platform
        // 2. SDL
        // 3. Window
        // 4. Display
        // 5. Graphics
        // 6. Input
        // 7. Audio
        // 8. Resource systems

        m_initialized = true;

        std::cout << "VLTEngine initialized successfully!" << std::endl;

        return true;
    }

    void Engine::run()
    {
        if (!m_initialized)
            return;

        std::cout << "VLTEngine running..." << std::endl;

        // Main engine loop will be implemented here.
    }

    void Engine::shutdown()
    {
        if (!m_initialized)
            return;

        std::cout << "VLTEngine shutting down..." << std::endl;

        // Shutdown systems in reverse initialization order.

        m_initialized = false;
    }

} // namespace VLTEngine::Core