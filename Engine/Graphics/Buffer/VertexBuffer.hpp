#pragma once

#include <cstddef>

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

    protected:
        VertexBuffer() = default;
    };

} // namespace VLTEngine::Graphics