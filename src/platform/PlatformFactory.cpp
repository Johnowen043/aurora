// ============================================
// src/platform/PlatformFactory.cpp
// ============================================
#include "aurora/platform/IPlatform.hpp"

namespace Aurora {

Unique<IPlatform> IPlatform::create() {
#ifdef AURORA_PLATFORM_X11
    return std::make_unique<X11Platform>();
#elif defined(AURORA_PLATFORM_WAYLAND)
    // return std::make_unique<WaylandPlatform>();
#else
    #error "No platform implementation available"
#endif
    return nullptr;
}

} // namespace Aurora

