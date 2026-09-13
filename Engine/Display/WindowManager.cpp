#include <Display/DisplayManager.hpp>

#include <SDL2/SDL.h>

#include <iostream>

namespace VLTEngine::Display
{

    DisplayManager::DisplayManager()
        : m_initialized(false)
    {
    }

    DisplayManager::~DisplayManager()
    {
        shutdown();
    }

    bool DisplayManager::initialize()
    {
        if (m_initialized)
            return true;

        std::cout << "Initializing DisplayManager..."
                  << std::endl;

        if (!refresh())
        {
            std::cerr
                << "DisplayManager failed to detect displays."
                << std::endl;

            return false;
        }

        m_initialized = true;

        std::cout
            << "DisplayManager initialized successfully!"
            << std::endl;

        return true;
    }

    void DisplayManager::shutdown()
    {
        if (!m_initialized)
            return;

        std::cout
            << "Shutting down DisplayManager..."
            << std::endl;

        m_displays.clear();

        m_initialized = false;
    }

    bool DisplayManager::refresh()
    {
        m_displays.clear();

        const int displayCount = SDL_GetNumVideoDisplays();

        if (displayCount < 1)
        {
            std::cerr
                << "No displays detected: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        for (int i = 0; i < displayCount; ++i)
        {
            DisplayInfo info;

            info.id = i;

            const char *displayName = SDL_GetDisplayName(i);

            if (displayName != nullptr)
                info.name = displayName;
            else
                info.name = "Display " + std::to_string(i + 1);

            // -------------------------------------------------
            // Display bounds
            // -------------------------------------------------

            SDL_Rect bounds{};

            if (SDL_GetDisplayBounds(i, &bounds) == 0)
            {
                info.x = bounds.x;
                info.y = bounds.y;

                info.width = bounds.w;
                info.height = bounds.h;
            }

            // -------------------------------------------------
            // Usable display area
            // -------------------------------------------------

            SDL_Rect usableBounds{};

            if (SDL_GetDisplayUsableBounds(i, &usableBounds) == 0)
            {
                info.usableX = usableBounds.x;
                info.usableY = usableBounds.y;

                info.usableWidth = usableBounds.w;
                info.usableHeight = usableBounds.h;
            }

            // -------------------------------------------------
            // Primary display
            //
            // SDL display 0 is normally the primary display.
            // We will refine primary-display detection later
            // when the platform layer is implemented.
            // -------------------------------------------------

            info.primary = (i == 0);

            m_displays.push_back(info);
        }

        return true;
    }

    void DisplayManager::handleEvent(const void *event)
    {
        if (event == nullptr)
            return;

        const SDL_Event *sdlEvent =
            static_cast<const SDL_Event *>(event);

        if (sdlEvent->type != SDL_DISPLAYEVENT)
            return;

        switch (sdlEvent->display.event)
        {
        case SDL_DISPLAYEVENT_CONNECTED:
        {
            std::cout
                << "Display connected. Refreshing displays..."
                << std::endl;

            refresh();
            break;
        }

        case SDL_DISPLAYEVENT_DISCONNECTED:
        {
            std::cout
                << "Display disconnected. Refreshing displays..."
                << std::endl;

            refresh();
            break;
        }

        case SDL_DISPLAYEVENT_ORIENTATION:
        {
            std::cout
                << "Display orientation changed. Refreshing displays..."
                << std::endl;

            refresh();
            break;
        }

        case SDL_DISPLAYEVENT_MOVED:
        {
            std::cout
                << "Display position changed. Refreshing displays..."
                << std::endl;

            refresh();
            break;
        }

        default:
            break;
        }
    }

    int DisplayManager::getDisplayCount() const
    {
        return static_cast<int>(m_displays.size());
    }

    const std::vector<DisplayInfo> &
    DisplayManager::getDisplays() const
    {
        return m_displays;
    }

    const DisplayInfo *
    DisplayManager::getDisplay(int id) const
    {
        for (const auto &display : m_displays)
        {
            if (display.id == id)
                return &display;
        }

        return nullptr;
    }

    const DisplayInfo *
    DisplayManager::getPrimaryDisplay() const
    {
        for (const auto &display : m_displays)
        {
            if (display.primary)
                return &display;
        }

        return nullptr;
    }

} // namespace VLTEngine::Display