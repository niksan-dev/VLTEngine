#pragma once

#include <Entity/Component.hpp>

#include <Graphics/Mesh/Mesh.hpp>
#include <Graphics/Material/Material.hpp>

namespace VLTEngine::Components
{

    class MeshRenderer
        : public VLTEngine::Entity::Component
    {
    public:
        explicit MeshRenderer(
            VLTEngine::Entity::Entity *entity = nullptr);

        ~MeshRenderer() override = default;

        MeshRenderer(const MeshRenderer &) = delete;
        MeshRenderer &operator=(const MeshRenderer &) = delete;

        void setMesh(
            VLTEngine::Graphics::Mesh *mesh);

        void setMaterial(
            VLTEngine::Graphics::Material *material);

        VLTEngine::Graphics::Mesh *getMesh();
        const VLTEngine::Graphics::Mesh *getMesh() const;

        VLTEngine::Graphics::Material *getMaterial();
        const VLTEngine::Graphics::Material *getMaterial() const;

        bool isRenderable() const;

        void render();

    private:
        VLTEngine::Graphics::Mesh *m_mesh;
        VLTEngine::Graphics::Material *m_material;
    };

}