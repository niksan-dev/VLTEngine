#pragma once

#include <string>

namespace VLTEngine::Graphics
{

    class Material
    {
    public:
        virtual ~Material() = default;

        Material(const Material &) = delete;
        Material &operator=(const Material &) = delete;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        virtual void setColor(
            float r,
            float g,
            float b,
            float a) = 0;

    protected:
        Material() = default;
    };

}