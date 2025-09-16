// ============================================
// examples/01_hello_window/main.cpp
// ============================================
#include <aurora/aurora.hpp>
#include <iostream>
#include <GL/gl.h>

using namespace Aurora;

int main(int argc, char** argv) {
    // Create application
    Application::Config appConfig;
    appConfig.name = "Aurora Hello Window";
    Application app(argc, argv, appConfig);
    
    // Create window
    Window::Config winConfig;
    winConfig.title = "Hello Aurora!";
    winConfig.width = 1024;
    winConfig.height = 768;
    winConfig.transparent = false;
    
    auto window = app.platform()->createWindow(winConfig);
    
    // Create OpenGL context
    void* glContext = app.platform()->createGLContext(window.get());
    
    // Setup window callbacks
    window->onClose = [&app]() {
        std::cout << "Window closed!" << std::endl;
        app.quit();
    };
    
    window->onResize = [](u32 width, u32 height) {
        std::cout << "Window resized: " << width << "x" << height << std::endl;
        glViewport(0, 0, width, height);
    };
    
    // Animation variables
    f32 hue = 0.0f;
    
    // Register frame callback
    app.onFrame([&](f64 deltaTime) {
        // Animate background color
        hue += deltaTime * 50.0f;
        if (hue > 360.0f) hue -= 360.0f;
        
        // Convert HSV to RGB
        f32 c = 1.0f;
        f32 x = c * (1 - abs(fmod(hue / 60.0f, 2) - 1));
        f32 r = 0, g = 0, b = 0;
        
        if (hue < 60) { r = c; g = x; }
        else if (hue < 120) { r = x; g = c; }
        else if (hue < 180) { g = c; b = x; }
        else if (hue < 240) { g = x; b = c; }
        else if (hue < 300) { r = x; b = c; }
        else { r = c; b = x; }
        
        // Clear with animated color
        glClearColor(r * 0.3f, g * 0.3f, b * 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw something simple
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.0f, 0.5f);
        glEnd();
        
        // Swap buffers
        app.platform()->swapBuffers(window.get());
        
        // Print FPS every second
        static f64 fpsTimer = 0;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0) {
            std::cout << "FPS: " << app.fps() << std::endl;
            fpsTimer = 0;
        }
    });
    
    // Show window
    window->show();
    
    // Run application
    return app.run();
}