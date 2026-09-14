#pragma once

namespace VLTEngine::Graphics
{

    class VertexArray
    {
    public:
        virtual ~VertexArray() = default;

        VertexArray(const VertexArray &) = delete;
        VertexArray &operator=(const VertexArray &) = delete;

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        VertexArray() = default;
    };

} // namespace VLTEngine::Graphics