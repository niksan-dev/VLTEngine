#include <Graphics/Mesh/OpenGLMesh.hpp>

#include <glad/glad.h>

namespace VLTEngine::Graphics
{

    OpenGLMesh::OpenGLMesh(
        const MeshData &meshData)
    {
        m_vertexArray =
            std::make_unique<OpenGLVertexArray>();

        m_vertexArray->bind();

        m_vertexBuffer =
            std::make_unique<OpenGLVertexBuffer>(
                meshData.vertices.data(),
                meshData.vertices.size() * sizeof(Vertex));

        VertexBufferLayout layout;

        layout.addElement(
            ShaderDataType::Float3);

        layout.addElement(
            ShaderDataType::Float3);

        layout.addElement(
            ShaderDataType::Float2);

        m_vertexBuffer->setLayout(layout);

        m_vertexArray->addVertexBuffer(
            m_vertexBuffer.get());

        m_indexBuffer =
            std::make_unique<OpenGLIndexBuffer>(
                meshData.indices.data(),
                meshData.indices.size());

        m_vertexArray->setIndexBuffer(
            m_indexBuffer.get());
    }

    OpenGLMesh::~OpenGLMesh()
    {
        m_indexBuffer.reset();
        m_vertexBuffer.reset();
        m_vertexArray.reset();
    }

    void OpenGLMesh::bind()
    {
        if (m_vertexArray)
            m_vertexArray->bind();
    }

    void OpenGLMesh::unbind()
    {
        if (m_vertexArray)
            m_vertexArray->unbind();
    }

    void OpenGLMesh::draw()
    {
        if (!m_vertexArray ||
            !m_indexBuffer)
        {
            return;
        }

        m_vertexArray->bind();

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(
                m_indexBuffer->getCount()),
            GL_UNSIGNED_INT,
            nullptr);
    }

}