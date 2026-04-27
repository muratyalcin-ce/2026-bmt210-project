#ifndef INPUT_H
#define INPUT_H

#include "canvas.h"
#include "app.h"

void HandleInput(App *app);
void drawCircle(Canvas *canvas,int xc, int yc, int x, int y, Color c);
void hollowCircle(Canvas *canvas, int xc, int yc, int r, Color c);
void fullCircle(Canvas *canvas, int xc, int yc, int r, Color c);

#endif
