
#pragma once

struct SDL_Window;

namespace VLTEngine::Window
{

    class WindowManager
    {
    public:
        WindowManager();
        ~WindowManager();

        WindowManager(const WindowManager &) = delete;
        WindowManager &operator=(const WindowManager &) = delete;

        bool initialize(
            const char *title,
            int width,
            int height);

        void shutdown();

        SDL_Window *getWindow() const;

    private:
        SDL_Window *m_window;
    };

} // namespace VLTEngine::Window