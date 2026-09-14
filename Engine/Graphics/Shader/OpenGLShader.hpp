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

        void setInt(
            const std::string &name,
            int value) override;

        void setFloat(
            const std::string &name,
            float value) override;

        void setVec2(
            const std::string &name,
            float x,
            float y) override;

        void setVec3(
            const std::string &name,
            float x,
            float y,
            float z) override;

        void setVec4(
            const std::string &name,
            float x,
            float y,
            float z,
            float w) override;

        void setMat4(
            const std::string &name,
            const float *value) override;

        int getUniformLocation(
            const std::string &name) const;

        unsigned int getProgramId() const;

    private:
        unsigned int compileShader(
            unsigned int type,
            const std::string &source);

        bool linkProgram(
            unsigned int vertexShader,
            unsigned int fragmentShader);

    private:
        unsigned int m_programId;
    };

}