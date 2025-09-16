// ============================================
// include/aurora/platform/IPlatform.hpp
// ============================================
#pragma once
#include "../core/Types.hpp"
#include "Window.hpp"
#include "Event.hpp"
#include <memory>
#include <vector>

namespace Aurora {

class IPlatform {
public:
    virtual ~IPlatform() = default;
    
    // Platform initialization
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    
    // Window management
    virtual Ref<Window> createWindow(const Window::Config& config) = 0;
    virtual void destroyWindow(Window* window) = 0;
    
    // Event handling
    virtual void pumpEvents() = 0;
    virtual bool hasEvents() const = 0;
    virtual Event nextEvent() = 0;
    
    // Display information
    virtual u32 displayCount() const = 0;
    virtual Rect displayBounds(u32 index) const = 0;
    
    // Platform name
    virtual std::string name() const = 0;
    
    // OpenGL context creation
    virtual void* createGLContext(Window* window) = 0;
    virtual void destroyGLContext(void* context) = 0;
    virtual void makeCurrent(Window* window, void* context) = 0;
    virtual void swapBuffers(Window* window) = 0;
    
    // Factory method
    static Unique<IPlatform> create();
};

} // namespace Aurora