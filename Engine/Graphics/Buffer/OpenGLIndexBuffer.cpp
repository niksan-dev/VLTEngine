#include <Graphics/Buffer/OpenGLIndexBuffer.hpp>

#include <glad/glad.h>

namespace VLTEngine::Graphics
{

    OpenGLIndexBuffer::OpenGLIndexBuffer(
        const unsigned int *indices,
        std::size_t count)
        : m_rendererId(0),
          m_count(count)
    {
        glGenBuffers(
            1,
            &m_rendererId);

        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            m_rendererId);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(
                count * sizeof(unsigned int)),
            indices,
            GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        if (m_rendererId != 0)
        {
            glDeleteBuffers(
                1,
                &m_rendererId);

            m_rendererId = 0;
        }

        m_count = 0;
    }

    void OpenGLIndexBuffer::bind()
    {
        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            m_rendererId);
    }

    void OpenGLIndexBuffer::unbind()
    {
        glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER,
            0);
    }

    std::size_t OpenGLIndexBuffer::getCount() const
    {
        return m_count;
    }

} // namespace VLTEngine::Graphics