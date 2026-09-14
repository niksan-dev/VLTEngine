#include <Graphics/Buffer/OpenGLVertexBuffer.hpp>

#include <glad/glad.h>

namespace VLTEngine::Graphics
{

    OpenGLVertexBuffer::OpenGLVertexBuffer(
        const void *data,
        std::size_t size)
        : m_rendererId(0)
    {
        glGenBuffers(
            1,
            &m_rendererId);

        glBindBuffer(
            GL_ARRAY_BUFFER,
            m_rendererId);

        glBufferData(
            GL_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(size),
            data,
            GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        if (m_rendererId != 0)
        {
            glDeleteBuffers(
                1,
                &m_rendererId);

            m_rendererId = 0;
        }
    }

    void OpenGLVertexBuffer::bind()
    {
        glBindBuffer(
            GL_ARRAY_BUFFER,
            m_rendererId);
    }

    void OpenGLVertexBuffer::unbind()
    {
        glBindBuffer(
            GL_ARRAY_BUFFER,
            0);
    }

} // namespace VLTEngine::Graphics