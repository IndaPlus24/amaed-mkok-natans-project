#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

typedef struct Inputs
{
    bool upDown;
    bool downDown;
    bool leftDown;
    bool rightDown;
    bool eDown;
} Inputs;

Inputs GetInputs()
{
    Inputs inputs = {0};
    inputs.upDown = IsKeyDown(KEY_W);
    inputs.downDown = IsKeyDown(KEY_S);
    inputs.leftDown = IsKeyDown(KEY_A);
    inputs.rightDown = IsKeyDown(KEY_D);
    inputs.eDown = IsKeyPressed(KEY_E);
    return inputs;
}

