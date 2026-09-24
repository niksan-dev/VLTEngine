#include <Runtime/Runtime.hpp>

#include <Components/Camera.hpp>
#include <Components/MeshRenderer.hpp>
#include <Components/Transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace VLTEngine::Runtime
{

    Runtime::Runtime()
        : m_initialized(false)
    {
    }

    Runtime::~Runtime()
    {
        shutdown();
    }

    bool Runtime::initialize(
        VLTEngine::Scene::Scene &scene)
    {
        if (m_initialized)
            return true;

        /*
         * Temporary test entity.
         *
         * Runtime owns scene creation.
         * Renderer will later provide the GPU resources.
         */
        auto &triangle =
            scene.createEntity(
                "Triangle");

        auto &triangleTransform =
            triangle.addComponent<
                VLTEngine::Components::Transform>();

        triangleTransform.position =
            glm::vec3(
                0.20f,
                0.0f,
                0.0f);

        triangleTransform.rotation.z =
            glm::radians(
                10.0f);

        triangleTransform.scale =
            glm::vec3(
                0.80f,
                0.80f,
                0.80f);

        triangle.addComponent<
            VLTEngine::Components::MeshRenderer>();

        /*
         * Main Camera
         */
        auto &mainCamera =
            scene.createEntity(
                "MainCamera");

        auto &cameraTransform =
            mainCamera.addComponent<
                VLTEngine::Components::Transform>();

        cameraTransform.position =
            glm::vec3(
                0.0f,
                0.0f,
                3.0f);

        auto &camera =
            mainCamera.addComponent<
                VLTEngine::Components::Camera>();

        camera.setPerspective(
            glm::radians(60.0f),
            16.0f / 9.0f,
            0.1f,
            100.0f);

        scene.setActiveCamera(
            &mainCamera);

        m_initialized = true;

        return true;
    }

    void Runtime::shutdown()
    {
        if (!m_initialized)
            return;

        m_initialized = false;
    }

}