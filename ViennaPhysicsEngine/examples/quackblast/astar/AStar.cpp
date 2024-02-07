#include "AStar.hpp"
#include <iostream>
#include <set>
#include <cmath>
#include <algorithm>
#include <stack>

bool AStar::isValid(int col, int row) {
    	return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
    }
    
bool AStar::isBlocked(int grid[][COL], int col, int row) {
    if(grid[row][col] == 1) {
        return false;
    }
    return true;
}
    
bool AStar::isDestination(int col, int row, Pair dest) {
    return (col == dest.first) && (row == dest.second);
}
    
// Use Distance Formula to get the Heuristic
double AStar::calculateHeuristicValue(int col, int row, Pair dest) {
    return ((double)sqrt((col - dest.first) * (col - dest.first) + (row - dest.second) * (row - dest.second)));
}
    
void AStar::tracePath(AStar::Node node[][COL], Pair dest, std::stack<Pair>& path) {
    int row = dest.first;
    int col = dest.second;
        
    while (!(node[row][col].xCoord == row && node[row][col].yCoord == col)) {
        path.push(Pair(row, col));
        int temp_row = node[row][col].xCoord;
        int temp_col = node[row][col].yCoord;
        row = temp_row;
        col = temp_col;
    }

    path.push(Pair(row, col));

}
    
std::stack<Pair> AStar::aStarSearch(int grid[][COL], Pair src, Pair dest, std::stack<Pair>& path) {
    if (!isValid(src.first, src.second) || !isValid(dest.first, dest.second)) {
        std::cout << "Source or destination is invalid\n";
        return std::stack<Pair>();
    }
    
    if (isBlocked(grid, src.first, src.second) || isBlocked(grid, dest.first, dest.second)) {
        std::cout << "Source or destination is blocked\n";
        return std::stack<Pair>();
    }
    
    if (isDestination(src.first, src.second, dest)) {
        std::cout << "We are already at the destination\n";
        return std::stack<Pair>();
    }
    
    bool closedList[ROW][COL];
    memset(closedList, false, sizeof(closedList));
    
    AStar::Node node[ROW][COL];
    
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            node[i][j].f = FLT_MAX;
            node[i][j].cost = FLT_MAX;
            node[i][j].heuristic = FLT_MAX;
            node[i][j].xCoord = -1;
            node[i][j].yCoord = -1;
        }
    }
    
    int colVal = src.first, rowVal = src.second;
    node[colVal][rowVal].f = 0.0;
    node[colVal][rowVal].cost = 0.0;
    node[colVal][rowVal].heuristic = 0.0;
    node[colVal][rowVal].xCoord = colVal;
    node[colVal][rowVal].yCoord = rowVal;
    
    std::set<DoublePair> openList;
    
    openList.insert(DoublePair(0.0, Pair(colVal, rowVal)));
    
    bool foundDest = false;
    
    while (!openList.empty()) {
        DoublePair p = *openList.begin();
    
        openList.erase(openList.begin());
    
        colVal = p.second.first;
        rowVal = p.second.second;
        closedList[colVal][rowVal] = true;
    
        double gNew, hNew, fNew;
    
        // Successors in different directions
        int successors[4][2] = { {-1, 0}, {1, 0}, {0, 1}, {0, -1} };
    
        for (int k = 0; k < 4; k++) {
            int nColVal = colVal + successors[k][0];
            int nRowVal = rowVal + successors[k][1];
    
            if (isValid(nColVal, nRowVal) && !closedList[nColVal][nRowVal] && !isBlocked(grid, nColVal, nRowVal)) {
                if (isDestination(nColVal, nRowVal, dest)) {
                    node[nColVal][nRowVal].xCoord = colVal;
                    node[nColVal][nRowVal].yCoord = rowVal;
                    std::cout << "The destination is found\n";
                    AStar::tracePath(node, dest, path);
                    foundDest = true;
                    return std::stack<Pair>();
                } else {
                    gNew = node[colVal][rowVal].cost + ((k < 4) ? 1.0 : 1.414);
                    hNew = calculateHeuristicValue(nColVal, nRowVal, dest);
                    fNew = gNew + hNew;
    
                    if (node[nColVal][nRowVal].f == FLT_MAX || node[nColVal][nRowVal].f > fNew) {
                        openList.insert(DoublePair(fNew, Pair(nColVal, nRowVal)));
    
                        node[nColVal][nRowVal].f = fNew;
                        node[nColVal][nRowVal].cost = gNew;
                        node[nColVal][nRowVal].heuristic = hNew;
                        node[nColVal][nRowVal].xCoord = colVal;
                        node[nColVal][nRowVal].yCoord = rowVal;
                    }
                }
            }
        }
    }
    
    if (!foundDest) {
        std::cout << "Failed to find the Destination\n";
    }
 
    return path;
}
