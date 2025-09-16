#pragma once
#include "graphics/Shader.h"
#include <glad/glad.h>
#include <unordered_map>

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~OpenGLShader();

    void bind() const override;
    void unbind() const override;

    void setUniform(const std::string& name, int value) override;
    void setUniform(const std::string& name, float value) override;
    void setUniform(const std::string& name, const glm::vec3& value) override;
    void setUniform(const std::string& name, const glm::mat4& value) override;

private:
    GLuint m_RendererID;
    mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

    GLint getUniformLocation(const std::string& name) const;
    GLuint compileShader(GLenum type, const std::string& source);
    GLuint createProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
};
