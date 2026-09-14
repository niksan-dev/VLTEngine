#pragma once

#include <cstddef>

namespace VLTEngine::Graphics
{

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        IndexBuffer(const IndexBuffer &) = delete;
        IndexBuffer &operator=(const IndexBuffer &) = delete;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual std::size_t getCount() const = 0;

    protected:
        IndexBuffer() = default;
    };

} // namespace VLTEngine::Graphics