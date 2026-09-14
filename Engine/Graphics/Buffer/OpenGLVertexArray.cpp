#include <Graphics/Buffer/OpenGLVertexArray.hpp>

#include <glad/glad.h>

namespace VLTEngine::Graphics
{

    OpenGLVertexArray::OpenGLVertexArray()
        : m_rendererId(0)
    {
        glGenVertexArrays(
            1,
            &m_rendererId);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        if (m_rendererId != 0)
        {
            glDeleteVertexArrays(
                1,
                &m_rendererId);

            m_rendererId = 0;
        }
    }

    void OpenGLVertexArray::bind()
    {
        glBindVertexArray(
            m_rendererId);
    }

    void OpenGLVertexArray::unbind()
    {
        glBindVertexArray(0);
    }

} // namespace VLTEngine::Graphics