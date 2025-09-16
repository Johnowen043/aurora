// ============================================
// include/aurora/core/Application.hpp
// ============================================
#pragma once
#include "Object.hpp"
#include "../platform/IPlatform.hpp"
#include <memory>
#include <functional>

namespace Aurora {

class Application : public Object {
public:
    struct Config {
        std::string name = "Aurora Application";
        bool vsync = true;
        bool multisampling = true;
        u32 msaaSamples = 4;
    };
    
    Application(int argc, char** argv, const Config& config = {});
    ~Application();
    
    // Singleton access
    static Application* instance() { return s_instance; }
    
    // Main loop
    int run();
    void quit(int exitCode = 0);
    
    // Platform access
    IPlatform* platform() const { return m_platform.get(); }
    
    // Frame callbacks
    void onFrame(std::function<void(f64 deltaTime)> callback);
    
    // FPS tracking
    f32 fps() const { return m_fps; }
    f64 frameTime() const { return m_frameTime; }
    
private:
    void initialize();
    void shutdown();
    void processEvents();
    void update(f64 deltaTime);
    void render();
    
    static Application* s_instance;
    
    Config m_config;
    Unique<IPlatform> m_platform;
    bool m_running = false;
    int m_exitCode = 0;
    
    // Timing
    f64 m_lastFrameTime = 0;
    f64 m_frameTime = 0;
    f32 m_fps = 0;
    
    // Callbacks
    std::vector<std::function<void(f64)>> m_frameCallbacks;
};

} // namespace Aurora
