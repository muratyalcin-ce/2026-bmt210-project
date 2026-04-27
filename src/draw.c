#include "draw.h"
#include <raylib.h>

void DrawCanvas(Canvas *canvas)
{
    DrawTexture(canvas->texture, 0, 0, WHITE);
}
