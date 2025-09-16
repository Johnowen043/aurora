// ============================================
// src/platform/x11/X11Platform.cpp (simplified)
// ============================================
#ifdef AURORA_PLATFORM_X11

#include "aurora/platform/IPlatform.hpp"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <unordered_map>

namespace Aurora {

class X11Window : public Window {
public:
    X11Window(const Config& config) : Window(config) {}
    
    void show() override { /* X11 implementation */ }
    void hide() override { /* X11 implementation */ }
    void close() override { /* X11 implementation */ }
    void setTitle(const std::string& title) override { /* X11 implementation */ }
    void setPosition(i32 x, i32 y) override { /* X11 implementation */ }
    void setSize(u32 width, u32 height) override { /* X11 implementation */ }
    void setOpacity(f32 opacity) override { /* X11 implementation */ }
};

class X11Platform : public IPlatform {
public:
    X11Platform() : m_display(nullptr) {}
    ~X11Platform() { shutdown(); }
    
    bool initialize() override {
        m_display = XOpenDisplay(nullptr);
        if (!m_display) {
            return false;
        }
        
        m_screen = DefaultScreen(m_display);
        m_rootWindow = RootWindow(m_display, m_screen);
        
        // Setup OpenGL attributes
        GLint glxAttribs[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_DEPTH_SIZE, 24,
            GLX_STENCIL_SIZE, 8,
            GLX_RED_SIZE, 8,
            GLX_GREEN_SIZE, 8,
            GLX_BLUE_SIZE, 8,
            GLX_ALPHA_SIZE, 8,
            GLX_SAMPLE_BUFFERS, 1,
            GLX_SAMPLES, 4,
            None
        };
        
        m_visualInfo = glXChooseVisual(m_display, m_screen, glxAttribs);
        if (!m_visualInfo) {
            XCloseDisplay(m_display);
            return false;
        }
        
        return true;
    }
    
    void shutdown() override {
        if (m_visualInfo) {
            XFree(m_visualInfo);
            m_visualInfo = nullptr;
        }
        if (m_display) {
            XCloseDisplay(m_display);
            m_display = nullptr;
        }
    }
    
    Ref<Window> createWindow(const Window::Config& config) override {
        auto window = std::make_shared<X11Window>(config);
        
        // Create X11 window
        XSetWindowAttributes attrs;
        attrs.colormap = XCreateColormap(m_display, m_rootWindow, 
                                        m_visualInfo->visual, AllocNone);
        attrs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                          ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                          StructureNotifyMask | FocusChangeMask;
        attrs.background_pixel = 0;
        attrs.border_pixel = 0;
        
        ::Window xwindow = XCreateWindow(
            m_display, m_rootWindow,
            config.x >= 0 ? config.x : 0,
            config.y >= 0 ? config.y : 0,
            config.width, config.height,
            0, m_visualInfo->depth, InputOutput, m_visualInfo->visual,
            CWColormap | CWEventMask | CWBackPixel | CWBorderPixel,
            &attrs
        );
        
        window->setNativeHandle(reinterpret_cast<void*>(xwindow));
        m_windows[xwindow] = window.get();
        
        // Set window properties based on type
        if (config.type == Window::Type::Dock) {
            // Set _NET_WM_WINDOW_TYPE_DOCK
            Atom windowType = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE", False);
            Atom dockType = XInternAtom(m_display, "_NET_WM_WINDOW_TYPE_DOCK", False);
            XChangeProperty(m_display, xwindow, windowType, XA_ATOM, 32,
                          PropModeReplace, (unsigned char*)&dockType, 1);
        }
        
        XMapWindow(m_display, xwindow);
        XFlush(m_display);
        
        return window;
    }
    
    void destroyWindow(Window* window) override {
        ::Window xwindow = reinterpret_cast<::Window>(window->nativeHandle());
        m_windows.erase(xwindow);
        XDestroyWindow(m_display, xwindow);
    }
    
    void pumpEvents() override {
        while (XPending(m_display)) {
            XEvent xevent;
            XNextEvent(m_display, &xevent);
            
            Event event;
            ::Window xwindow = xevent.xany.window;
            auto it = m_windows.find(xwindow);
            if (it != m_windows.end()) {
                event.window = it->second;
            }
            
            switch (xevent.type) {
                case ConfigureNotify:
                    event.type = EventType::WindowResize;
                    event.size.width = xevent.xconfigure.width;
                    event.size.height = xevent.xconfigure.height;
                    m_eventQueue.push_back(event);
                    break;
                    
                case MotionNotify:
                    event.type = EventType::MouseMove;
                    event.mouse.x = xevent.xmotion.x;
                    event.mouse.y = xevent.xmotion.y;
                    m_eventQueue.push_back(event);
                    break;
                    
                case ButtonPress:
                    event.type = EventType::MouseDown;
                    event.mouseButton.button = static_cast<MouseButton>(xevent.xbutton.button);
                    m_eventQueue.push_back(event);
                    break;
                    
                case KeyPress:
                    event.type = EventType::KeyDown;
                    // Convert X11 keycode to Aurora KeyCode
                    event.key.code = convertKeyCode(xevent.xkey.keycode);
                    event.key.modifiers = convertModifiers(xevent.xkey.state);
                    event.key.repeat = false;
                    m_eventQueue.push_back(event);
                    break;
            }
        }
    }
    
    bool hasEvents() const override {
        return !m_eventQueue.empty();
    }
    
    Event nextEvent() override {
        if (m_eventQueue.empty()) {
            return Event(EventType::None);
        }
        Event event = m_eventQueue.front();
        m_eventQueue.erase(m_eventQueue.begin());
        return event;
    }
    
    u32 displayCount() const override { return 1; }
    
    Rect displayBounds(u32 index) const override {
        return {0, 0, 
                (f32)DisplayWidth(m_display, m_screen),
                (f32)DisplayHeight(m_display, m_screen)};
    }
    
    std::string name() const override { return "X11"; }
    
    void* createGLContext(Window* window) override {
        ::Window xwindow = reinterpret_cast<::Window>(window->nativeHandle());
        GLXContext context = glXCreateContext(m_display, m_visualInfo, nullptr, GL_TRUE);
        glXMakeCurrent(m_display, xwindow, context);
        return context;
    }
    
    void destroyGLContext(void* context) override {
        glXDestroyContext(m_display, static_cast<GLXContext>(context));
    }
    
    void makeCurrent(Window* window, void* context) override {
        ::Window xwindow = reinterpret_cast<::Window>(window->nativeHandle());
        glXMakeCurrent(m_display, xwindow, static_cast<GLXContext>(context));
    }
    
    void swapBuffers(Window* window) override {
        ::Window xwindow = reinterpret_cast<::Window>(window->nativeHandle());
        glXSwapBuffers(m_display, xwindow);
    }
    
private:
    KeyCode convertKeyCode(unsigned int xkey) {
        // Simplified conversion - full implementation would use XLookupKeysym
        if (xkey >= 24 && xkey <= 33) return static_cast<KeyCode>('1' + (xkey - 24));
        if (xkey >= 38 && xkey <= 63) return static_cast<KeyCode>('A' + (xkey - 38));
        return KeyCode::Unknown;
    }
    
    u32 convertModifiers(unsigned int state) {
        u32 mods = 0;
        if (state & ShiftMask) mods |= (u32)KeyModifiers::Shift;
        if (state & ControlMask) mods |= (u32)KeyModifiers::Ctrl;
        if (state & Mod1Mask) mods |= (u32)KeyModifiers::Alt;
        return mods;
    }
    
    Display* m_display;
    int m_screen;
    ::Window m_rootWindow;
    XVisualInfo* m_visualInfo;
    std::unordered_map<::Window, Window*> m_windows;
    std::vector<Event> m_eventQueue;
};

} // namespace Aurora

#endif // AURORA_PLATFORM_X11