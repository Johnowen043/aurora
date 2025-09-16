// ============================================
// include/aurora/animation/Spring.hpp
// ============================================
#pragma once
#include "../core/Types.hpp"
#include <cmath>

namespace Aurora {

class Spring {
public:
    struct Config {
        f32 stiffness = 100.0f;   // Spring constant (higher = stiffer)
        f32 damping = 10.0f;       // Damping ratio (higher = less oscillation)
        f32 mass = 1.0f;           // Mass (higher = slower response)
        f32 restLength = 0.0f;     // Rest position
        f32 initialVelocity = 0.0f;
    };
    
    Spring(const Config& config = {});
    
    // Update spring physics
    void update(f32 deltaTime);
    
    // Set target position
    void setTarget(f32 target) { m_target = target; }
    f32 target() const { return m_target; }
    
    // Get current position
    f32 position() const { return m_position; }
    f32 velocity() const { return m_velocity; }
    
    // Apply impulse
    void applyImpulse(f32 force);
    
    // Reset spring
    void reset(f32 position = 0, f32 velocity = 0);
    
    // Check if spring is at rest
    bool isAtRest(f32 threshold = 0.001f) const;
    
    // Config access
    Config& config() { return m_config; }
    const Config& config() const { return m_config; }
    
private:
    Config m_config;
    f32 m_position = 0;
    f32 m_velocity = 0;
    f32 m_target = 0;
};

// 2D Spring for Vec2 animations
class Spring2D {
public:
    Spring2D(const Spring::Config& config = {});
    
    void update(f32 deltaTime);
    void setTarget(const Vec2& target);
    Vec2 position() const { return {m_springX.position(), m_springY.position()}; }
    Vec2 velocity() const { return {m_springX.velocity(), m_springY.velocity()}; }
    void reset(const Vec2& position = {}, const Vec2& velocity = {});
    bool isAtRest(f32 threshold = 0.001f) const;
    
private:
    Spring m_springX;
    Spring m_springY;
};

} // namespace Aurora
