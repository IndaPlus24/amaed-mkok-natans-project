#include <raylib.h>
#include <raymath.h>

#include "spriteSheet.h"

SpriteSheet LoadSpriteSheet(const char *fileName, int directions, int frames)
{
    SpriteSheet sheet;
    sheet.directions = directions;
    sheet.frames = frames;
    sheet.texture = LoadTexture(fileName);
    sheet.height = sheet.texture.height / directions;
    sheet.width = sheet.texture.width / frames;

    return sheet;
};

void UnloadSpriteSheet(SpriteSheet *SpriteSheet)
{
    UnloadTexture(SpriteSheet->texture);
}

void Draw(const SpriteSheet *sheet, int direction, int frame, Vector2 pos)
{
    Rectangle source;
    source.x = frame * sheet->width;
    source.y = direction * sheet->height;
    source.width = sheet->width;
    source.height = sheet->height;
    pos.x = floorf(pos.x);
    pos.y = floorf(pos.y);
    DrawTextureRec(sheet->texture, source, pos, WHITE);
}

void DrawCentre(const SpriteSheet *sheet, int direction, int frame, Vector2 pos)
{
    Draw(sheet, direction, frame, Vector2Add(pos, Vector2{(float)-(sheet->width >> 1), (float)-(sheet->height >> 1)}));
}