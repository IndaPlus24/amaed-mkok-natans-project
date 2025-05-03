#include "entityFunks.h"

bool EntityMove(Vector2 *pos, Vector2 move, int width, int height, GameData *gameData)
{
    bool stopX = false;

    // Horizontal
    if (move.x != 0)
    {
        int sign = move.x > 0 ? 1 : -1;
        float margin = sign * width / 2;

        int y0 = (int)((pos->y - height / 2) / tileSize);       // y start
        int y1 = (int)((pos->y + height / 2 - 1) / tileSize);   // y end
        int x0 = (int)((pos->x) / tileSize);                    // x start
        int x1 = (int)((pos->x + move.x + margin) / tileSize);  // x limit

        // Did it hit a wall?

        // loop from x0 to x1 (inclusive)
        for (int x = x0; x != x1 + sign; x += sign)
        {
            for (int y = y0; y <= y1; y++)
            {
                if (!GetTile(&gameData->currentRoom, x, y).isWalkable)
                {
                    stopX = true;
                    break;
                }
            }
            if (stopX)
            {
                int lastTile = x - sign; // The x of the tile they would have ended up in

                pos->x = (lastTile + (sign + 1) / 2) * tileSize - margin;
                break;
            }
        }

        if (!stopX)
        {
            pos->x += move.x;
        }
    }

    bool stopY = false;
    // Vertical
    if (move.y != 0)
    {
        int sign = move.y > 0 ? 1 : -1;
        float margin = sign * width / 2;

        int y0 = (int)((pos->y) / tileSize);                    // y start
        int y1 = (int)((pos->y + move.y + margin) / tileSize);  // y limit
        int x0 = (int)((pos->x - width / 2) / tileSize);        // x start
        int x1 = (int)((pos->x + width / 2 - 1) / tileSize);    // x end

        // Did it hit a wall?

        // loop from y0 to y1 (inclusive)
        for (int y = y0; y != y1 + sign; y += sign)
        {
            for (int x = x0; x <= x1; x++)
            {
                if (!GetTile(&gameData->currentRoom, x, y).isWalkable)
                {
                    stopY = true;
                }
            }
            if (stopY)
            {
                int lastTile = y - sign; // The y of the tile they would have ended up in

                pos->y = (lastTile + (sign + 1) / 2) * tileSize - margin;
                break;
            }
        }

        if (!stopY)
        {
            pos->y += move.y;
        }
    }

    return stopX || stopY;
}