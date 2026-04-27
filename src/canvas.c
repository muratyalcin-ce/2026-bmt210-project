#include "canvas.h"
#include <stdlib.h>

void InitCanvas(Canvas *canvas, int width, int height)
{
    canvas->width = width;
    canvas->height = height;

    canvas->pixels = (Color *) malloc(width * height * sizeof(Color));

    ClearCanvas(canvas, RAYWHITE);

    canvas->image.data = canvas->pixels;
    canvas->image.width = width;
    canvas->image.height = height;
    canvas->image.mipmaps = 1;
    canvas->image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    canvas->texture = LoadTextureFromImage(canvas->image);
	canvas->dirty = true;  
}

void ClearCanvas(Canvas *canvas, Color color)
{
    for (int i = 0; i < canvas->width * canvas->height; i++)
    {
        canvas->pixels[i] = color;
    }
}

void SetPixel(Canvas *canvas, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= canvas->width || y >= canvas->height)
        return;

    canvas->pixels[y * canvas->width + x] = color;
	canvas->dirty = true;
}

Color GetPixel(Canvas *canvas, int x, int y)
{
    return canvas->pixels[y * canvas->width + x];
}

void DestroyCanvas(Canvas *canvas)
{
    free(canvas->pixels);
}

void UpdateCanvasTexture(Canvas *canvas)
{
    UpdateTexture(canvas->texture, canvas->pixels);
}
