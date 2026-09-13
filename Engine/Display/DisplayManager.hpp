#pragma once

#include <string>
#include <vector>

namespace VLTEngine::Display
{

    struct DisplayInfo
    {
        int id = -1;

        std::string name;

        int x = 0;
        int y = 0;

        int width = 0;
        int height = 0;

        int usableX = 0;
        int usableY = 0;

        int usableWidth = 0;
        int usableHeight = 0;

        bool primary = false;
    };

    class DisplayManager
    {
    public:
        DisplayManager();
        ~DisplayManager();

        DisplayManager(const DisplayManager &) = delete;
        DisplayManager &operator=(const DisplayManager &) = delete;

        bool initialize();
        void shutdown();

        bool refresh();

        void handleEvent(const void *event);

        int getDisplayCount() const;

        const std::vector<DisplayInfo> &getDisplays() const;

        const DisplayInfo *getDisplay(int id) const;

        const DisplayInfo *getPrimaryDisplay() const;

    private:
        std::vector<DisplayInfo> m_displays;
        bool m_initialized;
    };

} // namespace VLTEngine::Display