#include <Graphics/OpenGLRenderer.hpp>

#include <Components/MeshRenderer.hpp>
#include <Components/Transform.hpp>

#include <Graphics/Mesh/MeshData.hpp>
#include <Graphics/Mesh/OpenGLMesh.hpp>
#include <Graphics/Shader/OpenGLShader.hpp>
#include <Graphics/Material/OpenGLMaterial.hpp>

#include <glad/glad.h>

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

        // =========================================================
        // OpenGL Context Configuration
        // =========================================================

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

        // =========================================================
        // Create OpenGL Contexts
        // =========================================================

        for (std::size_t i = 0;
             i < m_windows.size();
             ++i)
        {
            SDL_Window *window =
                m_windows[i];

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

            m_contexts.push_back(
                contextInfo);
        }

        // =========================================================
        // Make First Context Current
        // =========================================================

        if (!makeCurrent(0))
        {
            shutdown();
            return false;
        }

        // =========================================================
        // Initialize GLAD
        // =========================================================

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

        // =========================================================
        // Test Mesh
        // =========================================================

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

        // =========================================================
        // Shader
        // =========================================================

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

        // =========================================================
        // Material
        // =========================================================

        m_material =
            std::make_unique<OpenGLMaterial>(
                m_shader.get());

        m_material->setColor(
            1.0f,
            0.2f,
            0.2f,
            1.0f);

        // =========================================================
        // Scene
        // =========================================================

        m_scene =
            std::make_unique<
                VLTEngine::Scene::Scene>();

        // =========================================================
        // Triangle Entity
        // =========================================================

        auto &triangle =
            m_scene->createEntity(
                "Triangle");

        // ---------------------------------------------------------
        // Transform Component
        // ---------------------------------------------------------

        auto &transform =
            triangle.addComponent<
                VLTEngine::Components::Transform>();

        transform.position =
            glm::vec3(
                0.20f,
                0.0f,
                0.0f);

        transform.rotation.z =
            glm::radians(10.0f);

        transform.scale =
            glm::vec3(
                0.80f,
                0.80f,
                0.80f);

        // ---------------------------------------------------------
        // MeshRenderer Component
        // ---------------------------------------------------------

        auto &meshRenderer =
            triangle.addComponent<
                VLTEngine::Components::MeshRenderer>();

        meshRenderer.setMesh(
            m_mesh.get());

        meshRenderer.setMaterial(
            m_material.get());

        // =========================================================
        // Renderer Initialized
        // =========================================================

        m_initialized = true;

        std::cout
            << "OpenGLRenderer initialized successfully."
            << std::endl;

        return true;
    }

    void OpenGLRenderer::shutdown()
    {
        if (!m_contexts.empty())
        {
            makeCurrent(0);
        }

        // =========================================================
        // Destroy Scene
        //
        // MeshRenderer contains non-owning references to
        // Mesh and Material, so destroy Scene first.
        // =========================================================

        m_scene.reset();

        // =========================================================
        // Destroy GPU Resources
        // =========================================================

        m_material.reset();

        m_shader.reset();

        m_mesh.reset();

        // =========================================================
        // Destroy OpenGL Contexts
        // =========================================================

        for (auto &contextInfo :
             m_contexts)
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
                static_cast<int>(
                    m_contexts.size()))
        {
            std::cerr
                << "Invalid display ID: "
                << displayId
                << std::endl;

            return false;
        }

        if (displayId >=
            static_cast<int>(
                m_windows.size()))
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

        // =========================================================
        // Render Every Display
        // =========================================================

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

            // =====================================================
            // Viewport
            // =====================================================

            glViewport(
                0,
                0,
                width,
                height);

            // =====================================================
            // Clear
            // =====================================================

            glClearColor(
                0.08f,
                0.08f,
                0.10f,
                1.0f);

            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT);

            // =====================================================
            // Scene Rendering
            // =====================================================

            if (m_scene &&
                m_shader)
            {
                // -------------------------------------------------
                // Camera Projection
                // -------------------------------------------------

                const float aspectRatio =
                    height > 0
                        ? static_cast<float>(width) /
                              static_cast<float>(height)
                        : 1.0f;

                m_camera.setPerspective(
                    glm::radians(60.0f),
                    aspectRatio,
                    0.1f,
                    100.0f);

                const glm::mat4 viewMatrix =
                    m_camera.getViewMatrix();

                const glm::mat4 projectionMatrix =
                    m_camera.getProjectionMatrix();

                // -------------------------------------------------
                // Iterate Scene Entities
                // -------------------------------------------------

                for (auto *entity :
                     m_scene->getEntities())
                {
                    if (entity == nullptr)
                        continue;

                    if (!entity->isActive())
                        continue;

                    // ---------------------------------------------
                    // Transform
                    // ---------------------------------------------

                    auto *transform =
                        entity->getComponent<
                            VLTEngine::Components::Transform>();

                    // ---------------------------------------------
                    // MeshRenderer
                    // ---------------------------------------------

                    auto *meshRenderer =
                        entity->getComponent<
                            VLTEngine::Components::MeshRenderer>();

                    if (transform == nullptr ||
                        meshRenderer == nullptr)
                    {
                        continue;
                    }

                    if (!meshRenderer->isRenderable())
                        continue;

                    // ---------------------------------------------
                    // Material
                    // ---------------------------------------------

                    auto *material =
                        meshRenderer->getMaterial();

                    if (material == nullptr)
                        continue;

                    // ---------------------------------------------
                    // Bind Material
                    // ---------------------------------------------

                    material->bind();

                    // ---------------------------------------------
                    // Model Matrix
                    // ---------------------------------------------

                    const glm::mat4 modelMatrix =
                        transform->getModelMatrix();

                    m_shader->setMat4(
                        "uModel",
                        glm::value_ptr(
                            modelMatrix));

                    // ---------------------------------------------
                    // View Matrix
                    // ---------------------------------------------

                    m_shader->setMat4(
                        "uView",
                        glm::value_ptr(
                            viewMatrix));

                    // ---------------------------------------------
                    // Projection Matrix
                    // ---------------------------------------------

                    m_shader->setMat4(
                        "uProjection",
                        glm::value_ptr(
                            projectionMatrix));

                    // ---------------------------------------------
                    // Draw Mesh
                    // ---------------------------------------------

                    auto *mesh =
                        meshRenderer->getMesh();

                    if (mesh != nullptr)
                    {
                        mesh->draw();
                    }

                    // ---------------------------------------------
                    // Unbind Material
                    // ---------------------------------------------

                    material->unbind();
                }
            }

            // =====================================================
            // Present
            // =====================================================

            SDL_GL_SwapWindow(
                window);
        }
    }

}