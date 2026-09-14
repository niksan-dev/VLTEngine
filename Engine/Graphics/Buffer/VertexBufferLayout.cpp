#include <Graphics/Buffer/VertexBufferLayout.hpp>

namespace VLTEngine::Graphics
{

    namespace
    {

        std::size_t ShaderDataTypeSize(
            ShaderDataType type)
        {
            switch (type)
            {
            case ShaderDataType::Float:
                return sizeof(float);

            case ShaderDataType::Float2:
                return sizeof(float) * 2;

            case ShaderDataType::Float3:
                return sizeof(float) * 3;

            case ShaderDataType::Float4:
                return sizeof(float) * 4;

            case ShaderDataType::Int:
                return sizeof(int);

            case ShaderDataType::Int2:
                return sizeof(int) * 2;

            case ShaderDataType::Int3:
                return sizeof(int) * 3;

            case ShaderDataType::Int4:
                return sizeof(int) * 4;

            case ShaderDataType::Mat3:
                return sizeof(float) * 3 * 3;

            case ShaderDataType::Mat4:
                return sizeof(float) * 4 * 4;

            case ShaderDataType::None:
            default:
                return 0;
            }
        }

    }

    BufferElement::BufferElement(
        ShaderDataType type,
        bool normalized)
        : type(type),
          size(ShaderDataTypeSize(type)),
          offset(0),
          normalized(normalized)
    {
    }

    void VertexBufferLayout::addElement(
        ShaderDataType type,
        bool normalized)
    {
        BufferElement element(
            type,
            normalized);

        element.offset = m_stride;

        m_elements.push_back(element);

        m_stride += element.size;
    }

    const std::vector<BufferElement> &
    VertexBufferLayout::getElements() const
    {
        return m_elements;
    }

    std::size_t VertexBufferLayout::getStride() const
    {
        return m_stride;
    }

}