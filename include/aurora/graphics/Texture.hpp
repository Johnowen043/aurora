// ============================================
// include/aurora/graphics/Texture.hpp
// ============================================
#pragma once
#include "../core/Types.hpp"
#include <string>
#include <GL/glew.h>

namespace Aurora {

class Texture {
public:
    enum class Format {
        RGB,
        RGBA,
        BGR,
        BGRA,
        Red,
        RG
    };
    
    enum class Filter {
        Nearest,
        Linear,
        Trilinear
    };
    
    enum class Wrap {
        Repeat,
        Clamp,
        Mirror
    };
    
    struct Config {
        Format format = Format::RGBA;
        Filter minFilter = Filter::Linear;
        Filter magFilter = Filter::Linear;
        Wrap wrapS = Wrap::Clamp;
        Wrap wrapT = Wrap::Clamp;
        bool generateMipmaps = true;
        bool srgb = false;
    };
    
    Texture(u32 width, u32 height, const Config& config = {});
    Texture(const std::string& path, const Config& config = {});
    ~Texture();
    
    // Texture operations
    void bind(u32 slot = 0) const;
    void unbind() const;
    
    // Update texture data
    void setData(const void* data, u32 size);
    void setSubData(u32 x, u32 y, u32 width, u32 height, const void* data);
    
    // Properties
    u32 width() const { return m_width; }
    u32 height() const { return m_height; }
    GLuint textureId() const { return m_texture; }
    
    // Create render target texture
    static Ref<Texture> createRenderTarget(u32 width, u32 height, bool depth = true);
    
private:
    void createTexture(const void* data);
    
    GLuint m_texture;
    u32 m_width, m_height;
    Config m_config;
};

} // namespace Aurora