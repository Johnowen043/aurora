// ============================================
// include/aurora/platform/Window.hpp
// ============================================
#pragma once
#include "../core/Object.hpp"
#include "../core/Types.hpp"
#include <functional>

namespace Aurora {

class Window : public Object {
public:
    enum class Type {
        Normal,
        Dock,
        Panel,
        Popup,
        Desktop,
        Splash
    };
    
    struct Config {
        std::string title = "Aurora Window";
        i32 x = -1, y = -1;  // -1 = centered
        u32 width = 800, height = 600;
        Type type = Type::Normal;
        bool decorated = true;
        bool resizable = true;
        bool transparent = false;
        bool alwaysOnTop = false;
        f32 opacity = 1.0f;
    };
    
    Window(const Config& config);
    virtual ~Window();
    
    // Window properties
    const Config& config() const { return m_config; }
    void* nativeHandle() const { return m_nativeHandle; }
    void setNativeHandle(void* handle) { m_nativeHandle = handle; }
    
    // Window operations
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void close() = 0;
    
    virtual void setTitle(const std::string& title) = 0;
    virtual void setPosition(i32 x, i32 y) = 0;
    virtual void setSize(u32 width, u32 height) = 0;
    virtual void setOpacity(f32 opacity) = 0;
    
    // Event callbacks
    std::function<void()> onClose;
    std::function<void(u32, u32)> onResize;
    std::function<void(i32, i32)> onMove;
    std::function<void()> onFocus;
    std::function<void()> onBlur;
    
protected:
    Config m_config;
    void* m_nativeHandle = nullptr;
    void* m_glContext = nullptr;
};

} // namespace Aurora