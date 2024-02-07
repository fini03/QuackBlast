#pragma once

#include "VEInclude.h"
#include "VPE.hpp"
#include "../Helper.hpp"
#include <stack>

using namespace vpe;

extern bool isMoving;
extern bool eggPlaced;
extern double g_time;
extern std::shared_ptr<VPEWorld::Body> myDuck;
extern std::vector<std::shared_ptr<VPEWorld::Body>> eggs;
extern std::shared_ptr<VPEWorld::Body> currentEgg;
extern glmvec3 lastValidPos;
extern std::stack<Pair> myPath;
extern std::vector<Level> levels;
extern int currentLevel;

namespace ve::eventlisteners {

    class VEEventListenerPhysicsKeys : public VEEventListener {
    protected:
        bool onKeyboard(veEvent event);
        void placeRandomEgg();

        VPEWorld* m_physics; // Pointer to the physics world

    public:
        VEEventListenerPhysicsKeys(std::string name, VPEWorld* physics);
        virtual ~VEEventListenerPhysicsKeys();
    };
} // namespace ve::eventlisteners
