#include "layer.h"
#include "stroke.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Layer* CreateLayer(const char* name)
{
    Layer* l = (Layer*) malloc(sizeof(Layer));

    strcpy(l->name, name);
    InitStack(&l->undoStrokes);
    InitStack(&l->redoStrokes);

    l->visible = 1;

    l->parent = NULL;
    l->child = NULL;
    l->sibling = NULL;

    return l;
}

Layer* AddChildLayer(Layer* parent, const char* name)
{
    if (!parent) return NULL;

    Layer* child = CreateLayer(name);
    child->parent = parent;

    if (!parent->child)
    {
        parent->child = child;
    }
    else
    {
        Layer* cur = parent->child;
        while (cur->sibling)
            cur = cur->sibling;

        cur->sibling = child;
    }

    return child;
}

Layer* AddSiblingLayer(Layer* current, const char* name)
{
    if (!current) return NULL;

    Layer* newLayer = CreateLayer(name);

    newLayer->parent = current->parent;

    // Insert after current
    newLayer->sibling = current->sibling;
    current->sibling = newLayer;

    return newLayer;
}


void DrawLayerTree(Layer* layer)
{
    if (!layer) return;

    if (layer->visible)
    {
        DrawStrokesFromStack(&layer->undoStrokes);
    }

    DrawLayerTree(layer->child);
    DrawLayerTree(layer->sibling);
}

Layer* GetNextLayer(Layer* current, Layer* root)
{
    if (!current) return root;

    // 1. Go to child
    if (current->child)
        return current->child;

    // 2. Go to sibling
    if (current->sibling)
        return current->sibling;

    // 3. Climb up to find a parent with sibling
    Layer* p = current->parent;

    while (p)
    {
        if (p->sibling)
            return p->sibling;

        p = p->parent;
    }

    // 4. Wrap around → first real layer
    return root->child ? root->child : root;
}

Layer* GetPreviousLayer(Layer* current, Layer* root)
{
    if (!current || current == root)
        return root;

    // Find previous sibling
    Layer* parent = current->parent;

    if (!parent) return root;

    Layer* prev = parent->child;

    if (prev == current)
        return parent;

    while (prev && prev->sibling != current)
        prev = prev->sibling;

    // Go to deepest child of prev
    while (prev->child)
    {
        prev = prev->child;
        while (prev->sibling)
            prev = prev->sibling;
    }

    return prev;
}

void PrintLayerTree(Layer* layer, int depth)
{
    if (!layer) return;

    for (int i = 0; i < depth; i++)
        printf("  ");

    printf("- %s\n", layer->name);

    PrintLayerTree(layer->child, depth + 1);
    PrintLayerTree(layer->sibling, depth);
}

void ToggleAllLayerVisibility(Layer* layer)
{
    if (!layer) return;

    layer->visible = !layer->visible;

    ToggleAllLayerVisibility(layer->child);
    ToggleAllLayerVisibility(layer->sibling);
}
