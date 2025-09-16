// ============================================
// include/aurora/platform/Event.hpp
// ============================================
#pragma once
#include "../core/Types.hpp"

namespace Aurora {

enum class EventType {
    None,
    Quit,
    
    // Window events
    WindowClose,
    WindowResize,
    WindowMove,
    WindowFocus,
    WindowBlur,
    
    // Mouse events
    MouseMove,
    MouseDown,
    MouseUp,
    MouseScroll,
    MouseEnter,
    MouseLeave,
    
    // Keyboard events
    KeyDown,
    KeyUp,
    TextInput,
    
    // Touch events
    TouchBegin,
    TouchMove,
    TouchEnd
};

enum class MouseButton {
    None = 0,
    Left = 1,
    Middle = 2,
    Right = 3,
    X1 = 4,
    X2 = 5
};

enum class KeyCode {
    Unknown = 0,
    
    // Letters
    A = 'A', B = 'B', C = 'C', D = 'D', E = 'E',
    F = 'F', G = 'G', H = 'H', I = 'I', J = 'J',
    K = 'K', L = 'L', M = 'M', N = 'N', O = 'O',
    P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T',
    U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y',
    Z = 'Z',
    
    // Numbers
    Num0 = '0', Num1 = '1', Num2 = '2', Num3 = '3', Num4 = '4',
    Num5 = '5', Num6 = '6', Num7 = '7', Num8 = '8', Num9 = '9',
    
    // Function keys
    F1 = 256, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    
    // Special keys
    Escape, Tab, Space, Enter, Backspace, Delete,
    Up, Down, Left, Right,
    Home, End, PageUp, PageDown,
    Insert, PrintScreen, Pause,
    
    // Modifiers
    LeftShift, RightShift,
    LeftCtrl, RightCtrl,
    LeftAlt, RightAlt,
    LeftSuper, RightSuper
};

enum class KeyModifiers {
    None = 0,
    Shift = 1 << 0,
    Ctrl = 1 << 1,
    Alt = 1 << 2,
    Super = 1 << 3
};

struct Event {
    EventType type = EventType::None;
    Window* window = nullptr;
    
    union {
        struct { u32 width, height; } size;
        struct { i32 x, y; } position;
        struct { f32 x, y; } mouse;
        struct { f32 dx, dy; } scroll;
        struct { MouseButton button; } mouseButton;
        struct { 
            KeyCode code;
            u32 modifiers;
            bool repeat;
        } key;
        struct { char text[32]; } text;
    };
    
    Event() {}
    Event(EventType t) : type(t) {}
};

} // namespace Aurora
