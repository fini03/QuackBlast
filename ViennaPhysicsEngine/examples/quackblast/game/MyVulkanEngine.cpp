#include "MyVulkanEngine.hpp"
#include "VEEntity.h"
#include <cstdlib>
#include <stack>

MyVulkanEngine::MyVulkanEngine(veRendererType type, bool debug) : VEEngine(type, debug) {}

void MyVulkanEngine::registerEventListeners() {
    registerEventListener(m_physics_listener = new eventlisteners::VEEventListenerPhysics(
        "Physics", &m_physics), { veEvent::VE_EVENT_FRAME_STARTED });
	registerEventListener(m_physics_listener_keys = new eventlisteners::VEEventListenerPhysicsKeys(
		"Physics Keys", &m_physics), { veEvent::VE_EVENT_KEYBOARD });
	registerEventListener(m_physics_listener_gui = new eventlisteners::VEEventListenerPhysicsGUI(
        "Physics GUI", &m_physics), { veEvent::VE_EVENT_DRAW_OVERLAY });
}

void MyVulkanEngine::loadLevel(int numLevel) {
    VEEngine::loadLevel(numLevel);			
    setupGame();    
}

void MyVulkanEngine::setupGame() {
    createCameraAndScene();
    initLevels();
    spawnMap();
    spawnObstacles();
    spawnPlayers();
    spawnEnemyPlayers();
    initEggs();
}

void MyVulkanEngine::createCameraAndScene() {

VESceneNode *pScene;																	// Get Root Node
	VECHECKPOINTER( pScene =
	getSceneManagerPointer()->createSceneNode("Level 1", getRoot()) );
	
	// Scene models
	VESceneNode *sp1;
    VECHECKPOINTER( sp1 = getSceneManagerPointer()->createSkybox( "The Sky", "../../media/models/test/sky/cloudy",
                { "bluecloud_ft.jpg", "bluecloud_bk.jpg", "bluecloud_up.jpg", "bluecloud_dn.jpg",
				"bluecloud_rt.jpg", "bluecloud_lf.jpg" }, pScene) );

    VESceneNode *e4;
    VECHECKPOINTER( e4 = getSceneManagerPointer()->loadModel("The Plane", "../../media/models/test/plane", "plane_t_n_s.obj",0, pScene) );
	e4->setTransform(glm::scale( glm::translate( glm::vec3{ 0,0,0,}),
	glm::vec3(1000.0f, 1.0f, 1000.0f)));

	VEEntity *pE4;
	VECHECKPOINTER( pE4 = (VEEntity*)getSceneManagerPointer()->getSceneNode("The Plane/plane_t_n_s.obj/plane/Entity_0") );
	pE4->setParam( glm::vec4(1000.0f, 1000.0f, 0.0f, 0.0f) );

	getSceneManagerPointer()->getSceneNode("StandardCameraParent")->setPosition({0,1,-4});

    glmvec3 positionCamera{ getSceneManagerPointer()->getSceneNode("StandardCameraParent")->getWorldTransform()[3] };
    getSceneManagerPointer()->getSceneNode("StandardCameraParent")->lookAt(glmvec3(9.5f, 26.5f, 9.5f), glmvec3(9.5f, 0.0f, 9.5f), glmvec3(0, 0, -1));
}

void MyVulkanEngine::initLevels() {

    levels.push_back({
        {
            {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
            {0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 0}, // 1
            {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 2
            {0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 0}, // 3
            {0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 1, 0}, // 4
            {0, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0}, // 5
            {0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 6
            {0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1, 0}, // 7
            {0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 8
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 0}, // 9
            {0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 1, 0}, // 10
            {0, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 11
            {0, 2, 0, 2, 0, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 12
            {0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 0}, // 13
            {0, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 1, 0, 2, 0}, // 14
            {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0}, // 15
            {0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 1, 0, 1, 0}, // 16
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0}, // 17 pos of myduck
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 18
            //0 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
        },  
            {1, 0}

    });

    levels.push_back({
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 1
            {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 2
            {0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 0}, // 3
            {0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 1, 0}, // 4
            {0, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0}, // 5
            {0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 6
            {0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1, 0}, // 7
            {0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 8
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 0}, // 9
            {0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 1, 0}, // 10
            {1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 11
            {0, 2, 0, 2, 0, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 12
            {0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 0}, // 13
            {0, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 1, 0, 2, 0}, // 14
            {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0}, // 15
            {0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 1, 0, 1, 0}, // 16
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0}, // 17 pos of myduck
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 18
            //0 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
        },
            {0, 11}

    }); 

    levels.push_back({
        {
            {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
            {0, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 0}, // 1
            {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0}, // 2
            {0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 0}, // 3
            {0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 1, 0}, // 4
            {0, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 0}, // 5
            {0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 6
            {0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1, 0}, // 7
            {0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 8
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 1, 0}, // 9
            {0, 2, 0, 1, 0, 2, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 1, 0}, // 10
            {0, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0}, // 11
            {0, 2, 0, 2, 0, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 1, 0}, // 12
            {0, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1, 0}, // 13
            {0, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 1, 0, 2, 0}, // 14
            {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 0}, // 15
            {0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 1, 0, 1, 0}, // 16
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 0}, // 17 pos of myduck
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // 18
            //0 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
        },
            {7, 0}

    }); 

}

void MyVulkanEngine::spawnMap() {
    VESceneNode* cube1;

    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            if (levels[currentLevel].grid[row][col] == 0) {
                VECHECKPOINTER(cube1 = getSceneManagerPointer()->loadModel("Cube" + std::to_string(row) + "_" + std::to_string(col), "../../media/models/test/crate0", "cube.obj", 0, getRoot()));

                glm::vec3 cubePosition(
                    static_cast<float>(col),
                    0.5f,
                    static_cast<float>(row)
                );


                auto cube = std::make_shared<VPEWorld::Body>(
                    &m_physics,
                    "Cube",
                    cube1,
                    &m_physics.g_cube,
                    glm::vec3(0.9f), // Scale of the cube
                    cubePosition,   // Cube position based on grid indices
                    glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
                    glm::vec3(0.0f), // Initial velocity
                    glm::vec3(0.0f), // Initial angular velocity
                    0.0_real,
                    0.0_real,
                    1.0_real
                );

                cube->m_on_move = onMove;
                cube->m_on_erase = onErase;
                m_physics.addBody(cube);
            } 
        }
    }
}

void MyVulkanEngine::spawnObstacles() {
    VESceneNode* cube1;

    for (int row = 0; row < ROW; ++row) {
        for (int col = 0; col < COL; ++col) {
            if (levels[currentLevel].grid[row][col] == 2) {
                VECHECKPOINTER(cube1 = getSceneManagerPointer()->loadModel("Obstacles" + std::to_string(row) + "-" + std::to_string(col), "../../media/models/test/crate1", "cube.obj", 0, getRoot()));

                glm::vec3 cubePosition(
                    static_cast<float>(col),
                    0.5f,
                    static_cast<float>(row)
                );

                auto cube = std::make_shared<VPEWorld::Body>(
                    &m_physics,
                    "Obstacles",
                    cube1,
                    &m_physics.g_cube,
                    glm::vec3(0.9f), // Scale of the cube
                    cubePosition,   // Cube position based on grid indices
                    glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
                    glm::vec3(0.0f), // Initial velocity
                    glm::vec3(0.0f), // Initial angular velocity
                    0.0_real,
                    0.0_real,
                    1.0_real
                );

                cube->m_on_move = onMove;
                cube->m_on_erase = onErase;
                m_physics.addBody(cube);
            } 
        }
    }
}

void MyVulkanEngine::initEggs() {
    VESceneNode* redEgg;
    VESceneNode* blueEgg;
    VESceneNode* greenEgg;
    VESceneNode* yellowEgg;

    VECHECKPOINTER(redEgg = getSceneManagerPointer()->loadModel("RedEgg", "../../media/models/test/egg", "red_egg.obj", 0, getRoot()));
    VECHECKPOINTER(blueEgg = getSceneManagerPointer()->loadModel("BlueEgg", "../../media/models/test/egg", "blue_egg.obj", 0, getRoot()));
    VECHECKPOINTER(greenEgg = getSceneManagerPointer()->loadModel("GreenEgg", "../../media/models/test/egg", "green_egg.obj", 0, getRoot()));
    VECHECKPOINTER(yellowEgg = getSceneManagerPointer()->loadModel("YellowEgg", "../../media/models/test/egg", "yellow_egg.obj", 0, getRoot()));

    glm::vec3 position(
        myDuck->m_positionW.x,
        0.5f,
        myDuck->m_positionW.z
    );

    auto red_egg = std::make_shared<VPEWorld::Body>(
        &m_physics,
        "RedEgg",
        redEgg,
        &m_physics.g_cube,
        glm::vec3(0.3f), // Scale of the cube
        position,   // Cube position based on grid indices
        glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
        glm::vec3(0.0f), // Initial velocity
        glm::vec3(0.0f), // Initial angular velocity
        0.0001_real,
        m_physics.m_restitution,
        1.0
    );
            
    auto blue_egg = std::make_shared<VPEWorld::Body>(
        &m_physics,
        "BlueEgg",
        blueEgg,
        &m_physics.g_cube,
        glm::vec3(0.3f), // Scale of the cube
        position,   // Cube position based on grid indices
        glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
        glm::vec3(0.0f), // Initial velocity
        glm::vec3(0.0f), // Initial angular velocity
        0.0001_real,
        m_physics.m_restitution,
        1.0
    );

    auto green_egg = std::make_shared<VPEWorld::Body>(
        &m_physics,
        "GreenEgg",
        greenEgg,
        &m_physics.g_cube,
        glm::vec3(0.3f), // Scale of the cube
        position,   // Cube position based on grid indices
        glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
        glm::vec3(0.0f), // Initial velocity
        glm::vec3(0.0f), // Initial angular velocity
        0.0001_real,
        m_physics.m_restitution,
        1.0
    );

    auto yellow_egg = std::make_shared<VPEWorld::Body>(
        &m_physics,
        "YellowEgg",
        yellowEgg,
        &m_physics.g_cube,
        glm::vec3(0.3f), // Scale of the cube
        position,   // Cube position based on grid indices
        glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
        glm::vec3(0.0f), // Initial velocity
        glm::vec3(0.0f), // Initial angular velocity
        0.0001_real,
        m_physics.m_restitution,
        1.0
    );

    getSceneManager()->setVisibility(redEgg, false);
    getSceneManager()->setVisibility(blueEgg, false);
    getSceneManager()->setVisibility(greenEgg, false);
    getSceneManager()->setVisibility(yellowEgg, false);

    eggs.push_back(red_egg);
    eggs.push_back(blue_egg);
    eggs.push_back(green_egg);
    eggs.push_back(yellow_egg);
}

void MyVulkanEngine::spawnPlayers() {
    VESceneNode* playerNode;
    VECHECKPOINTER(playerNode = getSceneManagerPointer()->loadModel("MyDuck", "../../media/models/test/duck", "duck_smol.obj", 0, getRoot()));

    glm::vec3 myposition(
        static_cast<float>(17),
        0.5f,
        static_cast<float>(17)
    );

    myDuck = std::make_shared<VPEWorld::Body>(
        &m_physics,
        "MyDuck",
        playerNode,
        &m_physics.g_cube,
        glm::vec3(0.7f), // Scale of the cube
        myposition,   // Cube position based on grid indices
        glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
        glm::vec3(0.0f), // Initial velocity
        glm::vec3(0.0f), // Initial angular velocity
        1.0_real,
        0.0,
        0.0
    );

    myDuck->m_on_move = onMove;
    myDuck->m_on_erase = onHideErase;
    m_physics.addBody(myDuck);
    m_physics.addCollider(myDuck, onCollide);
    myDuck->m_orientationLW = glm::quat(glm::vec3(0.0f, glm::pi<float>(), 0.0f));
    getSceneManager()->setVisibility(static_cast<VESceneNode*>(myDuck->m_owner), true);
}

void MyVulkanEngine::spawnEnemyPlayers() {
    VESceneNode* enemyNode;
    for(int i = 0; i < fixedPlayerPositions.size(); i++) {
        VECHECKPOINTER(enemyNode = getSceneManagerPointer()->loadModel("EnemyDuck" + std::to_string(i), "../../media/models/test/duck", "duck2_smol.obj", 0, getRoot()));

        glm::vec3 position(
            static_cast<float>(fixedPlayerPositions[i].first),
            0.5f,
            static_cast<float>(fixedPlayerPositions[i].second)
        );

        auto enemyDuck = std::make_shared<VPEWorld::Body>(
            &m_physics,
            "EnemyDuck" + std::to_string(i),
            enemyNode,
            &m_physics.g_cube,
            glm::vec3(0.7f), // Scale of the cube
            position,   // Cube position based on grid indices
            glm::quat(1, 0, 0, 0), // Quaternion for orientation (identity)
            glm::vec3(0.0f), // Initial velocity
            glm::vec3(0.0f), // Initial angular velocity
            1.0_real,
            0.0,
            0.0
        );


        enemyDuck->m_on_move = onMove;
        enemyDuck->m_on_erase = onHideErase;
        m_physics.addBody(enemyDuck);
        enemyDucks[enemyDuck] = std::stack<Pair>();
        m_physics.addCollider(enemyDuck, onEnemyDuckCollision);
        getSceneManager()->setVisibility(static_cast<VESceneNode*>(enemyDuck->m_owner), true);
    }    
}

void MyVulkanEngine::advanceToNextLevel() {
    if(currentLevel >= 2) {
        std::cout << "Congrats! You completed the game!\n";
        exit(0);
    }
    if(!g_gameLost) {
        glmvec3 position = myDuck->m_positionW;
        roundPosition(position);
        if(position.x == levels[currentLevel].nextLevelPosition.first && position.z == levels[currentLevel].nextLevelPosition.second) {
            currentLevel++;
            for(const auto& [enemyDuck, _] : enemyDucks) enemyDuck->m_on_erase = onErase;
            myDuck->m_on_erase = onErase;
            
            g_gameLost = false;        
            isMoving = false;
            eggPlaced = false;
            knockback = false;

            std::shared_ptr<VPEWorld::Body> myDuck = nullptr;
            lastValidPos = glmvec3(0.0f);

            myPath = std::stack<Pair>();
    
            levels.clear();

            eggs.clear();
            currentEgg = nullptr;

            enemyDucks.clear();
            enemyDuckPositions.clear();
            knockbackDucks.clear();

            g_time = 4.0f;
            g_knockback = 2.0f;


            m_physics.clear();  
            setupGame();
        }
    }
}
