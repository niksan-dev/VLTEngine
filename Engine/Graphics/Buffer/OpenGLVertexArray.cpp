#include <Graphics/Buffer/OpenGLVertexArray.hpp>
#include <Graphics/Buffer/OpenGLIndexBuffer.hpp>

#include <glad/glad.h>

namespace VLTEngine::Graphics
{

    namespace
    {

        unsigned int ShaderDataTypeComponentCount(
            ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Float:
                return 1;

            case ShaderDataType::Float2:
                return 2;

            case ShaderDataType::Float3:
                return 3;

            case ShaderDataType::Float4:
                return 4;

            case ShaderDataType::Int:
                return 1;

            case ShaderDataType::Int2:
                return 2;

            case ShaderDataType::Int3:
                return 3;

            case ShaderDataType::Int4:
                return 4;

            case ShaderDataType::Mat3:
                return 3 * 3;

            case ShaderDataType::Mat4:
                return 4 * 4;

            case ShaderDataType::None:
            default:
                return 0;
            }
        }

        bool IsIntegerType(
            ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return true;

            default:
                return false;
            }
        }

    }

    OpenGLVertexArray::OpenGLVertexArray()
        : m_rendererId(0),
          m_indexBuffer(nullptr)
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

        m_indexBuffer = nullptr;
    }

    void OpenGLVertexArray::bind()
    {
        glBindVertexArray(m_rendererId);
    }

    void OpenGLVertexArray::unbind()
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::addVertexBuffer(
        VertexBuffer *vertexBuffer)
    {
        if (vertexBuffer == nullptr)
            return;

        bind();
        vertexBuffer->bind();

        const auto &layout =
            vertexBuffer->getLayout();

        unsigned int attributeIndex = 0;

        for (const auto &element :
             layout.getElements())
        {
            if (element.type == ShaderDataType::None)
                continue;

            if (IsIntegerType(element.type))
            {
                glEnableVertexAttribArray(
                    attributeIndex);

                glVertexAttribIPointer(
                    attributeIndex,
                    static_cast<GLint>(
                        ShaderDataTypeComponentCount(
                            element.type)),
                    GL_INT,
                    static_cast<GLsizei>(
                        layout.getStride()),
                    reinterpret_cast<const void *>(
                        element.offset));
            }
            else
            {
                glEnableVertexAttribArray(
                    attributeIndex);

                glVertexAttribPointer(
                    attributeIndex,
                    static_cast<GLint>(
                        ShaderDataTypeComponentCount(
                            element.type)),
                    GL_FLOAT,
                    element.normalized
                        ? GL_TRUE
                        : GL_FALSE,
                    static_cast<GLsizei>(
                        layout.getStride()),
                    reinterpret_cast<const void *>(
                        element.offset));
            }

            ++attributeIndex;
        }
    }

    void OpenGLVertexArray::setIndexBuffer(
        OpenGLIndexBuffer *indexBuffer)
    {
        if (indexBuffer == nullptr)
            return;

        bind();

        indexBuffer->bind();

        m_indexBuffer = indexBuffer;
    }

}