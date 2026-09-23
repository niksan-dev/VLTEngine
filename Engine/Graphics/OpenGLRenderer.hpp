#pragma once

#include <Scene/Scene.hpp>

#include <Graphics/Renderer.hpp>
#include <Graphics/Mesh/OpenGLMesh.hpp>
#include <Graphics/Material/OpenGLMaterial.hpp>
#include <Graphics/Shader/OpenGLShader.hpp>

#include <SDL2/SDL.h>

#include <memory>
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

        bool makeCurrent(
            int displayId);

    private:
        std::vector<OpenGLContextInfo> m_contexts;

        std::vector<SDL_Window *> m_windows;

        // ---------------------------------------------------------
        // Test / Runtime Graphics Resources
        // ---------------------------------------------------------

        std::unique_ptr<OpenGLMesh> m_mesh;

        std::unique_ptr<OpenGLShader> m_shader;

        std::unique_ptr<OpenGLMaterial> m_material;

        // ---------------------------------------------------------
        // Scene
        // ---------------------------------------------------------

        std::unique_ptr<
            VLTEngine::Scene::Scene>
            m_scene;

        bool m_initialized;
    };

}