#pragma once
#include <string>
#include <glm/glm.hpp>

// Shader abstraction (API-agnostic interface)
class Shader {
public:
    virtual ~Shader() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void setUniform(const std::string& name, int value) = 0;
    virtual void setUniform(const std::string& name, float value) = 0;
    virtual void setUniform(const std::string& name, const glm::vec3& value) = 0;
    virtual void setUniform(const std::string& name, const glm::mat4& value) = 0;
};
