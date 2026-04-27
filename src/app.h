#ifndef APP_H
#define APP_H

#include "stroke.h"
#include "stack.h"

typedef struct {
    StrokeList strokes;
    Stroke* currentStroke;

    Stack undoStack;
    Stack redoStack;
} App;


void InitApp(App *app, int width, int height);
void UpdateApp(App *app);
void RenderApp(App *app);
void DestroyApp(App *app);

#endif
