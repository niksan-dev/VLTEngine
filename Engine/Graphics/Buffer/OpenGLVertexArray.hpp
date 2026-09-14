#pragma once

#include <Graphics/Buffer/VertexArray.hpp>
#include <Graphics/Buffer/VertexBuffer.hpp>

namespace VLTEngine::Graphics
{

    class OpenGLIndexBuffer;

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        OpenGLVertexArray(const OpenGLVertexArray &) = delete;
        OpenGLVertexArray &operator=(const OpenGLVertexArray &) = delete;

        void bind() override;
        void unbind() override;

        void addVertexBuffer(
            VertexBuffer *vertexBuffer);

        void setIndexBuffer(
            OpenGLIndexBuffer *indexBuffer);

    private:
        unsigned int m_rendererId;

        OpenGLIndexBuffer *m_indexBuffer;
    };

}