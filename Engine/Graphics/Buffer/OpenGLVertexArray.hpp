#pragma once

#include <Graphics/Buffer/VertexArray.hpp>

namespace VLTEngine::Graphics
{

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        OpenGLVertexArray(const OpenGLVertexArray &) = delete;
        OpenGLVertexArray &operator=(const OpenGLVertexArray &) = delete;

        void bind() override;
        void unbind() override;

    private:
        unsigned int m_rendererId;
    };

} // namespace VLTEngine::Graphics