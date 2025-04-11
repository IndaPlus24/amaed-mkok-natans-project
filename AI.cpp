#include "AI.h"
#include <queue>
#include <limits>
#include <cmath>

// An extremly large value to represent infinity.
// This is used to initialize the cost of each cell in the grid.
const float INF = std::numeric_limits<float>::max();

// The actual grid is defined here.
GridCell grid[GRID_WIDTH][GRID_HEIGHT];

// A simple node structure used by Dijkstra’s algorithm.
struct Node {
    int x, y;
    float distance;
    // For the priority queue, we compare nodes by distance.
    bool operator>(const Node &other) const {
        return distance > other.distance;
    }
};

// 4-directional movement (up, down, left, right)
const int dx[4] = { 1, -1,  0,  0 };
const int dy[4] = { 0,  0,  1, -1 };

// Compute the flow field from the player's grid position using a reverse Dijkstra's algorithm.
void ComputeFlowField(int playerGridX, int playerGridY)
{
    // Initialize the cost for every cell to infinity.
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            grid[i][j].cost = INF;
        }
    }
    
    // Priority queue to process the cells.
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
    
    // Set the player's cell cost to 0.
    grid[playerGridX][playerGridY].cost = 0;
    open.push({playerGridX, playerGridY, 0});
    
    
    while (!open.empty())
    {
        Node current = open.top();
        open.pop();
        
        // Skip if we already found a better cost.
        if (current.distance > grid[current.x][current.y].cost)
            continue;
        
        // Process each neighbor.
        for (int dir = 0; dir < 4; ++dir)
        {
            int nx = current.x + dx[dir];
            int ny = current.y + dy[dir];
            
            // Check grid bounds and if the cell is walkable.
            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT && grid[nx][ny].walkable)
            {
                // Assume a uniform cost of 1 per step.
                float newCost = current.distance + 1.0f;
                if (newCost < grid[nx][ny].cost)
                {
                    grid[nx][ny].cost = newCost;
                    open.push({nx, ny, newCost});
                }
            }
        }
    }
}

// Return the normalized direction vector for an enemy at grid coordinates (gridX, gridY)
// by finding the neighbor cell with the lowest cost.
Vector2 GetFlowFieldDirection(int gridX, int gridY,)
{

    float bestCost = grid[gridX][gridY].cost;
    int bestX = gridX, bestY = gridY;

    // Look for the neighbor with the smallest cost.
    for (int dir = 0; dir < 4; ++dir)
    {
        int nx = gridX + dx[dir];
        int ny = gridY + dy[dir];
        if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT && grid[nx][ny].walkable)
        {
            if (grid[nx][ny].cost < bestCost)
            {
                bestCost = grid[nx][ny].cost;
                bestX = nx;
                bestY = ny;
            }
        }
    }
    
    // Create a direction vector from the current cell to the chosen neighbor.
    Vector2 dir = { static_cast<float>(bestX - gridX), static_cast<float>(bestY - gridY) };
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 0)
    {
        dir.x /= len;
        dir.y /= len;
    }
    return dir;
}
