#pragma once

namespace VLTEngine::Graphics
{

    class Mesh
    {
    public:
        virtual ~Mesh() = default;

        Mesh(const Mesh &) = delete;
        Mesh &operator=(const Mesh &) = delete;

        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void draw() = 0;

    protected:
        Mesh() = default;
    };

}