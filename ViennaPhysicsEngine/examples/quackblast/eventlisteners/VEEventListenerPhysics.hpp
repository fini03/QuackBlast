#pragma once

#include "VEInclude.h"
#include "VPE.hpp"
#include "../Helper.hpp"
#include "../astar/AStar.hpp"
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <unordered_set>

using namespace vpe;

extern bool g_gameLost;
extern double g_time;
extern double g_knockback;
extern bool eggPlaced;
extern std::shared_ptr<VPEWorld::Body> myDuck;
extern std::unordered_map<std::shared_ptr<VPEWorld::Body>, std::stack<Pair>> enemyDucks;
extern std::vector<std::shared_ptr<VPEWorld::Body>> knockbackDucks;
extern std::vector<Level> levels;
extern int currentLevel;
extern std::unordered_set<std::shared_ptr<VPEWorld::Body>> objectsToDelete;

namespace ve::eventlisteners {

    class VEEventListenerPhysics : public VEEventListener {
    protected:
        void onFrameStarted(veEvent event);
        void removeEgg();
        void shootEggOneUnit(const glm::vec3& targetCoordinate, const glmvec3& position, const glmvec3& direction);
        void shootEgg(const glmvec3& position, const glmvec3& direction);
        std::shared_ptr<VPEWorld::Body> createEgg();

        VPEWorld* m_physics; // Pointer to the physics world

    public:
        VEEventListenerPhysics(std::string name, VPEWorld* physics);
        virtual ~VEEventListenerPhysics();
    };

} // namespace ve::eventlisteners
