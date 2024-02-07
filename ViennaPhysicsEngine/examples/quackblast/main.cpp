#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "astar/AStar.hpp"
#include "Helper.hpp"
#include "game/MyVulkanEngine.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_LEFT_HANDED
#include "glm/glm.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include "glm/gtx/matrix_cross_product.hpp"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include "VPE.hpp"
#include "VEInclude.h"
#include "VESceneManager.h"
#include "irrKlang.h"

using namespace vpe;
using namespace eventlisteners;

bool g_gameLost = false;		
bool isMoving = false;
bool eggPlaced = false;
bool knockback = false;

std::shared_ptr<VPEWorld::Body> myDuck;
std::vector<std::shared_ptr<VPEWorld::Body>> knockbackDucks;
glmvec3 lastValidPos = glmvec3(0.0f);

std::stack<Pair> myPath;

std::vector<Level> levels;
int currentLevel = 0;

std::vector<std::shared_ptr<VPEWorld::Body>> eggs;
std::shared_ptr<VPEWorld::Body> currentEgg;

const std::vector<Pair> fixedPlayerPositions = {
    {1, 1}, {17, 1}, {4, 17}
};
std::unordered_map<std::shared_ptr<VPEWorld::Body>, std::stack<Pair>> enemyDucks;
std::unordered_map<std::shared_ptr<VPEWorld::Body>, glmvec3> enemyDuckPositions;

double g_time = 4.0;
double g_knockback = 2.0;

MyVulkanEngine mve(veRendererType::VE_RENDERER_TYPE_FORWARD);

std::unordered_set<std::shared_ptr<VPEWorld::Body>> objectsToDelete;

using namespace ve;

int main() {
	mve.initEngine();
	mve.loadLevel(currentLevel);
	mve.run();

	return 0;
}
