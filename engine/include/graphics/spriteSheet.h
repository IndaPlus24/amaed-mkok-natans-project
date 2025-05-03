#include <raylib.h>

/// @brief This thing makes it easy to draw a specific section of a texture representing an animation. The standard is rows are different directions, columns are different frames
typedef struct SpriteSheet
{
    Texture texture;
    int directions;
    int frames;
    int width;
    int height;
} SpriteSheet;

/// @brief What do you think it does?
/// @param fileName
/// @param directions
/// @param frames
/// @return
SpriteSheet LoadSpriteSheet(const char *fileName, int directions, int frames);

/// @brief What do you think it does?
/// @param SpriteSheet
void UnloadSpriteSheet(SpriteSheet *SpriteSheet);

/// @brief Draw with x y as the top left of the sprite
/// @param sheet
/// @param direction
/// @param frame
/// @param x
/// @param y
void Draw(const SpriteSheet *sheet, int direction, int frame, Vector2 pos);

/// @brief Draw with x y as the centre of the sprite
/// @param sheet
/// @param direction
/// @param frame
/// @param x
/// @param y
void DrawCentre(const SpriteSheet *sheet, int direction, int frame, Vector2 pos);