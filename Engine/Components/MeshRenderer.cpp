#include <Components/MeshRenderer.hpp>

namespace VLTEngine::Components
{

    MeshRenderer::MeshRenderer(
        VLTEngine::Entity::Entity *entity)
        : Component(entity),
          m_mesh(nullptr),
          m_material(nullptr)
    {
    }

    void MeshRenderer::setMesh(
        VLTEngine::Graphics::Mesh *mesh)
    {
        m_mesh = mesh;
    }

    void MeshRenderer::setMaterial(
        VLTEngine::Graphics::Material *material)
    {
        m_material = material;
    }

    VLTEngine::Graphics::Mesh *
    MeshRenderer::getMesh()
    {
        return m_mesh;
    }

    const VLTEngine::Graphics::Mesh *
    MeshRenderer::getMesh() const
    {
        return m_mesh;
    }

    VLTEngine::Graphics::Material *
    MeshRenderer::getMaterial()
    {
        return m_material;
    }

    const VLTEngine::Graphics::Material *
    MeshRenderer::getMaterial() const
    {
        return m_material;
    }

    bool MeshRenderer::isRenderable() const
    {
        return isEnabled() &&
               m_mesh != nullptr &&
               m_material != nullptr;
    }

    void MeshRenderer::render()
    {
        if (!isRenderable())
            return;

        m_material->bind();

        m_mesh->draw();

        m_material->unbind();
    }

}