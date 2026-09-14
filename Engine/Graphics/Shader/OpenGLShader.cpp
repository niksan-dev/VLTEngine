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

    bool OpenGLShader::loadFromSource(
        const std::string &vertexSource,
        const std::string &fragmentSource)
    {
        const unsigned int vertexShader =
            compileShader(
                GL_VERTEX_SHADER,
                vertexSource);

        if (vertexShader == 0)
            return false;

        const unsigned int fragmentShader =
            compileShader(
                GL_FRAGMENT_SHADER,
                fragmentSource);

        if (fragmentShader == 0)
        {
            glDeleteShader(vertexShader);
            return false;
        }

        const bool linked =
            linkProgram(
                vertexShader,
                fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return linked;
    }

    bool OpenGLShader::loadFromFiles(
        const std::string &vertexPath,
        const std::string &fragmentPath)
    {
        std::ifstream vertexFile(vertexPath);

        if (!vertexFile.is_open())
        {
            std::cerr
                << "Failed to open vertex shader: "
                << vertexPath
                << std::endl;

            return false;
        }

        std::ifstream fragmentFile(fragmentPath);

        if (!fragmentFile.is_open())
        {
            std::cerr
                << "Failed to open fragment shader: "
                << fragmentPath
                << std::endl;

            return false;
        }

        std::stringstream vertexStream;
        std::stringstream fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        return loadFromSource(
            vertexStream.str(),
            fragmentStream.str());
    }

    void OpenGLShader::bind()
    {
        glUseProgram(m_programId);
    }

    void OpenGLShader::unbind()
    {
        glUseProgram(0);
    }

    void OpenGLShader::setInt(
        const std::string &name,
        int value)
    {
        const int location =
            getUniformLocation(name);

        if (location == -1)
            return;

        glUniform1i(
            location,
            value);
    }

    void OpenGLShader::setFloat(
        const std::string &name,
        float value)
    {
        const int location =
            getUniformLocation(name);

        if (location == -1)
            return;

        glUniform1f(
            location,
            value);
    }

    void OpenGLShader::setVec2(
        const std::string &name,
        float x,
        float y)
    {
        const int location =
            getUniformLocation(name);

        if (location == -1)
            return;

        glUniform2f(
            location,
            x,
            y);
    }

    void OpenGLShader::setVec3(
        const std::string &name,
        float x,
        float y,
        float z)
    {
        const int location =
            getUniformLocation(name);

        if (location == -1)
            return;

        glUniform3f(
            location,
            x,
            y,
            z);
    }

    void OpenGLShader::setVec4(
        const std::string &name,
        float x,
        float y,
        float z,
        float w)
    {
        const int location =
            getUniformLocation(name);

        if (location == -1)
            return;

        glUniform4f(
            location,
            x,
            y,
            z,
            w);
    }

    void OpenGLShader::setMat4(
        const std::string &name,
        const float *value)
    {
        if (value == nullptr)
            return;

        const int location =
            getUniformLocation(name);

        if (location < 0)
            return;

        glUniformMatrix4fv(
            location,
            1,
            GL_FALSE,
            value);
    }

    unsigned int OpenGLShader::getProgramId() const
    {
        return m_programId;
    }

    int OpenGLShader::getUniformLocation(
        const std::string &name) const
    {
        if (m_programId == 0)
            return -1;

        return glGetUniformLocation(
            m_programId,
            name.c_str());
    }

    unsigned int OpenGLShader::compileShader(
        unsigned int type,
        const std::string &source)
    {
        const unsigned int shader =
            glCreateShader(type);

        const char *sourceCode =
            source.c_str();

        glShaderSource(
            shader,
            1,
            &sourceCode,
            nullptr);

        glCompileShader(shader);

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

            std::string infoLog(
                static_cast<std::size_t>(
                    logLength),
                '\0');

            glGetShaderInfoLog(
                shader,
                logLength,
                nullptr,
                infoLog.data());

            std::cerr
                << "Shader compilation failed:"
                << std::endl
                << infoLog
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

        glAttachShader(
            program,
            vertexShader);

        glAttachShader(
            program,
            fragmentShader);

        glLinkProgram(program);

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

            std::string infoLog(
                static_cast<std::size_t>(
                    logLength),
                '\0');

            glGetProgramInfoLog(
                program,
                logLength,
                nullptr,
                infoLog.data());

            std::cerr
                << "Shader linking failed:"
                << std::endl
                << infoLog
                << std::endl;

            glDeleteProgram(program);

            return false;
        }

        if (m_programId != 0)
        {
            glDeleteProgram(m_programId);
        }

        m_programId = program;

        return true;
    }

}