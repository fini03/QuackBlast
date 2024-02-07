#pragma once

#include "VEInclude.h"
#include "VPE.hpp"

using namespace vpe;
extern bool g_gameLost;
extern bool g_restart;
extern double g_time;

namespace ve::eventlisteners {

    class VEEventListenerPhysicsGUI : public VEEventListener {
    protected:
        virtual void onDrawOverlay(veEvent event);

        VPEWorld* m_physics; // Pointer to the physics world

    public:
        VEEventListenerPhysicsGUI(std::string name, VPEWorld* physics);
        virtual ~VEEventListenerPhysicsGUI();
    };

} // namespace ve::eventlisteners

