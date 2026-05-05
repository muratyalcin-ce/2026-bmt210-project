#ifndef LAYER_H
#define LAYER_H

#include "stack.h"

typedef struct Layer {
    char name[32];

    Stack undoStrokes;
    Stack redoStrokes;

    int visible;

    struct Layer* parent;
    struct Layer* child;   // first child
    struct Layer* sibling; // next layer at same level
} Layer;

Layer* CreateLayer(const char* name);
Layer* AddChildLayer(Layer* parent, const char* name);
void DrawLayerTree(Layer* layer);
Layer* GetNextLayer(Layer* current, Layer* root);
Layer* GetPreviousLayer(Layer* current, Layer* root);
Layer* AddSiblingLayer(Layer* current, const char* name);
void PrintLayerTree(Layer* layer, int depth);
void ToggleAllLayerVisibility(Layer* layer);

#endif // LAYER_H
