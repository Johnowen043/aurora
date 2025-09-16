// ============================================
// include/aurora/graphics/Shader.hpp
// ============================================
#pragma once
#include "../core/Types.hpp"
#include <string>
#include <unordered_map>
#include <GL/glew.h>

namespace Aurora {

class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    
    // Shader usage
    void use() const;
    void unbind() const;
    
    // Uniform setters
    void setInt(const std::string& name, i32 value);
    void setFloat(const std::string& name, f32 value);
    void setVec2(const std::string& name, const Vec2& value);
    void setVec3(const std::string& name, f32 x, f32 y, f32 z);
    void setVec4(const std::string& name, f32 x, f32 y, f32 z, f32 w);
    void setMat4(const std::string& name, const f32* matrix);
    void setColor(const std::string& name, const Color& color);
    
    // Get program ID
    GLuint programId() const { return m_program; }
    
    // Create from files
    static Ref<Shader> fromFiles(const std::string& vertexPath, 
                                  const std::string& fragmentPath);
    
    // Built-in shaders
    static Ref<Shader> createBasic();
    static Ref<Shader> createTextured();
    static Ref<Shader> createBlur();
    
private:
    GLuint compileShader(GLenum type, const std::string& source);
    GLint getUniformLocation(const std::string& name);
    
    GLuint m_program;
    std::unordered_map<std::string, GLint> m_uniformCache;
};

} // namespace Aurora
