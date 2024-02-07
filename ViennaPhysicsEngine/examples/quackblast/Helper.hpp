#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include "VEInclude.h"
#include "VESceneManager.h"
#include "VPE.hpp"
#include <memory>
#include <stack>
#include <vector>
#include <unordered_set>

using namespace vpe;
using namespace ve;

#define ROW 19
#define COL 19

using Pair = std::pair<int, int>;
using DoublePair = std::pair<double, Pair>;

struct Level {
    int grid[ROW][COL];
    Pair nextLevelPosition;
};

extern bool g_gameLost;
extern bool isMoving;
extern bool eggPlaced;
extern bool knockback;
extern std::shared_ptr<VPEWorld::Body> myDuck;
extern std::vector<std::shared_ptr<VPEWorld::Body>> eggs;
extern std::vector<std::shared_ptr<VPEWorld::Body>> knockbackDucks;
extern std::shared_ptr<VPEWorld::Body> currentEgg;
extern glmvec3 lastValidPos;
extern std::stack<Pair> myPath;
extern std::unordered_map<std::shared_ptr<VPEWorld::Body>, std::stack<Pair>> enemyDucks;
extern std::unordered_map<std::shared_ptr<VPEWorld::Body>, glmvec3> enemyDuckPositions;
extern std::vector<Level> levels;
extern int currentLevel;
extern std::unordered_set<std::shared_ptr<VPEWorld::Body>> objectsToDelete;
extern double g_knockback;

static float roundToNearestInt(float value) {
    return std::round(value);
}

static void roundPosition(glm::vec3& position) {
    position.x = roundToNearestInt(position.x);
    position.y = 0.5f;
    position.z = roundToNearestInt(position.z);
}

static void updateGrid(int row, int col) {
    if (row >= 0 && row < ROW && col >= 0 && col < COL) {
        if (levels[currentLevel].grid[col][row] == 2) {
            levels[currentLevel].grid[col][row] = 1;  
        }
    }
};


static VPEWorld::callback_move onMove = [](double dt, std::shared_ptr<VPEWorld::Body> body) {
    // Implementation for onMove callback
    VESceneNode* cube = static_cast<VESceneNode*>(body->m_owner);								// Owner is a pointer to a scene node
	glmvec3 pos = body->m_positionW;															// New position of the scene node
	glmquat orient = body->m_orientationLW;														// New orientation of the scende node
	body->stepPosition(dt, pos, orient, false);													// Extrapolate
  	cube->setTransform(VPEWorld::Body::computeModel(pos, orient, body->m_scale));
};

static VPEWorld::callback_move onEggMove = [](double dt, std::shared_ptr<VPEWorld::Body> body) {
    onMove(dt, body); 
    getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(currentEgg->m_owner), true);
};

static VPEWorld::callback_move moveWithKeyboard = [](double dt, std::shared_ptr<VPEWorld::Body> body) {
    // Implementation for moveWithKeyboard callback
    if(myPath.empty()) {
        body->m_linear_velocityW = glmvec3(0);
        isMoving = false;
        return;
    }
	onMove(dt, body);
    Pair targetCoordinate = myPath.top();
    glmvec3 targetPosition = glmvec3(targetCoordinate.first, 0.5f, targetCoordinate.second);
    auto targetDir = targetPosition - body->m_positionW;
    if(glm::length(targetDir) < 0.1) {
        roundPosition(targetPosition);
        body->m_positionW = targetPosition;
        lastValidPos = body->m_positionW;
        myPath.pop();
        isMoving = false;
    } else {
        body->m_linear_velocityW = glm::normalize(targetDir) * 0.03_real * 100.0f;
        isMoving = true;
    }
};

static VPEWorld::callback_move moveToDst = [](double dt, std::shared_ptr<VPEWorld::Body> body) { 
    // Retrieve the enemyPath for the current duck
    auto it = std::find_if(enemyDucks.begin(), enemyDucks.end(),
        [body](const auto& entry) { return entry.first == body; });

    if (it == enemyDucks.end()) {
        // Duck not found in the map, handle the error as needed
        return;
    }

    auto& enemyPath = it->second;

    if (enemyPath.empty()) {
        body->m_linear_velocityW = glmvec3(0);
        return;
    }

    onMove(dt, body);
    Pair targetCoordinate = enemyPath.top();
    glmvec3 targetPosition = glmvec3(targetCoordinate.first, 0.5f, targetCoordinate.second);
    auto targetDir = targetPosition - body->m_positionW;

    // Calculate the angle between the current orientation and the target direction
    float angle = atan2(targetDir.x, targetDir.z);
    // Create a quaternion representing the rotation around the vertical axis (Y-axis)
    glm::quat rotationQuat = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply the rotation to the duck's orientation
    body->m_orientationLW = rotationQuat;

    if(glm::length(targetDir) < 0.1) {
        roundPosition(targetPosition);
        body->m_positionW = glmvec3(targetCoordinate.first, 0.5f, targetCoordinate.second);
        enemyDuckPositions[body] = body->m_positionW;
        enemyPath.pop();
    } else {
        body->m_linear_velocityW = glm::normalize(targetDir) * 0.01_real * 100.0f;
    }
};

static VPEWorld::callback_erase onErase = [](std::shared_ptr<VPEWorld::Body> body) { 
    VESceneNode* node = static_cast<VESceneNode*>(body->m_owner);								// Owner is a pointer to a scene node
	getSceneManagerPointer()->deleteSceneNodeAndChildren(
            ((VESceneNode*)body->m_owner)->getName());
};

static VPEWorld::callback_erase onHideErase = [](std::shared_ptr<VPEWorld::Body> body) { 
    getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(body->m_owner), false);
};

static VPEWorld::callback_collide onCollide = [](std::shared_ptr<VPEWorld::Body> body1, std::shared_ptr<VPEWorld::Body> body2) {
    if (body1->m_name == "MyDuck" && (body2->m_name.starts_with("Cube") || body2->m_name.starts_with("Obstacles") || body2->m_name.find("Egg") != std::string::npos)) {
        roundPosition(lastValidPos);
        body1->m_positionW = lastValidPos;
        if(!myPath.empty()) myPath.pop();
    }
    return;
};

static VPEWorld::callback_collide onEnemyDuckCollision = [](std::shared_ptr<VPEWorld::Body> body1, std::shared_ptr<VPEWorld::Body> body2) {
    if(body1->m_name.starts_with("Enemy") && body2->m_name == "MyDuck") {
        g_gameLost = true;
        for (const auto& [enemyDuck, _] : enemyDucks) {
            enemyDuck->m_linear_velocityW = glmvec3(0);  
        }
    }
};

static VPEWorld::callback_collide onEggCollision = [](std::shared_ptr<VPEWorld::Body> body1, std::shared_ptr<VPEWorld::Body> body2) {
    if(body1->m_name.starts_with("Explosive") && (body2->m_name.starts_with("Enemy") || body2->m_name == "MyDuck" || body2->m_name.starts_with("Obstacles") || body2->m_name.starts_with("Cube"))) {
        if(body2->m_name == "MyDuck") {
            g_gameLost = true;
            body1->m_linear_velocityW = glmvec3(0.0f);
            getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(body1->m_owner), false);
            objectsToDelete.insert(body2);
            return;
        }
        if(body2->m_name.starts_with("Cube")) {
            roundPosition(body2->m_positionW);
            body1->m_linear_velocityW = glmvec3(0.0f);
            getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(body1->m_owner), false);
            objectsToDelete.insert(body1);
            return;
        }

        if(body2->m_name.starts_with("Enemy")) {

            body2->m_on_move = onMove;
            knockback = true;
            knockbackDucks.push_back(body2); 
            return;
        }
        
        if(body2->m_name.starts_with("Obstacles")) {
            roundPosition(body2->m_positionW);
            updateGrid(body2->m_positionW.x, body2->m_positionW.z);
            body1->m_linear_velocityW = glmvec3(0.0f);
            getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(body2->m_owner), false);
            objectsToDelete.insert(body2);
        }
    }
};
