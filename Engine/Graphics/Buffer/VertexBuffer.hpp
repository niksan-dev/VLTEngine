#pragma once

#include <Graphics/Buffer/VertexBufferLayout.hpp>

namespace VLTEngine::Graphics
{

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        VertexBuffer(const VertexBuffer &) = delete;
        VertexBuffer &operator=(const VertexBuffer &) = delete;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setLayout(
            const VertexBufferLayout &layout) = 0;

        virtual const VertexBufferLayout &
        getLayout() const = 0;

    protected:
        VertexBuffer() = default;
    };

}