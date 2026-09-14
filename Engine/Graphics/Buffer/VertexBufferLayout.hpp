#pragma once

#include <cstddef>
#include <vector>

namespace VLTEngine::Graphics
{

    enum class ShaderDataType
    {
        None = 0,

        Float,
        Float2,
        Float3,
        Float4,

        Int,
        Int2,
        Int3,
        Int4,

        Mat3,
        Mat4
    };

    struct BufferElement
    {
        ShaderDataType type;
        std::size_t size;
        std::size_t offset;
        bool normalized;

        BufferElement(
            ShaderDataType type,
            bool normalized = false);
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() = default;
        ~VertexBufferLayout() = default;

        void addElement(
            ShaderDataType type,
            bool normalized = false);

        const std::vector<BufferElement> &
        getElements() const;

        std::size_t getStride() const;

    private:
        std::vector<BufferElement> m_elements;
        std::size_t m_stride = 0;
    };

}