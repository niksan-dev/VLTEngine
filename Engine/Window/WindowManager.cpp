#include <Window/WindowManager.hpp>

#include <iostream>

namespace VLTEngine::Window
{

    WindowManager::WindowManager()
    {
    }

    WindowManager::~WindowManager()
    {
        shutdown();
    }

    bool WindowManager::initialize(
        const char *title,
        const std::vector<SDL_Rect> &displayBounds)
    {
        if (!m_windows.empty())
            return true;

        for (size_t i = 0; i < displayBounds.size(); ++i)
        {
            const SDL_Rect &bounds = displayBounds[i];

            SDL_Window *window = SDL_CreateWindow(
                title,
                bounds.x,
                bounds.y,
                bounds.w,
                bounds.h,
                SDL_WINDOW_SHOWN |
                    SDL_WINDOW_OPENGL);

            if (window == nullptr)
            {
                std::cerr
                    << "Failed to create window for Display "
                    << i + 1
                    << ": "
                    << SDL_GetError()
                    << std::endl;

                shutdown();

                return false;
            }

            WindowInfo info;

            info.displayId = static_cast<int>(i);
            info.window = window;

            m_windows.push_back(info);

            std::cout
                << "Window created for Display "
                << i + 1
                << " | Resolution: "
                << bounds.w
                << "x"
                << bounds.h
                << " | Position: ("
                << bounds.x
                << ", "
                << bounds.y
                << ")"
                << std::endl;
        }

        return true;
    }

    void WindowManager::shutdown()
    {
        for (auto &windowInfo : m_windows)
        {
            if (windowInfo.window != nullptr)
            {
                SDL_DestroyWindow(windowInfo.window);
                windowInfo.window = nullptr;
            }
        }

        m_windows.clear();
    }

    SDL_Window *
    WindowManager::getWindow(int displayId) const
    {
        for (const auto &windowInfo : m_windows)
        {
            if (windowInfo.displayId == displayId)
                return windowInfo.window;
        }

        return nullptr;
    }

    const std::vector<WindowInfo> &
    WindowManager::getWindows() const
    {
        return m_windows;
    }

    int WindowManager::getWindowCount() const
    {
        return static_cast<int>(m_windows.size());
    }

} // namespace VLTEngine::Window