#pragma once

#include <Graphics/Material/Material.hpp>
#include <Graphics/Shader/OpenGLShader.hpp>

namespace VLTEngine::Graphics
{

    class OpenGLMaterial : public Material
    {
    public:
        explicit OpenGLMaterial(
            OpenGLShader *shader);

        ~OpenGLMaterial() override;

        OpenGLMaterial(const OpenGLMaterial &) = delete;
        OpenGLMaterial &operator=(const OpenGLMaterial &) = delete;

        void bind() override;
        void unbind() override;

        void setColor(
            float r,
            float g,
            float b,
            float a) override;

    private:
        OpenGLShader *m_shader;

        float m_color[4];
    };

}