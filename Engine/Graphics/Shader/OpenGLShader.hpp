#pragma once

#include <Graphics/Shader/Shader.hpp>

#include <string>

namespace VLTEngine::Graphics
{

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader();
        ~OpenGLShader() override;

        OpenGLShader(const OpenGLShader &) = delete;
        OpenGLShader &operator=(const OpenGLShader &) = delete;

        bool loadFromSource(
            const std::string &vertexSource,
            const std::string &fragmentSource) override;

        bool loadFromFiles(
            const std::string &vertexPath,
            const std::string &fragmentPath) override;

        void bind() override;
        void unbind() override;

        unsigned int getProgramId() const;

    private:
        unsigned int compileShader(
            unsigned int type,
            const std::string &source);

        bool linkProgram(
            unsigned int vertexShader,
            unsigned int fragmentShader);

        unsigned int m_programId;
    };

} // namespace VLTEngine::Graphics