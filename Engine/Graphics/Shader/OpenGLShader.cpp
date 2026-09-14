#include <Graphics/Shader/OpenGLShader.hpp>

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace VLTEngine::Graphics
{

    OpenGLShader::OpenGLShader()
        : m_programId(0)
    {
    }

    OpenGLShader::~OpenGLShader()
    {
        if (m_programId != 0)
        {
            glDeleteProgram(m_programId);

            m_programId = 0;
        }
    }

    unsigned int OpenGLShader::getProgramId() const
    {
        return m_programId;
    }

    bool OpenGLShader::loadFromSource(
        const std::string &vertexSource,
        const std::string &fragmentSource)
    {
        // ---------------------------------------------------------
        // Remove previously loaded shader
        // ---------------------------------------------------------

        if (m_programId != 0)
        {
            glDeleteProgram(m_programId);
            m_programId = 0;
        }

        // ---------------------------------------------------------
        // Compile vertex shader
        // ---------------------------------------------------------

        const unsigned int vertexShader =
            compileShader(
                GL_VERTEX_SHADER,
                vertexSource);

        if (vertexShader == 0)
        {
            return false;
        }

        // ---------------------------------------------------------
        // Compile fragment shader
        // ---------------------------------------------------------

        const unsigned int fragmentShader =
            compileShader(
                GL_FRAGMENT_SHADER,
                fragmentSource);

        if (fragmentShader == 0)
        {
            glDeleteShader(vertexShader);

            return false;
        }

        // ---------------------------------------------------------
        // Link shader program
        // ---------------------------------------------------------

        const bool linked =
            linkProgram(
                vertexShader,
                fragmentShader);

        // ---------------------------------------------------------
        // Individual shaders are no longer needed
        // after linking.
        // ---------------------------------------------------------

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return linked;
    }

    bool OpenGLShader::loadFromFiles(
        const std::string &vertexPath,
        const std::string &fragmentPath)
    {
        // ---------------------------------------------------------
        // Read vertex shader
        // ---------------------------------------------------------

        std::ifstream vertexFile(vertexPath);

        if (!vertexFile.is_open())
        {
            std::cerr
                << "Failed to open vertex shader: "
                << vertexPath
                << std::endl;

            return false;
        }

        std::stringstream vertexStream;

        vertexStream
            << vertexFile.rdbuf();

        vertexFile.close();

        // ---------------------------------------------------------
        // Read fragment shader
        // ---------------------------------------------------------

        std::ifstream fragmentFile(fragmentPath);

        if (!fragmentFile.is_open())
        {
            std::cerr
                << "Failed to open fragment shader: "
                << fragmentPath
                << std::endl;

            return false;
        }

        std::stringstream fragmentStream;

        fragmentStream
            << fragmentFile.rdbuf();

        fragmentFile.close();

        // ---------------------------------------------------------
        // Compile and link
        // ---------------------------------------------------------

        return loadFromSource(
            vertexStream.str(),
            fragmentStream.str());
    }

    void OpenGLShader::bind()
    {
        if (m_programId == 0)
            return;

        glUseProgram(m_programId);
    }

    void OpenGLShader::unbind()
    {
        glUseProgram(0);
    }

    unsigned int OpenGLShader::compileShader(
        unsigned int type,
        const std::string &source)
    {
        const unsigned int shader =
            glCreateShader(type);

        if (shader == 0)
        {
            std::cerr
                << "Failed to create OpenGL shader."
                << std::endl;

            return 0;
        }

        const char *sourceCode =
            source.c_str();

        glShaderSource(
            shader,
            1,
            &sourceCode,
            nullptr);

        glCompileShader(shader);

        // ---------------------------------------------------------
        // Check compilation status
        // ---------------------------------------------------------

        int success = 0;

        glGetShaderiv(
            shader,
            GL_COMPILE_STATUS,
            &success);

        if (!success)
        {
            int logLength = 0;

            glGetShaderiv(
                shader,
                GL_INFO_LOG_LENGTH,
                &logLength);

            std::string errorLog;

            if (logLength > 0)
            {
                errorLog.resize(
                    static_cast<size_t>(logLength));

                glGetShaderInfoLog(
                    shader,
                    logLength,
                    nullptr,
                    errorLog.data());
            }

            std::cerr
                << "OpenGL shader compilation failed:"
                << std::endl
                << errorLog
                << std::endl;

            glDeleteShader(shader);

            return 0;
        }

        return shader;
    }

    bool OpenGLShader::linkProgram(
        unsigned int vertexShader,
        unsigned int fragmentShader)
    {
        const unsigned int program =
            glCreateProgram();

        if (program == 0)
        {
            std::cerr
                << "Failed to create OpenGL shader program."
                << std::endl;

            return false;
        }

        glAttachShader(
            program,
            vertexShader);

        glAttachShader(
            program,
            fragmentShader);

        glLinkProgram(program);

        // ---------------------------------------------------------
        // Check linking status
        // ---------------------------------------------------------

        int success = 0;

        glGetProgramiv(
            program,
            GL_LINK_STATUS,
            &success);

        if (!success)
        {
            int logLength = 0;

            glGetProgramiv(
                program,
                GL_INFO_LOG_LENGTH,
                &logLength);

            std::string errorLog;

            if (logLength > 0)
            {
                errorLog.resize(
                    static_cast<size_t>(logLength));

                glGetProgramInfoLog(
                    program,
                    logLength,
                    nullptr,
                    errorLog.data());
            }

            std::cerr
                << "OpenGL shader program linking failed:"
                << std::endl
                << errorLog
                << std::endl;

            glDeleteProgram(program);

            return false;
        }

        m_programId = program;

        return true;
    }

} // namespace VLTEngine::Graphics