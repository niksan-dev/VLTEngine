#include <Graphics/Material/OpenGLMaterial.hpp>

namespace VLTEngine::Graphics
{

    OpenGLMaterial::OpenGLMaterial(
        OpenGLShader *shader)
        : m_shader(shader),
          m_color{
              1.0f,
              1.0f,
              1.0f,
              1.0f}
    {
    }

    OpenGLMaterial::~OpenGLMaterial()
    {
        m_shader = nullptr;
    }

    void OpenGLMaterial::bind()
    {
        if (m_shader == nullptr)
            return;

        m_shader->bind();

        m_shader->setVec4(
            "uColor",
            m_color[0],
            m_color[1],
            m_color[2],
            m_color[3]);
    }

    void OpenGLMaterial::unbind()
    {
        if (m_shader == nullptr)
            return;

        m_shader->unbind();
    }

    void OpenGLMaterial::setColor(
        float r,
        float g,
        float b,
        float a)
    {
        m_color[0] = r;
        m_color[1] = g;
        m_color[2] = b;
        m_color[3] = a;
    }

}