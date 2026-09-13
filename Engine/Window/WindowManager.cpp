
#include <Window/WindowManager.hpp>

#include <SDL2/SDL.h>
#include <iostream>

namespace VLTEngine::Window
{

    WindowManager::WindowManager()
        : m_window(nullptr)
    {
    }

    WindowManager::~WindowManager()
    {
        shutdown();
    }

    bool WindowManager::initialize(
        const char *title,
        int width,
        int height)
    {
        if (m_window != nullptr)
            return true;

        m_window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN);

        if (m_window == nullptr)
        {
            std::cerr
                << "Failed to create window: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        std::cout << "Window created successfully!"
                  << std::endl;

        return true;
    }

    void WindowManager::shutdown()
    {
        if (m_window == nullptr)
            return;

        SDL_DestroyWindow(m_window);
        m_window = nullptr;

        std::cout << "Window destroyed."
                  << std::endl;
    }

    SDL_Window *WindowManager::getWindow() const
    {
        return m_window;
    }

} // namespace VLTEngine::Window