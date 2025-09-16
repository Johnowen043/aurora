// ============================================
// include/aurora/core/Types.hpp
// ============================================
#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <functional>

namespace Aurora {

// Basic types
using i32 = int32_t;
using u32 = uint32_t;
using f32 = float;
using f64 = double;

// Smart pointers
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T>
using Weak = std::weak_ptr<T>;

// Math types
struct Vec2 {
    f32 x, y;
    Vec2(f32 x = 0, f32 y = 0) : x(x), y(y) {}
    Vec2 operator+(const Vec2& v) const { return {x + v.x, y + v.y}; }
    Vec2 operator-(const Vec2& v) const { return {x - v.x, y - v.y}; }
    Vec2 operator*(f32 s) const { return {x * s, y * s}; }
};

struct Rect {
    f32 x, y, width, height;
    
    bool contains(const Vec2& p) const {
        return p.x >= x && p.x <= x + width &&
               p.y >= y && p.y <= y + height;
    }
};

struct Color {
    f32 r, g, b, a;
    
    Color(f32 r = 0, f32 g = 0, f32 b = 0, f32 a = 1)
        : r(r), g(g), b(b), a(a) {}
    
    static Color fromHex(u32 hex) {
        return Color(
            ((hex >> 24) & 0xFF) / 255.0f,
            ((hex >> 16) & 0xFF) / 255.0f,
            ((hex >> 8) & 0xFF) / 255.0f,
            (hex & 0xFF) / 255.0f
        );
    }
};

} // namespace Aurora