// ============================================
// include/aurora/animation/Easing.hpp
// ============================================
#pragma once
#include "../core/Types.hpp"
#include <cmath>
#include <functional>

namespace Aurora {

class Easing {
public:
    using Function = std::function<f32(f32)>;
    
    // Linear
    static f32 linear(f32 t) { return t; }
    
    // Quad
    static f32 quadIn(f32 t) { return t * t; }
    static f32 quadOut(f32 t) { return t * (2 - t); }
    static f32 quadInOut(f32 t) {
        return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t;
    }
    
    // Cubic
    static f32 cubicIn(f32 t) { return t * t * t; }
    static f32 cubicOut(f32 t) { 
        f32 t1 = t - 1;
        return t1 * t1 * t1 + 1;
    }
    static f32 cubicInOut(f32 t) {
        return t < 0.5f ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
    }
    
    // Elastic
    static f32 elasticIn(f32 t) {
        if (t == 0 || t == 1) return t;
        f32 p = 0.3f;
        f32 s = p / 4;
        f32 t1 = t - 1;
        return -powf(2, 10 * t1) * sinf((t1 - s) * (2 * M_PI) / p);
    }
    
    static f32 elasticOut(f32 t) {
        if (t == 0 || t == 1) return t;
        f32 p = 0.3f;
        f32 s = p / 4;
        return powf(2, -10 * t) * sinf((t - s) * (2 * M_PI) / p) + 1;
    }
    
    // Bounce
    static f32 bounceOut(f32 t) {
        if (t < 1 / 2.75f) {
            return 7.5625f * t * t;
        } else if (t < 2 / 2.75f) {
            t -= 1.5f / 2.75f;
            return 7.5625f * t * t + 0.75f;
        } else if (t < 2.5f / 2.75f) {
            t -= 2.25f / 2.75f;
            return 7.5625f * t * t + 0.9375f;
        } else {
            t -= 2.625f / 2.75f;
            return 7.5625f * t * t + 0.984375f;
        }
    }
    
    static f32 bounceIn(f32 t) {
        return 1 - bounceOut(1 - t);
    }
    
    // Back
    static f32 backIn(f32 t) {
        const f32 s = 1.70158f;
        return t * t * ((s + 1) * t - s);
    }
    
    static f32 backOut(f32 t) {
        const f32 s = 1.70158f;
        t = t - 1;
        return t * t * ((s + 1) * t + s) + 1;
    }
    
    // Get easing function by name
    static Function getFunction(const std::string& name);
};

} // namespace Aurora
