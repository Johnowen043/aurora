// ============================================
// include/aurora/animation/Timeline.hpp
// ============================================
#pragma once
#include "../core/Object.hpp"
#include "../core/Types.hpp"
#include "Easing.hpp"
#include <functional>
#include <vector>
#include <memory>

namespace Aurora {

class Animation {
public:
    enum class State {
        Idle,
        Playing,
        Paused,
        Finished
    };
    
    Animation(f32 duration) : m_duration(duration) {}
    virtual ~Animation() = default;
    
    // Animation control
    virtual void start();
    virtual void stop();
    virtual void pause();
    virtual void resume();
    
    // Update animation
    virtual void update(f32 deltaTime);
    
    // Properties
    f32 duration() const { return m_duration; }
    f32 currentTime() const { return m_currentTime; }
    f32 progress() const { return m_duration > 0 ? m_currentTime / m_duration : 1.0f; }
    State state() const { return m_state; }
    
    // Settings
    void setLoop(bool loop) { m_loop = loop; }
    void setReverse(bool reverse) { m_reverse = reverse; }
    void setSpeed(f32 speed) { m_speed = speed; }
    
    // Callbacks
    std::function<void()> onStart;
    std::function<void()> onComplete;
    std::function<void(f32)> onUpdate;
    
protected:
    virtual void onAnimate(f32 progress) = 0;
    
    f32 m_duration;
    f32 m_currentTime = 0;
    f32 m_speed = 1.0f;
    State m_state = State::Idle;
    bool m_loop = false;
    bool m_reverse = false;
    bool m_pingPong = false;
    bool m_reversing = false;
};

// Property animation for animating any numeric value
template<typename T>
class PropertyAnimation : public Animation {
public:
    PropertyAnimation(T* target, T from, T to, f32 duration)
        : Animation(duration), m_target(target), m_from(from), m_to(to) {}
    
    void setEasing(Easing::Function easing) { m_easing = easing; }
    
protected:
    void onAnimate(f32 progress) override {
        if (m_target && m_easing) {
            f32 t = m_easing(progress);
            *m_target = m_from + (m_to - m_from) * t;
        }
    }
    
private:
    T* m_target;
    T m_from;
    T m_to;
    Easing::Function m_easing = Easing::linear;
};

// Timeline for orchestrating multiple animations
class Timeline : public Object {
public:
    Timeline();
    ~Timeline();
    
    // Add animations
    void add(Ref<Animation> animation, f32 startTime = 0);
    void addSequential(Ref<Animation> animation); // Add after previous
    void addParallel(Ref<Animation> animation);   // Add at same time as previous
    
    // Timeline control
    void play();
    void pause();
    void stop();
    void seek(f32 time);
    
    // Update
    void update(f32 deltaTime);
    
    // Properties
    f32 duration() const;
    f32 currentTime() const { return m_currentTime; }
    bool isPlaying() const { return m_playing; }
    
    // Callbacks
    std::function<void()> onComplete;
    
private:
    struct AnimationEntry {
        Ref<Animation> animation;
        f32 startTime;
    };
    
    std::vector<AnimationEntry> m_animations;
    f32 m_currentTime = 0;
    f32 m_lastSequentialTime = 0;
    bool m_playing = false;
};

} // namespace Aurora