#include <Graphics/OpenGLRenderer.hpp>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <iostream>

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
                << "OpenGLRenderer: No windows provided."
                << std::endl;

            return false;
        }

        std::cout
            << "Initializing OpenGLRenderer..."
            << std::endl;

        // ---------------------------------------------------------
        // Configure OpenGL 3.3 Core Profile
        // ---------------------------------------------------------

        if (SDL_GL_SetAttribute(
                SDL_GL_CONTEXT_MAJOR_VERSION,
                3) != 0)
        {
            std::cerr
                << "Failed to set OpenGL major version: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        if (SDL_GL_SetAttribute(
                SDL_GL_CONTEXT_MINOR_VERSION,
                3) != 0)
        {
            std::cerr
                << "Failed to set OpenGL minor version: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        if (SDL_GL_SetAttribute(
                SDL_GL_CONTEXT_PROFILE_MASK,
                SDL_GL_CONTEXT_PROFILE_CORE) != 0)
        {
            std::cerr
                << "Failed to set OpenGL core profile: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        if (SDL_GL_SetAttribute(
                SDL_GL_DOUBLEBUFFER,
                1) != 0)
        {
            std::cerr
                << "Failed to enable OpenGL double buffering: "
                << SDL_GetError()
                << std::endl;

            return false;
        }

        // ---------------------------------------------------------
        // Store windows
        // ---------------------------------------------------------

        m_windows = windows;

        // ---------------------------------------------------------
        // Create one OpenGL context per window
        // ---------------------------------------------------------

        for (size_t i = 0; i < m_windows.size(); ++i)
        {
            SDL_Window *window = m_windows[i];

            if (window == nullptr)
            {
                std::cerr
                    << "OpenGLRenderer: Invalid window at index "
                    << i
                    << "."
                    << std::endl;

                shutdown();
                return false;
            }

            SDL_GLContext context =
                SDL_GL_CreateContext(window);

            if (context == nullptr)
            {
                std::cerr
                    << "Failed to create OpenGL context for Display "
                    << i + 1
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

            std::cout
                << "OpenGL context created for Display "
                << i + 1
                << std::endl;
        }

        // ---------------------------------------------------------
        // Make first OpenGL context current
        // ---------------------------------------------------------

        if (!makeCurrent(0))
        {
            std::cerr
                << "Failed to make OpenGL context current."
                << std::endl;

            shutdown();
            return false;
        }

        // ---------------------------------------------------------
        // Initialize GLAD
        //
        // This project uses the generated GLAD1-style API:
        //
        //     int gladLoadGL(void);
        //
        // ---------------------------------------------------------

        if (!gladLoadGL())
        {
            std::cerr
                << "Failed to initialize GLAD."
                << std::endl;

            shutdown();
            return false;
        }

        std::cout
            << "GLAD initialized successfully!"
            << std::endl;

        // ---------------------------------------------------------
        // Query OpenGL information
        // ---------------------------------------------------------

        const GLubyte *vendor =
            glGetString(GL_VENDOR);

        const GLubyte *renderer =
            glGetString(GL_RENDERER);

        const GLubyte *version =
            glGetString(GL_VERSION);

        std::cout
            << "OpenGL Vendor: "
            << (vendor != nullptr
                    ? reinterpret_cast<const char *>(vendor)
                    : "Unknown")
            << std::endl;

        std::cout
            << "OpenGL Renderer: "
            << (renderer != nullptr
                    ? reinterpret_cast<const char *>(renderer)
                    : "Unknown")
            << std::endl;

        std::cout
            << "OpenGL Version: "
            << (version != nullptr
                    ? reinterpret_cast<const char *>(version)
                    : "Unknown")
            << std::endl;

        // ---------------------------------------------------------
        // Default OpenGL state
        // ---------------------------------------------------------

        glClearColor(
            0.05f,
            0.05f,
            0.08f,
            1.0f);

        // ---------------------------------------------------------
        // Triangle geometry
        // ---------------------------------------------------------

        const float vertices[] =
            {
                0.0f, 0.5f, 0.0f,
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f};

        // ---------------------------------------------------------
        // Create Vertex Array
        // ---------------------------------------------------------

        m_vertexArray =
            std::make_unique<OpenGLVertexArray>();

        m_vertexArray->bind();

        // ---------------------------------------------------------
        // Create Vertex Buffer
        // ---------------------------------------------------------

        m_vertexBuffer =
            std::make_unique<OpenGLVertexBuffer>(
                vertices,
                sizeof(vertices));

        m_vertexBuffer->bind();

        // ---------------------------------------------------------
        // Vertex position layout
        // ---------------------------------------------------------

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            nullptr);

        // ---------------------------------------------------------
        // Create shader
        // ---------------------------------------------------------

        m_shader =
            std::make_unique<OpenGLShader>();

        if (!m_shader->loadFromFiles(
                "Assets/Shaders/basic.vert",
                "Assets/Shaders/basic.frag"))
        {
            std::cerr
                << "Failed to load basic shader."
                << std::endl;

            m_shader.reset();
            m_vertexBuffer.reset();
            m_vertexArray.reset();

            shutdown();

            return false;
        }

        std::cout
            << "Triangle rendering resources initialized!"
            << std::endl;

        // ---------------------------------------------------------
        // Renderer initialized
        // ---------------------------------------------------------

        m_initialized = true;

        std::cout
            << "OpenGLRenderer initialized successfully!"
            << std::endl;

        return true;
    }

    void OpenGLRenderer::shutdown()
    {
        if (!m_contexts.empty())
        {
            std::cout
                << "Shutting down OpenGLRenderer..."
                << std::endl;
        }

        // ---------------------------------------------------------
        // Destroy rendering resources
        // ---------------------------------------------------------

        m_shader.reset();
        m_vertexBuffer.reset();
        m_vertexArray.reset();

        // ---------------------------------------------------------
        // Destroy OpenGL contexts
        // ---------------------------------------------------------

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

        // ---------------------------------------------------------
        // Release window references
        //
        // WindowManager owns the SDL_Window objects.
        // OpenGLRenderer does NOT destroy them.
        // ---------------------------------------------------------

        m_windows.clear();

        m_initialized = false;
    }

    bool OpenGLRenderer::makeCurrent(int displayId)
    {
        if (displayId < 0 ||
            displayId >= static_cast<int>(m_contexts.size()))
        {
            std::cerr
                << "Invalid OpenGL display ID: "
                << displayId
                << std::endl;

            return false;
        }

        if (displayId >=
            static_cast<int>(m_windows.size()))
        {
            std::cerr
                << "No window exists for Display "
                << displayId + 1
                << std::endl;

            return false;
        }

        SDL_Window *window =
            m_windows[displayId];

        SDL_GLContext context =
            m_contexts[displayId].context;

        if (window == nullptr)
        {
            std::cerr
                << "Invalid window for Display "
                << displayId + 1
                << std::endl;

            return false;
        }

        if (context == nullptr)
        {
            std::cerr
                << "Invalid OpenGL context for Display "
                << displayId + 1
                << std::endl;

            return false;
        }

        if (SDL_GL_MakeCurrent(
                window,
                context) != 0)
        {
            std::cerr
                << "Failed to make OpenGL context current for Display "
                << displayId + 1
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

        // ---------------------------------------------------------
        // Render every display
        // ---------------------------------------------------------

        for (size_t i = 0; i < m_windows.size(); ++i)
        {
            const int displayId =
                static_cast<int>(i);

            // -----------------------------------------------------
            // Make display context current
            // -----------------------------------------------------

            if (!makeCurrent(displayId))
                continue;

            SDL_Window *window =
                m_windows[i];

            // -----------------------------------------------------
            // Get window size
            // -----------------------------------------------------

            int width = 0;
            int height = 0;

            SDL_GetWindowSize(
                window,
                &width,
                &height);

            // -----------------------------------------------------
            // Set viewport
            // -----------------------------------------------------

            glViewport(
                0,
                0,
                width,
                height);

            // -----------------------------------------------------
            // Clear framebuffer
            // -----------------------------------------------------

            glClear(
                GL_COLOR_BUFFER_BIT);

            // -----------------------------------------------------
            // Draw triangle
            // -----------------------------------------------------

            if (m_shader &&
                m_vertexArray)
            {
                m_shader->bind();

                m_vertexArray->bind();

                glDrawArrays(
                    GL_TRIANGLES,
                    0,
                    3);

                m_vertexArray->unbind();

                m_shader->unbind();
            }

            SDL_GL_SwapWindow(window);
        }
    }

} // namespace VLTEngine::Graphics