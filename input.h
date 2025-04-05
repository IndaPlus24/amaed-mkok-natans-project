#ifndef H_INPUT
#define H_INPUT

// value & 1 == 1 <=> is down
// value & 2 == 2 <=> state just changed
enum class ButtonState {
    Up = 0,
    Down = 1,
    Released = 2,
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


// Get the inputs
Inputs GetInputs();
#endif