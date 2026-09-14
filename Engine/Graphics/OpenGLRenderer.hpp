#pragma once
#include <Graphics/Buffer/OpenGLVertexArray.hpp>
#include <Graphics/Buffer/OpenGLVertexBuffer.hpp>
#include <Graphics/Shader/OpenGLShader.hpp>

#include <memory>
#include <Graphics/Renderer.hpp>

#include <SDL2/SDL.h>

#include <vector>

namespace VLTEngine::Graphics
{

    struct OpenGLContextInfo
    {
        int displayId = -1;
        SDL_GLContext context = nullptr;
    };

    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override;

        OpenGLRenderer(const OpenGLRenderer &) = delete;
        OpenGLRenderer &operator=(const OpenGLRenderer &) = delete;

        bool initialize(
            const std::vector<SDL_Window *> &windows) override;

        void shutdown() override;

        void render() override;

        bool makeCurrent(int displayId);

    private:
        std::vector<OpenGLContextInfo> m_contexts;
        std::vector<SDL_Window *> m_windows;

        std::unique_ptr<OpenGLVertexArray> m_vertexArray;
        std::unique_ptr<OpenGLVertexBuffer> m_vertexBuffer;
        std::unique_ptr<OpenGLShader> m_shader;

        bool m_initialized;
    };

} // namespace VLTEngine::Graphics