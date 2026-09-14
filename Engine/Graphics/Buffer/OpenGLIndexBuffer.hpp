#pragma once

#include <Graphics/Buffer/IndexBuffer.hpp>

#include <cstddef>

namespace VLTEngine::Graphics
{

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(
            const unsigned int *indices,
            std::size_t count);

        ~OpenGLIndexBuffer() override;

        OpenGLIndexBuffer(const OpenGLIndexBuffer &) = delete;
        OpenGLIndexBuffer &operator=(const OpenGLIndexBuffer &) = delete;

        void bind() override;
        void unbind() override;

        std::size_t getCount() const override;

    private:
        unsigned int m_rendererId;
        std::size_t m_count;
    };

} // namespace VLTEngine::Graphics