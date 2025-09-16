// ============================================
// include/aurora/graphics/Renderer.hpp
// ============================================
#pragma once
#include "../core/Object.hpp"
#include "../core/Types.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include <stack>
#include <GL/glew.h>

namespace Aurora {

class RenderCommand {
public:
    enum class Type {
        DrawMesh,
        SetShader,
        SetTexture,
        SetScissor,
        Clear
    };
    
    Type type;
    union {
        struct { Mesh* mesh; } drawMesh;
        struct { Shader* shader; } setShader;
        struct { Texture* texture; u32 slot; } setTexture;
        struct { i32 x, y, width, height; } scissor;
        struct { Color color; u32 flags; } clear;
    };
};

class Renderer : public Object {
public:
    struct Stats {
        u32 drawCalls = 0;
        u32 triangles = 0;
        u32 vertices = 0;
        f64 gpuTime = 0;
    };
    
    Renderer();
    ~Renderer();
    
    // Initialization
    bool initialize();
    void shutdown();
    
    // Frame operations
    void beginFrame();
    void endFrame();
    
    // Viewport
    void setViewport(i32 x, i32 y, u32 width, u32 height);
    void setScissor(i32 x, i32 y, u32 width, u32 height);
    void disableScissor();
    
    // Clear operations
    void clear(const Color& color = {0, 0, 0, 1});
    void clearDepth(f32 depth = 1.0f);
    
    // State management
    void pushState();
    void popState();
    
    // Drawing operations
    void setShader(Shader* shader);
    void setTexture(Texture* texture, u32 slot = 0);
    void draw(Mesh* mesh);
    void drawQuad(const Rect& rect, const Color& color = {1, 1, 1, 1});
    void drawCircle(const Vec2& center, f32 radius, const Color& color = {1, 1, 1, 1});
    void drawRoundedRect(const Rect& rect, f32 radius, const Color& color = {1, 1, 1, 1});
    
    // Blending modes
    enum class BlendMode {
        None,
        Alpha,
        Additive,
        Multiply
    };
    void setBlendMode(BlendMode mode);
    
    // Depth testing
    void enableDepthTest(bool enable);
    void setDepthFunc(GLenum func);
    
    // Stats
    const Stats& stats() const { return m_stats; }
    void resetStats();
    
    // Matrix operations (for 2D we'll use simplified matrices)
    void setProjectionMatrix(const f32* matrix);
    void setViewMatrix(const f32* matrix);
    void setModelMatrix(const f32* matrix);
    
    // Convenience matrix builders
    static void orthoMatrix(f32* out, f32 left, f32 right, f32 bottom, f32 top);
    static void translateMatrix(f32* out, f32 x, f32 y);
    static void scaleMatrix(f32* out, f32 x, f32 y);
    static void rotateMatrix(f32* out, f32 angle); // in radians
    
private:
    struct RenderState {
        Shader* shader = nullptr;
        BlendMode blendMode = BlendMode::Alpha;
        bool depthTest = false;
        Rect viewport;
        bool scissorEnabled = false;
        Rect scissorRect;
    };
    
    void applyBlendMode(BlendMode mode);
    void executeCommands();
    
    bool m_initialized = false;
    RenderState m_currentState;
    std::stack<RenderState> m_stateStack;
    std::vector<RenderCommand> m_commandBuffer;
    Stats m_stats;
    
    // Built-in resources
    Ref<Shader> m_basicShader;
    Ref<Mesh> m_quadMesh;
    Ref<Mesh> m_circleMesh;
    
    // Matrices
    f32 m_projectionMatrix[16];
    f32 m_viewMatrix[16];
    f32 m_modelMatrix[16];
};

} // namespace Aurora
