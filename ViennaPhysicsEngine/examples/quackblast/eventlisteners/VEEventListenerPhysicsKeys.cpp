#include "VEEventListenerPhysicsKeys.hpp"
#include "VEEntity.h"
#include "VEEventListenerPhysics.hpp"
#include "VESceneManager.h"

using namespace ve::eventlisteners;

VEEventListenerPhysicsKeys::VEEventListenerPhysicsKeys(std::string name, VPEWorld* physics)
    : VEEventListener(name), m_physics{ physics } {}

VEEventListenerPhysicsKeys::~VEEventListenerPhysicsKeys() {}

bool VEEventListenerPhysicsKeys::onKeyboard(veEvent event) {
    if (event.idata3 == GLFW_PRESS) {
        if (!isMoving) {
            glm::vec3 position = myDuck->m_positionW;
            roundPosition(position);

            switch (event.idata1) {
                case GLFW_KEY_W: // Move forward
                    myPath.push(Pair(position.x, position.z - 1.0f));
                    myDuck->m_on_move = moveWithKeyboard;
                    isMoving = true;

                    // Rotate the duck to face forward
                    myDuck->m_orientationLW = glm::quat(glm::vec3(0.0f, glm::pi<float>(), 0.0f));
                    break;
                case GLFW_KEY_S: // Move backward
                    myPath.push(Pair(position.x, position.z + 1.0f));
                    myDuck->m_on_move = moveWithKeyboard;
                    isMoving = true;

                    // Rotate the duck to face backward
                    myDuck->m_orientationLW = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
                    break;
                case GLFW_KEY_A: // Move left
                    myPath.push(Pair(position.x + 1.0f, position.z));
                    myDuck->m_on_move = moveWithKeyboard;
                    isMoving = true;

                    // Rotate the duck to face left
                    myDuck->m_orientationLW = glm::quat(glm::vec3(0.0f, glm::half_pi<float>(), 0.0f));
                    break;
                case GLFW_KEY_D: // Move right
                    myPath.push(Pair(position.x - 1.0f, position.z));
                    myDuck->m_on_move = moveWithKeyboard;
                    isMoving = true;

                    // Rotate the duck to face right
                    myDuck->m_orientationLW = glm::quat(glm::vec3(0.0f, -glm::half_pi<float>(), 0.0f));
                    break;
                case GLFW_KEY_SPACE: // poop out an egg
                    if(!eggPlaced) {
                        placeRandomEgg();
                        eggPlaced = true;
                    }
                    break;
                case GLFW_KEY_R: // Rotate in the direction of the clock
                    myDuck->m_orientationLW = glm::rotate(myDuck->m_orientationLW, glm::half_pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
                    myDuck->m_on_move = onMove;
                    break;
            }
        }
    }
    return true;
}

void VEEventListenerPhysicsKeys::placeRandomEgg() {
    std::random_device rd; 
    std::mt19937 gen(rd());

    // Generate a random index within the range of the vector size
    std::uniform_int_distribution<> dis(0, eggs.size() - 1);
    int randomIndex = dis(gen);
    currentEgg = eggs[randomIndex];

    // Set position of the currentEgg
    currentEgg->m_positionW = myDuck->m_positionW;

    // Adjust position based on duck's orientation
    glm::vec3 eggOffset = glm::rotate(myDuck->m_orientationLW, glm::vec3(0.0f, 0.0f, 1.0f));
    currentEgg->m_positionW += eggOffset;
    roundPosition(currentEgg->m_positionW);

    // Check if the target position is valid (not on a border or obstacle)
    int targetRow = static_cast<int>(currentEgg->m_positionW.z);
    int targetCol = static_cast<int>(currentEgg->m_positionW.x);

    if (targetRow >= 0 && targetRow < ROW && targetCol >= 0 && targetCol < COL && (levels[currentLevel].grid[targetRow][targetCol] != 0 && levels[currentLevel].grid[targetRow][targetCol] != 2)) {
        // If the target position is valid, add the egg to the physics simulation
        m_physics->addBody(currentEgg);
        currentEgg->m_on_move = onEggMove;
        currentEgg->m_on_erase = onHideErase;
        getSceneManagerPointer()->setVisibility(static_cast<VESceneNode*>(currentEgg->m_owner), true);
        eggPlaced = true;
    } else {
        // If the target position is not valid, we'll simply discard the egg
        std::cout << "Can't place egg here\n";
        eggPlaced = false;
        g_time = 4.0;
    }
}
