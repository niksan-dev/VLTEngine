#pragma once

#include <cstddef>
#include <vector>

namespace VLTEngine::Graphics
{

    struct Vertex
    {
        float position[3];
        float normal[3];
        float texCoord[2];
    };

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::size_t vertexCount() const
        {
            return vertices.size();
        }

        std::size_t indexCount() const
        {
            return indices.size();
        }
    };

}