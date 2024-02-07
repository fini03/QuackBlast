#include "VEEventListenerPhysics.hpp"
#include "../game/MyVulkanEngine.hpp"
#include "VEEventListenerPhysicsGUI.hpp"
#include "VESceneManager.h"
#include "VPE.hpp"
#include <memory>
#include <random>
#include <string>
#include <vector>

using namespace ve::eventlisteners;
extern MyVulkanEngine mve;

VEEventListenerPhysics::VEEventListenerPhysics(std::string name, vpe::VPEWorld* physics)
    : VEEventListener(name), m_physics{ physics } {}

VEEventListenerPhysics::~VEEventListenerPhysics() {}


std::default_random_engine rnd_gen{ 12345 };					//Random numbers
std::uniform_real_distribution<> rnd_unif{ 0.0f, 1.0f };		//Random numbers
int eggNum = 0;

void VEEventListenerPhysics::onFrameStarted(veEvent event) {
    for (auto& enemyDuckEntry : enemyDucks) {
        auto& enemyDuck = enemyDuckEntry.first;
        auto& enemyPath = enemyDuckEntry.second;

        if (enemyPath.empty()) {
            Pair myDuckPos = Pair(myDuck->m_positionW.x + 0.5f, myDuck->m_positionW.z + 0.5f);
            Pair startPos = Pair(enemyDuck->m_positionW.x + 0.5f, enemyDuck->m_positionW.z + 0.5f);
            AStar::aStarSearch(levels[currentLevel].grid, startPos, myDuckPos, enemyPath);

            if(enemyPath.size() < 2) continue;

            enemyPath.pop();
            Pair nextCoordinate = enemyPath.top();
            enemyPath = std::stack<Pair>();
            enemyPath.push(nextCoordinate);

            enemyDuck->m_on_move = moveToDst;
        }
    }

    glmvec3 position = myDuck->m_positionW;
    roundPosition(position);

    if(position.x == levels[currentLevel].nextLevelPosition.first && position.z == levels[currentLevel].nextLevelPosition.second) mve.advanceToNextLevel();
    
    if(eggPlaced) g_time -= event.dt;
    if(knockback) g_knockback -=event.dt;

    if(eggPlaced && g_time <= 0) {
        removeEgg();
    }

    if(g_knockback <= 0) {
        g_knockback = 2.0f;
        knockback = false;
        for(auto& duck : knockbackDucks) {
            duck->m_on_move = moveToDst;
            objectsToDelete.insert(duck);
        }
    }
     
    // Update physics for all ducks
    m_physics->tick(event.dt);

    for(const auto& eachBody : objectsToDelete) {
        getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(eachBody->m_owner), false);
        m_physics->eraseBody(eachBody);
    }

    objectsToDelete.clear();
}

void VEEventListenerPhysics::removeEgg() {
    eggPlaced = false;

    // Get the position of the current egg
    glmvec3 eggPosition = currentEgg->m_positionW;
    roundPosition(eggPosition);

    // Create cubes and shoot them in different directions
    if (currentEgg->m_name == "RedEgg") {
        // Shoot only one unit in the specified direction
        shootEggOneUnit(eggPosition + glmvec3(1.0f, 0.0f, 0.0f), eggPosition, glm::vec3(1.0f, 0.0f, 0.0f));  // Shoot right
        shootEggOneUnit(eggPosition - glmvec3(1.0f, 0.0f, 0.0f), eggPosition, glm::vec3(-1.0f, 0.0f, 0.0f)); // Shoot left
        shootEggOneUnit(eggPosition - glmvec3(0.0f, 0.0f, 1.0f), eggPosition, glm::vec3(0.0f, 0.0f, -1.0f));  // Shoot up
        shootEggOneUnit(eggPosition + glmvec3(0.0f, 0.0f, 1.0f), eggPosition, glm::vec3(0.0f, 0.0f, 1.0f)); // Shoot down


    } else if (currentEgg->m_name == "BlueEgg") {
        // Shoot in every row
        shootEgg(eggPosition, glm::vec3(1.0f, 0.0f, 0.0f));  // Shoot right
        shootEgg(eggPosition, glm::vec3(-1.0f, 0.0f, 0.0f)); // Shoot left
    } else if (currentEgg->m_name == "GreenEgg") {
        // Shoot in every column
        shootEgg(eggPosition, glm::vec3(0.0f, 0.0f, -1.0f));  // Shoot up
        shootEgg(eggPosition, glm::vec3(0.0f, 0.0f, 1.0f)); // Shoot down
    } else if (currentEgg->m_name == "YellowEgg") {
        // Shoot in every row and column
        shootEgg(eggPosition, glm::vec3(1.0f, 0.0f, 0.0f));  // Shoot right
        shootEgg(eggPosition, glm::vec3(-1.0f, 0.0f, 0.0f)); // Shoot left
        shootEgg(eggPosition, glm::vec3(0.0f, 0.0f, -1.0f));  // Shoot up
        shootEgg(eggPosition, glm::vec3(0.0f, 0.0f, 1.0f)); // Shoot down
    }
 
    objectsToDelete.insert(currentEgg);
    g_time = 4.0;
}

void VEEventListenerPhysics::shootEggOneUnit(const glm::vec3& targetCoordinates, const glm::vec3& position, const glm::vec3& direction) {
    auto egg = createEgg();

    // Set the force for the red egg to move one unit in the specified direction
    egg->setForce(0ul, VPEWorld::Force{ {1.0f * direction.x, 0.0, 1.0f * direction.z} });

    egg->m_positionW = position;
    egg->m_linear_velocityW = 0.03f * direction;

    // Add the cube to the physics simulation
    m_physics->addBody(egg);

    // Register the onMove callback for the red egg
    egg->m_on_move = [targetCoordinates](double dt, std::shared_ptr<VPEWorld::Body> body) {
        onMove(dt, body);
		glmvec3 forceDir = glm::normalize(targetCoordinates - body->m_positionW);
        auto force = -body->m_linear_velocityW * 0.01f;
        body->m_linear_velocityW += forceDir + force;
        
       if (glm::length(targetCoordinates - body->m_positionW) < 0.1f) {
            // Remove the cube from the physics simulation
            body->m_on_move = nullptr;
            getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(body->m_owner), false);
            objectsToDelete.insert(body);
        }
    };
}

void VEEventListenerPhysics::shootEgg(const glmvec3& position, const glm::vec3& direction) {
    auto egg = createEgg();

    // Adjust the shooting direction based on the color of the egg
    if (currentEgg->m_name == "BlueEgg") {
        // Shoot in every row
        egg->setForce(0ul, VPEWorld::Force{ {50.0f * direction.x, 0.0f, 0.0f} });
    } else if (currentEgg->m_name == "GreenEgg") {
        // Shoot in every column
        egg->setForce(0ul, VPEWorld::Force{ {0.0f, 0.0f, 50.0f * direction.z} });
    } else if (currentEgg->m_name == "YellowEgg") {
        // Shoot in every row and column
        egg->setForce(0ul, VPEWorld::Force{ {50.0f * direction.x, 0.0f, 50.0f * direction.z} });
    }

    egg->m_positionW = position;
    egg->m_linear_velocityW = 0.03f * direction;

    // Add the cube to the physics simulation
    m_physics->addBody(egg);
}

std::shared_ptr<VPEWorld::Body> VEEventListenerPhysics::createEgg() {
    std::string eggColor = currentEgg->m_name;
    std::string modelFileName;

    if (eggColor == "RedEgg") {
        modelFileName = "red_egg.obj";
    } else if (eggColor == "BlueEgg") {
        modelFileName = "blue_egg.obj";  // Adjust the filename accordingly
    } else if (eggColor == "GreenEgg") {
        modelFileName = "green_egg.obj";  // Adjust the filename accordingly
    } else {
        modelFileName = "yellow_egg.obj";  // Adjust the filename accordingly
    }

    glmvec3 positionCamera{ getSceneManagerPointer()->getSceneNode("StandardCameraParent")->getWorldTransform()[3] };
	glmvec3 dir{ getSceneManagerPointer()->getSceneNode("StandardCamera")->getWorldTransform()[2] };
	glmvec3 vel = (30.0_real + 5.0_real * (real)rnd_unif(rnd_gen)) * dir / glm::length(dir);
	glmvec3 scale{ 0.15,0.15,0.15 }; // = rnd_unif(rnd_gen) * 10;
	VESceneNode* cube;
   	VECHECKPOINTER(cube = getSceneManagerPointer()->loadModel("Explosive" + std::to_string(eggNum), "../../media/models/test/egg", modelFileName, 0, getRoot()));
	auto body = std::make_shared<VPEWorld::Body>(
        m_physics,													// Pointer to the physics world
        "Explosive" + std::to_string(eggNum),                                            // Name of the body
        cube,														// Pointer of the owner
        &m_physics->g_cube,					    					// Pointer of the Polytop 
        scale,														// 3D scaling 
        positionCamera + 2.0_real * dir,							// 3D position of the body 
        glm::quat(1, 0, 0, 0),		                    			// Body orientation as quarernion
        vel,														// Velocity
        glm::vec3(0.0f),										// Angular velocity 
        0.1,									            	// Mass
        m_physics->m_restitution,									// Restitution
        0.0f										// Friction
    );
    body->m_on_move = onMove;
    body->m_on_erase = onHideErase;
    m_physics->addCollider(body, onEggCollision);
    //getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(body->m_owner), false);
    eggNum++;
    return body;
}
