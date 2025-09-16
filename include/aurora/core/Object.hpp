// ============================================
// include/aurora/core/Object.hpp
// ============================================
#pragma once
#include "Types.hpp"
#include <atomic>
#include <vector>

namespace Aurora {

class Object : public std::enable_shared_from_this<Object> {
public:
    Object();
    virtual ~Object();
    
    // Reference counting
    void retain();
    void release();
    u32 retainCount() const { return m_refCount; }
    
    // Parent-child hierarchy
    void setParent(Object* parent);
    Object* parent() const { return m_parent; }
    const std::vector<Object*>& children() const { return m_children; }
    
    // Object name for debugging
    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }
    
protected:
    virtual void onParentChanged(Object* oldParent) {}
    
private:
    std::atomic<u32> m_refCount{1};
    Object* m_parent = nullptr;
    std::vector<Object*> m_children;
    std::string m_name;
};

} // namespace Aurora