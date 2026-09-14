#pragma once

#include <string>

namespace VLTEngine::Graphics
{

    class Shader
    {
    public:
        virtual ~Shader() = default;

        Shader(const Shader &) = delete;
        Shader &operator=(const Shader &) = delete;

        virtual bool loadFromSource(
            const std::string &vertexSource,
            const std::string &fragmentSource) = 0;

        virtual bool loadFromFiles(
            const std::string &vertexPath,
            const std::string &fragmentPath) = 0;

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        Shader() = default;
    };

} // namespace VLTEngine::Graphics