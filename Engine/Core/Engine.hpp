#pragma once

namespace VLTEngine::Core
{

    class Engine
    {
    public:
        Engine();
        ~Engine();

        Engine(const Engine &) = delete;
        Engine &operator=(const Engine &) = delete;

        bool initialize();
        void run();
        void shutdown();

    private:
        bool m_initialized;
    };

} // namespace VLTEngine::Core