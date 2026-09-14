#pragma once

#include <Graphics/Buffer/VertexBuffer.hpp>

#include <cstddef>

namespace VLTEngine::Graphics
{

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(
            const void *data,
            std::size_t size);

        ~OpenGLVertexBuffer() override;

        OpenGLVertexBuffer(
            const OpenGLVertexBuffer &) = delete;

        OpenGLVertexBuffer &operator=(
            const OpenGLVertexBuffer &) = delete;

        void bind() override;
        void unbind() override;

        void setLayout(
            const VertexBufferLayout &layout) override;

        const VertexBufferLayout &
        getLayout() const override;

    private:
        unsigned int m_rendererId;

        VertexBufferLayout m_layout;
    };

}