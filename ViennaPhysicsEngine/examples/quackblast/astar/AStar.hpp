#pragma once

#include <algorithm>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/fwd.hpp>
#include <initializer_list>
#include <math.h>
#include <memory>
#include <vector>
#include <cstdio>
#include <iterator>
#include <ranges>
#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <cfloat>
#include <set>
#include <stack>
#include <cstring>
#include "glm/glm.hpp"

#include "VEInclude.h"
#include "VESceneManager.h"
#include "irrKlang.h"

#define GLM_FORCE_LEFT_HANDED
#include "glm/glm.hpp"
#include "glm/gtx/matrix_operation.hpp"
#include "glm/gtx/matrix_cross_product.hpp"

#include "VPE.hpp"
#include "../Helper.hpp"

namespace AStar {
    struct Node {
        // Define the Node structure
        int xCoord;
        int yCoord;
        double f;
        double cost;
        double heuristic;
    };

    // Function declarations related to A* algorithm
    bool isValid(int col, int row);
    bool isBlocked(int grid[][COL], int col, int row);
    bool isDestination(int col, int row, Pair dest);
    double calculateHeuristicValue(int col, int row, Pair dest);
    void tracePath(Node node[][COL], Pair dest, std::stack<Pair>& path);
    std::stack<Pair> aStarSearch(int grid[][COL], Pair src, Pair dest, std::stack<Pair>& path);
}
