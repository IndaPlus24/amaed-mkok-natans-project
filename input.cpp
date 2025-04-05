#include <stdio.h>
#include <raylib.h>
#include "input.h"

#ifdef DRIFVARKADEN
// Drifvarkaden's button layout, for some reason
KeyboardKey keys[] = {
    KEY_UP,             // Up
    KEY_DOWN,           // Down
    KEY_LEFT,           // Left
    KEY_RIGHT,          // Right
    KEY_LEFT_ALT,       // (A)
    KEY_LEFT_CONTROL,   // (B)
    KEY_C,              // (C)
    KEY_LEFT_SHIFT,     // (X)
    KEY_SPACE,          // (Y)
    KEY_Z,              // (Z)
    KEY_H,              // START
    KEY_Y               // SELECT
};

// Drifvarkaden's player 2 button layout, for some reason
KeyboardKey keysP2[] = {
    KEY_R,              // Up
    KEY_F,              // Down
    KEY_D,              // Left
    KEY_G,              // Right
    KEY_S,              // (A)
    KEY_A,              // (B)
    KEY_O,              // (C)
    KEY_Q,              // (X)
    KEY_W,              // (Y)
    KEY_I,              // (Z)
    KEY_J,              // START
    KEY_U               // SELECT
};
#else

// A button layout I made up on the spot.
KeyboardKey keys[] = {
    KEY_W,        // Up
    KEY_S,        // Down
    KEY_A,        // Left
    KEY_D,        // Right
    KEY_J,        // (A)
    KEY_K,        // (B)
    KEY_L,        // (C)
    KEY_U,        // (X)
    KEY_I,        // (Y)
    KEY_O,        // (Z)
    KEY_ENTER,    // START
    KEY_BACKSPACE // SELECT
};
#endif

Inputs ArrayToInputs(ButtonState array[12]) {
    Inputs inputs;
    inputs.up = array[0];
    inputs.down = array[1];
    inputs.left = array[2];
    inputs.right = array[3];
    inputs.a = array[4];
    inputs.b = array[5];
    inputs.c = array[6];
    inputs.x = array[7];
    inputs.y = array[8];
    inputs.z = array[9];
    inputs.start = array[10];
    inputs.select = array[11];
    return inputs;
}

Inputs GetInputs()
{
    ButtonState inList[12];

    for (int i = 0; i < 12; i++)
    {
        KeyboardKey key = keys[i];
        if (IsKeyReleased(key))
        {
            inList[i] = ButtonState::Released;
        }
        else if (IsKeyUp(key))
        {
            inList[i] = ButtonState::Up;
        }
        else if (IsKeyPressed(key))
        {
            inList[i] = ButtonState::Pressed;
        }
        else
        {
            inList[i] = ButtonState::Down;
        }
    }
    
    return ArrayToInputs(inList);
}
