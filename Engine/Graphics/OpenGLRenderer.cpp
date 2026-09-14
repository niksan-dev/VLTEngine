#include <Graphics/OpenGLRenderer.hpp>

#include <Graphics/Mesh/MeshData.hpp>
#include <Graphics/Mesh/OpenGLMesh.hpp>
#include <Graphics/Shader/OpenGLShader.hpp>

#include <glad/glad.h>

#include <SDL2/SDL.h>

#include <iostream>
#include <memory>
#include <vector>

namespace VLTEngine::Graphics
{

    OpenGLRenderer::OpenGLRenderer()
        : m_initialized(false)
    {
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        shutdown();
    }

    bool OpenGLRenderer::initialize(
        const std::vector<SDL_Window *> &windows)
    {
        if (m_initialized)
            return true;

        if (windows.empty())
        {
            std::cerr
                << "OpenGLRenderer initialization failed: "
                << "no windows provided."
                << std::endl;

            return false;
        }

        m_windows = windows;

        /*
         * Configure the OpenGL context.
         *
         * VLTEngine currently targets OpenGL 3.3 Core.
         */
        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_MAJOR_VERSION,
            3);

        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_MINOR_VERSION,
            3);

        SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(
            SDL_GL_DOUBLEBUFFER,
            1);

        /*
         * Create one OpenGL context for each window/display.
         */
        for (std::size_t i = 0;
             i < m_windows.size();
             ++i)
        {
            SDL_Window *window = m_windows[i];

            if (window == nullptr)
            {
                std::cerr
                    << "OpenGLRenderer initialization failed: "
                    << "window "
                    << i
                    << " is null."
                    << std::endl;

                shutdown();
                return false;
            }

            SDL_GLContext context =
                SDL_GL_CreateContext(window);

            if (context == nullptr)
            {
                std::cerr
                    << "Failed to create OpenGL context for "
                    << "display/window "
                    << i
                    << ": "
                    << SDL_GetError()
                    << std::endl;

                shutdown();
                return false;
            }

            OpenGLContextInfo contextInfo;

            contextInfo.displayId =
                static_cast<int>(i);

            contextInfo.context =
                context;

            m_contexts.push_back(contextInfo);
        }

        /*
         * Make the first context current.
         */
        if (!makeCurrent(0))
        {
            shutdown();
            return false;
        }

        /*
         * Load OpenGL functions through GLAD.
         *
         * This project uses GLAD1-style generated loader.
         */
        if (!gladLoadGL())
        {
            std::cerr
                << "Failed to initialize GLAD."
                << std::endl;

            shutdown();
            return false;
        }

        std::cout
            << "OpenGL initialized."
            << std::endl;

        std::cout
            << "  Vendor:   "
            << reinterpret_cast<const char *>(
                   glGetString(GL_VENDOR))
            << std::endl;

        std::cout
            << "  Renderer: "
            << reinterpret_cast<const char *>(
                   glGetString(GL_RENDERER))
            << std::endl;

        std::cout
            << "  Version:  "
            << reinterpret_cast<const char *>(
                   glGetString(GL_VERSION))
            << std::endl;

        /*
         * Create a simple test triangle.
         *
         * Vertex layout:
         *
         *   position : 3 floats
         *   normal   : 3 floats
         *   texCoord : 2 floats
         */
        MeshData meshData;

        meshData.vertices =
            {
                {{0.0f, 0.65f, 0.0f},
                 {0.0f, 0.0f, 1.0f},
                 {0.0f, 0.0f}},

                {{-0.65f, -0.65f, 0.0f},
                 {0.0f, 0.0f, 1.0f},
                 {0.0f, 0.0f}},

                {{0.65f, -0.65f, 0.0f},
                 {0.0f, 0.0f, 1.0f},
                 {0.0f, 0.0f}}};

        meshData.indices =
            {
                0,
                1,
                2};

        m_mesh =
            std::make_unique<OpenGLMesh>(
                meshData);

        /*
         * Create shader.
         */
        m_shader =
            std::make_unique<OpenGLShader>();

        if (!m_shader->loadFromFiles(
                "Assets/Shaders/basic.vert",
                "Assets/Shaders/basic.frag"))
        {
            std::cerr
                << "Failed to load basic shader."
                << std::endl;

            shutdown();
            return false;
        }

        m_initialized = true;

        std::cout
            << "OpenGLRenderer initialized successfully."
            << std::endl;

        return true;
    }

    void OpenGLRenderer::shutdown()
    {
        /*
         * GPU resources must be destroyed while an OpenGL
         * context is still available.
         */
        if (!m_contexts.empty())
        {
            makeCurrent(0);
        }

        m_shader.reset();
        m_mesh.reset();

        /*
         * Destroy OpenGL contexts.
         */
        for (auto &contextInfo : m_contexts)
        {
            if (contextInfo.context != nullptr)
            {
                SDL_GL_DeleteContext(
                    contextInfo.context);

                contextInfo.context = nullptr;
            }
        }

        m_contexts.clear();
        m_windows.clear();

        m_initialized = false;
    }

    bool OpenGLRenderer::makeCurrent(
        int displayId)
    {
        if (displayId < 0 ||
            displayId >=
                static_cast<int>(m_contexts.size()))
        {
            std::cerr
                << "Invalid display ID: "
                << displayId
                << std::endl;

            return false;
        }

        if (displayId >=
            static_cast<int>(m_windows.size()))
        {
            std::cerr
                << "No window for display ID: "
                << displayId
                << std::endl;

            return false;
        }

        SDL_Window *window =
            m_windows[displayId];

        SDL_GLContext context =
            m_contexts[displayId].context;

        if (window == nullptr ||
            context == nullptr)
        {
            std::cerr
                << "Invalid window/context for display ID: "
                << displayId
                << std::endl;

            return false;
        }

        if (SDL_GL_MakeCurrent(
                window,
                context) != 0)
        {
            std::cerr
                << "Failed to make OpenGL context current "
                << "for display "
                << displayId
                << ": "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        return true;
    }

    void OpenGLRenderer::render()
    {
        if (!m_initialized)
            return;

        for (std::size_t i = 0;
             i < m_windows.size();
             ++i)
        {
            if (!makeCurrent(
                    static_cast<int>(i)))
            {
                continue;
            }

            SDL_Window *window =
                m_windows[i];

            int width = 0;
            int height = 0;

            SDL_GetWindowSize(
                window,
                &width,
                &height);

            glViewport(
                0,
                0,
                width,
                height);

            glClearColor(
                0.08f,
                0.08f,
                0.10f,
                1.0f);

            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

            if (m_shader &&
                m_mesh)
            {
                m_shader->bind();

                m_shader->setVec4(
                    "uColor",
                    1.0f,
                    0.2f,
                    0.2f,
                    1.0f);

                m_mesh->draw();

                m_shader->unbind();
            }

            SDL_GL_SwapWindow(window);
        }
    }

}