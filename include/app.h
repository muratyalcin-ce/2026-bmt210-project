#ifndef APP_H
#define APP_H

#include "stroke.h"
#include "stack.h"
#include "hashmap.h"
#include "layer.h"
#include "layer_palette.h"

typedef struct {
    Layer* root;
    Layer* activeLayer;

    Stroke* currentStroke;

    HashMap tools;
    Tool* currentTool;

    LayerPalette palette;
    int layerCounter;
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
