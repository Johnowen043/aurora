// ============================================
// src/core/Application.cpp
// ============================================
#include "aurora/core/Application.hpp"
#include <chrono>
#include <GL/gl.h>

namespace Aurora {

Application* Application::s_instance = nullptr;

Application::Application(int argc, char** argv, const Config& config)
    : m_config(config) {
    s_instance = this;
    initialize();
}

Application::~Application() {
    shutdown();
    s_instance = nullptr;
}

void Application::initialize() {
    // Create platform
    m_platform = IPlatform::create();
    if (!m_platform->initialize()) {
        throw std::runtime_error("Failed to initialize platform");
    }
}

void Application::shutdown() {
    if (m_platform) {
        m_platform->shutdown();
    }
}

int Application::run() {
    m_running = true;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    auto lastTime = startTime;
    
    while (m_running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<f64> elapsed = currentTime - lastTime;
        f64 deltaTime = elapsed.count();
        lastTime = currentTime;
        
        // Update FPS
        m_frameTime = deltaTime;
        m_fps = 1.0f / deltaTime;
        
        // Process events
        processEvents();
        
        // Update
        update(deltaTime);
        
        // Render
        render();
    }
    
    return m_exitCode;
}

void Application::quit(int exitCode) {
    m_exitCode = exitCode;
    m_running = false;
}

void Application::processEvents() {
    m_platform->pumpEvents();
    
    while (m_platform->hasEvents()) {
        Event event = m_platform->nextEvent();
        
        switch (event.type) {
            case EventType::Quit:
                quit();
                break;
                
            case EventType::WindowClose:
                if (event.window && event.window->onClose) {
                    event.window->onClose();
                }
                break;
                
            case EventType::WindowResize:
                if (event.window && event.window->onResize) {
                    event.window->onResize(event.size.width, event.size.height);
                }
                break;
                
            default:
                break;
        }
    }
}

void Application::update(f64 deltaTime) {
    for (auto& callback : m_frameCallbacks) {
        callback(deltaTime);
    }
}

void Application::render() {
    // This will be handled by the Renderer in the next step
}

void Application::onFrame(std::function<void(f64)> callback) {
    m_frameCallbacks.push_back(callback);
}

} // namespace Aurora
