#ifndef CANVAS_H
#define CANVAS_H

#include <raylib.h>

typedef struct {
    int width;
    int height;
    Color *pixels;

	Image image;
    Texture2D texture;

	bool dirty;
} Canvas;

void InitCanvas(Canvas *canvas, int width, int height);
void ClearCanvas(Canvas *canvas, Color color);
void SetPixel(Canvas *canvas, int x, int y, Color color);
Color GetPixel(Canvas *canvas, int x, int y);
void DestroyCanvas(Canvas *canvas);
void UpdateCanvasTexture(Canvas *canvas);

#endif
