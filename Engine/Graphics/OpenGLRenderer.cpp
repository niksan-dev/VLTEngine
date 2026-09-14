#include <Graphics/OpenGLRenderer.hpp>

#include <Graphics/Mesh/MeshData.hpp>
#include <Graphics/Mesh/OpenGLMesh.hpp>
#include <Graphics/Shader/OpenGLShader.hpp>
#include <Graphics/Material/OpenGLMaterial.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
         * Configure OpenGL 3.3 Core.
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
         * Create one OpenGL context for every window/display.
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
         * Load OpenGL functions.
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
         * Create test triangle.
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

        /*
         * Create material using the shader.
         *
         * Material does not own the shader.
         */
        m_material =
            std::make_unique<OpenGLMaterial>(
                m_shader.get());

        /*
         * Set the material color.
         *
         * This is now the material's responsibility.
         */
        m_material->setColor(
            1.0f,
            0.2f,
            0.2f,
            1.0f);

        m_transform.position =
            glm::vec3(
                0.20f,
                0.0f,
                0.0f);

        m_transform.rotation.z =
            glm::radians(10.0f);

        m_transform.scale =
            glm::vec3(
                0.80f,
                0.80f,
                0.80f);

        m_initialized = true;

        std::cout
            << "OpenGLRenderer initialized successfully."
            << std::endl;

        return true;
    }

    void OpenGLRenderer::shutdown()
    {
        /*
         * GPU resources must be destroyed while an
         * OpenGL context is still available.
         */
        if (!m_contexts.empty())
        {
            makeCurrent(0);
        }

        /*
         * Material must be destroyed before the shader
         * it references.
         */
        m_material.reset();

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

            if (m_material &&
                m_mesh &&
                m_shader)
            {
                m_material->bind();

                glm::mat4 modelMatrix =
                    m_transform.getModelMatrix();

                m_shader->setMat4(
                    "uModel",
                    glm::value_ptr(modelMatrix));

                m_mesh->draw();

                m_material->unbind();
            }

            SDL_GL_SwapWindow(window);
        }
    }

}