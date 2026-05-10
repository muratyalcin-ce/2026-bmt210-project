#ifndef LAYER_PALETTE_H
#define LAYER_PALETTE_H

#include "raylib.h"
#include "layer.h"

#define MAX_VISIBLE_LAYERS 32
#define LAYER_BUTTON_WIDTH 80
#define LAYER_BUTTON_HEIGHT 30
#define LAYER_BAR_PADDING 8
#define CHILD_MENU_OFFSET 5

typedef struct {
    Rectangle bounds;
    Rectangle layerButtons[MAX_VISIBLE_LAYERS];
    int layerCount;

    int hoveredLayerIndex;
    int hoveredChildIndex;
    bool showChildMenu;

    float hoverTimer;
} LayerPalette;

void InitLayerPalette(LayerPalette* palette, int screenWidth);
void ResizeLayerPalette(LayerPalette* palette, int screenWidth);
void UpdateLayerPaletteButtons(LayerPalette* palette, Layer* root);
bool UpdateLayerPalette(LayerPalette* palette, Layer* root, Layer** activeLayer);
void DrawLayerPalette(LayerPalette* palette, Layer* root, Layer* activeLayer);

#endif