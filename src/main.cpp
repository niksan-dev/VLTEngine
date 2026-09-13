#include <Core/Engine.hpp>

int main()
{
    VLTEngine::Core::Engine engine;

    if (!engine.initialize())
        return 1;

    engine.run();

    engine.shutdown();

    return 0;
}