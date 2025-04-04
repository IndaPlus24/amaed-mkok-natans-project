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

Inputs GetInputs()
{
       
    Inputs inputs;

    ButtonState inList[6];

    for (int i = 0; i < 6; i++)
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

    inputs = *((Inputs *)&inList);

    return inputs;
}
