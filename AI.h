#ifndef AI_H
#define AI_H

#include "raylib.h"  

const int GRID_WIDTH = 100;
const int GRID_HEIGHT = 100;

// Structure for each cell in the navigation grid
struct GridCell {
    bool walkable;  // Set to false for obstacles.
    float cost;     // The "distance" cost from this cell to the player's position.
};

// Global navigation grid declaration.
extern GridCell grid[GRID_WIDTH][GRID_HEIGHT];

// Compute the flow field/ distance field from the player's grid position.
void ComputeFlowField(int playerGridX, int playerGridY);

// Get the normalized direction for an enemy located at (gridX, gridY) based on the flow field.
Vector2 GetFlowFieldDirection(int gridX, int gridY);

#endif // AI_H
