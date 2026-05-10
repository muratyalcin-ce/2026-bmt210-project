#include "layer_palette.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

void InitLayerPalette(LayerPalette* palette, int screenWidth)
{
    palette->layerCount = 0;
    palette->hoveredLayerIndex = -1;
    palette->hoveredChildIndex = -1;
    palette->showChildMenu = false;
    memset(palette->layerButtons, 0, sizeof(palette->layerButtons));

    palette->bounds.x = 0;
    palette->bounds.y = (float)GetScreenHeight() - LAYER_BUTTON_HEIGHT - LAYER_BAR_PADDING;
    palette->bounds.width = (float)screenWidth;
    palette->bounds.height = (float)LAYER_BUTTON_HEIGHT + LAYER_BAR_PADDING * 2;
}

void ResizeLayerPalette(LayerPalette* palette, int screenWidth)
{
    palette->bounds.width = (float)screenWidth;
    palette->bounds.y = (float)GetScreenHeight() - LAYER_BUTTON_HEIGHT - LAYER_BAR_PADDING;
}

static int CollectChildLayers(Layer* parent, Layer** children, int maxCount)
{
    int count = 0;
    if (!parent) return 0;

    Layer* child = parent->child;
    while (child && count < maxCount)
    {
        children[count++] = child;
        child = child->sibling;
    }
    return count;
}

static int CollectRootLayers(Layer* root, Layer** layers, int maxCount)
{
    int count = 0;
    if (!root) return 0;

    Layer* current = root->child;
    while (current && count < maxCount)
    {
        layers[count++] = current;
        current = current->sibling;
    }
    return count;
}

void UpdateLayerPaletteButtons(LayerPalette* palette, Layer* root)
{
    Layer* rootLayers[MAX_VISIBLE_LAYERS];
    palette->layerCount = CollectRootLayers(root, rootLayers, MAX_VISIBLE_LAYERS);

    float startX = LAYER_BAR_PADDING;
    float buttonY = palette->bounds.y + LAYER_BAR_PADDING;

    for (int i = 0; i < palette->layerCount; i++)
    {
        palette->layerButtons[i].x = startX;
        palette->layerButtons[i].y = buttonY;
        palette->layerButtons[i].width = (float)LAYER_BUTTON_WIDTH;
        palette->layerButtons[i].height = (float)LAYER_BUTTON_HEIGHT;
        startX += LAYER_BUTTON_WIDTH + LAYER_BAR_PADDING;
    }
}

bool UpdateLayerPalette(LayerPalette* palette, Layer* root, Layer** activeLayer)
{
    bool changed = false;
    Vector2 mouse = GetMousePosition();

    UpdateLayerPaletteButtons(palette, root);
    if (palette->layerCount == 0) return false;

    Layer* rootLayers[MAX_VISIBLE_LAYERS];
    CollectRootLayers(root, rootLayers, MAX_VISIBLE_LAYERS);

    int newHoveredIndex = -1;
    for (int i = 0; i < palette->layerCount; i++)
    {
        if (CheckCollisionPointRec(mouse, palette->layerButtons[i]))
        {
            newHoveredIndex = i;
            break;
        }
    }

    // Hover değiştiyse
    if (newHoveredIndex != palette->hoveredLayerIndex)
    {
        palette->hoveredLayerIndex = newHoveredIndex;
        palette->showChildMenu = (newHoveredIndex != -1);
        palette->hoveredChildIndex = -1;
    }

    bool mouseOverChildMenu = false;
    int childCountForMenu = 0;
    Layer* childrenForMenu[MAX_VISIBLE_LAYERS] = {NULL};
    Rectangle childRects[MAX_VISIBLE_LAYERS];

    if (palette->showChildMenu && palette->hoveredLayerIndex != -1)
    {
        Layer* selectedLayer = rootLayers[palette->hoveredLayerIndex];
        childCountForMenu = CollectChildLayers(selectedLayer, childrenForMenu, MAX_VISIBLE_LAYERS);

        if (childCountForMenu > 0)
        {
            Rectangle button = palette->layerButtons[palette->hoveredLayerIndex];
            float menuX = button.x;
            float menuY = button.y - (childCountForMenu * (LAYER_BUTTON_HEIGHT + 2)) - CHILD_MENU_OFFSET;

            if (menuX + LAYER_BUTTON_WIDTH > GetScreenWidth())
                menuX = GetScreenWidth() - LAYER_BUTTON_WIDTH - LAYER_BAR_PADDING;
            if (menuY < 0)
                menuY = button.y + LAYER_BUTTON_HEIGHT + CHILD_MENU_OFFSET;

            for (int i = 0; i < childCountForMenu; i++)
            {
                childRects[i] = (Rectangle){
                    menuX,
                    menuY + i * (LAYER_BUTTON_HEIGHT + 2),
                    (float)LAYER_BUTTON_WIDTH,
                    (float)LAYER_BUTTON_HEIGHT
                };

                if (CheckCollisionPointRec(mouse, childRects[i]))
                {
                    mouseOverChildMenu = true;
                    palette->hoveredChildIndex = i;
                    break;
                }
            }

            float menuHeight = childCountForMenu * (LAYER_BUTTON_HEIGHT + 2) + 4;
            Rectangle menuBg = { menuX - 4, menuY - 4, LAYER_BUTTON_WIDTH + 8, menuHeight + 8 };
            if (!mouseOverChildMenu && CheckCollisionPointRec(mouse, menuBg))
            {
                mouseOverChildMenu = true;
            }
        }
    }

    if (palette->showChildMenu && !mouseOverChildMenu && palette->hoveredLayerIndex == -1)
    {
        palette->showChildMenu = false;
        palette->hoveredChildIndex = -1;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Önce alt menüdeki child'lara bak
        if (mouseOverChildMenu && palette->hoveredChildIndex != -1 && palette->hoveredLayerIndex != -1)
        {
            *activeLayer = childrenForMenu[palette->hoveredChildIndex];
            changed = true;
            palette->showChildMenu = false;
            palette->hoveredLayerIndex = -1;
            palette->hoveredChildIndex = -1;
            return changed;
        }

        // Sonra ana butonlara bak (HER ZAMAN)
        for (int i = 0; i < palette->layerCount; i++)
        {
            if (CheckCollisionPointRec(mouse, palette->layerButtons[i]))
            {
                *activeLayer = rootLayers[i];
                changed = true;
                palette->showChildMenu = false;
                palette->hoveredLayerIndex = -1;
                palette->hoveredChildIndex = -1;
                return changed;
            }
        }
    }

    return changed;
}

void DrawLayerPalette(LayerPalette* palette, Layer* root, Layer* activeLayer)
{
    if (palette->layerCount == 0) return;

    Layer* rootLayers[MAX_VISIBLE_LAYERS];
    CollectRootLayers(root, rootLayers, MAX_VISIBLE_LAYERS);

    // Arka plan
    DrawRectangleRec(palette->bounds, (Color){30, 30, 40, 220});
    DrawRectangleLinesEx(palette->bounds, 1, (Color){80, 80, 100, 255});

    // Ana butonlar
    for (int i = 0; i < palette->layerCount; i++)
    {
        Rectangle btn = palette->layerButtons[i];
        Color btnColor;

        if (rootLayers[i] == activeLayer)
            btnColor = (Color){60, 120, 200, 255};
        else if (i == palette->hoveredLayerIndex)
            btnColor = (Color){80, 80, 120, 255};
        else
            btnColor = (Color){50, 50, 70, 200};

        DrawRectangleRec(btn, btnColor);
        DrawRectangleLinesEx(btn, 1, (Color){120, 120, 150, 255});

        // Buton metni
        char displayName[32];
        snprintf(displayName, sizeof(displayName), "%s", rootLayers[i]->name);
        DrawText(displayName, (int)btn.x + 4, (int)btn.y + 8, 12, (Color){220, 220, 240, 255});

        // Göz ikonu
        Color eyeColor = rootLayers[i]->visible ? (Color){150, 220, 150, 255} : (Color){150, 150, 150, 180};
        DrawCircle((int)(btn.x + btn.width - 12), (int)(btn.y + btn.height/2), 6, eyeColor);
    }

    // Alt menü
    if (palette->showChildMenu && palette->hoveredLayerIndex != -1)
    {
        Layer* selectedLayer = rootLayers[palette->hoveredLayerIndex];
        Layer* children[MAX_VISIBLE_LAYERS];
        int childCount = CollectChildLayers(selectedLayer, children, MAX_VISIBLE_LAYERS);

        if (childCount > 0)
        {
            Rectangle button = palette->layerButtons[palette->hoveredLayerIndex];
            float menuX = button.x;
            float menuY = button.y - (childCount * (LAYER_BUTTON_HEIGHT + 2)) - CHILD_MENU_OFFSET;

            if (menuX + LAYER_BUTTON_WIDTH > GetScreenWidth())
                menuX = GetScreenWidth() - LAYER_BUTTON_WIDTH - LAYER_BAR_PADDING;
            if (menuY < 0)
                menuY = button.y + LAYER_BUTTON_HEIGHT + CHILD_MENU_OFFSET;

            // Menü arka planı
            float menuHeight = childCount * (LAYER_BUTTON_HEIGHT + 2) + 4;
            Rectangle menuBg = { menuX - 2, menuY - 2, LAYER_BUTTON_WIDTH + 4, menuHeight };
            DrawRectangleRec(menuBg, (Color){20, 20, 30, 240});
            DrawRectangleLinesEx(menuBg, 1, (Color){100, 100, 130, 255});

            for (int i = 0; i < childCount; i++)
            {
                Rectangle childRect = {
                    menuX,
                    menuY + i * (LAYER_BUTTON_HEIGHT + 2),
                    (float)LAYER_BUTTON_WIDTH,
                    (float)LAYER_BUTTON_HEIGHT
                };

                Color childColor;
                if (children[i] == activeLayer)
                    childColor = (Color){60, 120, 200, 255};
                else if (i == palette->hoveredChildIndex)
                    childColor = (Color){80, 80, 120, 255};
                else
                    childColor = (Color){45, 45, 60, 220};

                DrawRectangleRec(childRect, childColor);
                DrawRectangleLinesEx(childRect, 1, (Color){100, 100, 140, 200});
                DrawText(children[i]->name, (int)childRect.x + 4, (int)childRect.y + 8, 11, (Color){200, 200, 220, 255});

                Color eyeColorChild = children[i]->visible ? (Color){130, 200, 130, 200} : (Color){130, 130, 130, 160};
                DrawCircle((int)(childRect.x + childRect.width - 12), (int)(childRect.y + childRect.height/2), 5, eyeColorChild);
            }
        }
    }
}
