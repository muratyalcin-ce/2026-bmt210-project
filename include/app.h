#ifndef APP_H
#define APP_H

#include "stroke.h"
#include "stack.h"
#include "hashmap.h"
#include "layer.h"

typedef struct {
    Layer* root;
    Layer* activeLayer;

    Stroke* currentStroke;

    HashMap tools;
    Tool* currentTool;
} App;


void InitApp(App *app);
void UpdateApp(App *app);
void RenderApp(App *app);
void DestroyApp(App *app);
void AddChildLayerToApp(App *app);
void AddSiblingLayerToApp(App *app);
void ToggleActiveLayerVisibility(App *app);

void Undo(App *app);
void Redo(App *app);

#endif
