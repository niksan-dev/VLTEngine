#pragma once

#include <Graphics/Mesh/Mesh.hpp>
#include <Graphics/Mesh/MeshData.hpp>

#include <Graphics/Buffer/OpenGLVertexArray.hpp>
#include <Graphics/Buffer/OpenGLVertexBuffer.hpp>
#include <Graphics/Buffer/OpenGLIndexBuffer.hpp>

#include <memory>

namespace VLTEngine::Graphics
{

    class OpenGLMesh : public Mesh
    {
    public:
        explicit OpenGLMesh(
            const MeshData &meshData);

        ~OpenGLMesh() override;

        OpenGLMesh(const OpenGLMesh &) = delete;
        OpenGLMesh &operator=(const OpenGLMesh &) = delete;

        void bind() override;
        void unbind() override;

        void draw() override;

    private:
        std::unique_ptr<OpenGLVertexArray> m_vertexArray;
        std::unique_ptr<OpenGLVertexBuffer> m_vertexBuffer;
        std::unique_ptr<OpenGLIndexBuffer> m_indexBuffer;
    };

}