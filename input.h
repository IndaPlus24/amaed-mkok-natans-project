#ifndef H_INPUT
#define H_INPUT

// value & 0b01 == 0b01 <=> is down
// value & 0b10 == 0b10 <=> state just changed
enum class ButtonState
{
    // 0b00
    Up = 0,
    // 0b01
    Down = 1,
    // 0b10
    Released = 2,
    // 0b11
    Pressed = 3,
};

// The input
typedef struct Inputs
{
    ButtonState up;
    ButtonState down;
    ButtonState left;
    ButtonState right;
    ButtonState a;
    ButtonState b;
    ButtonState c;
    ButtonState x;
    ButtonState y;
    ButtonState z;
    ButtonState start;
    ButtonState select;
} Inputs;

/// @brief Get the relevant inputs for the current frame
/// @return The relevant inputs for the current fram
Inputs GetInputs();
#endif