#pragma once

#include <SDL2/SDL.h>

#include <vector>

namespace VLTEngine::Window
{

    struct WindowInfo
    {
        int displayId = -1;
        SDL_Window *window = nullptr;
    };

    class WindowManager
    {
    public:
        WindowManager();
        ~WindowManager();

        WindowManager(const WindowManager &) = delete;
        WindowManager &operator=(const WindowManager &) = delete;

        bool initialize(
            const char *title,
            const std::vector<SDL_Rect> &displayBounds);

        void shutdown();

        SDL_Window *getWindow(int displayId) const;

        const std::vector<WindowInfo> &getWindows() const;

        int getWindowCount() const;

    private:
        std::vector<WindowInfo> m_windows;
    };

} // namespace VLTEngine::Window