#pragma once

#include "VEInclude.h"
#include "VPE.hpp"
#include "../Helper.hpp"
#include "../eventlisteners/VEEventListenerPhysics.hpp"
#include "../eventlisteners/VEEventListenerPhysicsKeys.hpp"
#include "../eventlisteners/VEEventListenerPhysicsGUI.hpp"
#include <vector>
#include <unordered_set>

using namespace vpe;
using namespace ve::eventlisteners;

extern bool g_gameLost;        
extern bool isMoving;
extern bool eggPlaced;
extern bool knockback;

extern std::shared_ptr<VPEWorld::Body> myDuck;
extern std::vector<std::shared_ptr<VPEWorld::Body>> knockbackDucks;
extern glmvec3 lastValidPos;

extern std::stack<Pair> myPath;

extern std::vector<Level> levels;

extern std::vector<std::shared_ptr<VPEWorld::Body>> eggs;
extern std::shared_ptr<VPEWorld::Body> currentEgg;

extern const std::vector<Pair> fixedPlayerPositions;

extern std::unordered_map<std::shared_ptr<VPEWorld::Body>, std::stack<Pair>> enemyDucks;
extern std::unordered_map<std::shared_ptr<VPEWorld::Body>, glmvec3> enemyDuckPositions;

extern double g_time;
extern double g_knockback;

extern std::unordered_set<std::shared_ptr<VPEWorld::Body>> objectsToDelete;

class MyVulkanEngine : public VEEngine {
public:
    VPEWorld m_physics; 

    VEEventListenerPhysics* m_physics_listener;
    VEEventListenerPhysicsKeys* m_physics_listener_keys;
    VEEventListenerPhysicsGUI* m_physics_listener_gui;

    MyVulkanEngine(veRendererType type = veRendererType::VE_RENDERER_TYPE_FORWARD, bool debug = false);

    virtual void registerEventListeners();
    virtual void loadLevel(int numLevel);
    void setupGame();
    void createCameraAndScene();
    void initLevels();
    void spawnMap();
    void spawnObstacles();
    void initEggs();
    void spawnPlayers();
    void spawnEnemyPlayers();
    void advanceToNextLevel();
};
