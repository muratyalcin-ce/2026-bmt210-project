#include "app.h"
#include "input.h"
#include "stack.h"
#include "stroke.h"
#include "layer.h"
#include "layer_palette.h"

#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

#include "raygui.h"

void InitApp(App *app)
{
	
	app->root = CreateLayer("Root");
	app->currentStroke = NULL;
	app->layerCounter = 2;
	//app->activeLayer = app->root;

	Layer* layer1 = AddChildLayer(app->root, "Layer 1");
	Layer* layer2 = AddChildLayer(app->root, "Layer 2");

	Layer* child1 = AddChildLayer(layer1, "Child 1.1");
	Layer* child2 = AddChildLayer(layer1, "Child 1.2");

	app->activeLayer = layer1;

    InitHashMap(&app->tools);

    Tool brush = { TOOL_BRUSH, WHITE, 5 };
    Tool eraser = { TOOL_ERASER, BLACK, 10 };

    Insert(&app->tools, "brush", brush);
    Insert(&app->tools, "eraser", eraser);

    app->currentTool = Get(&app->tools, "brush");

    InitLayerPalette(&app->palette, GetScreenWidth());
}

void UpdateApp(App *app)
{
    HandleInput(app);

    UpdateLayerPalette(&app->palette, app->root, &app->activeLayer);

    static int lastWidth = 0;
    int currentWidth = GetScreenWidth();
    if (lastWidth != currentWidth)
    {
        ResizeLayerPalette(&app->palette, currentWidth);
        lastWidth = currentWidth;
    }
}

void RenderApp(App *app)
{
	float r = (float) app->currentTool->color.r;
	float g = (float) app->currentTool->color.g;
	float b = (float) app->currentTool->color.b;
	float a = (float) app->currentTool->color.a;
	ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    // Draw completed strokes
	DrawLayerTree(app->activeLayer);

	// Current tool name
	if (app->currentTool) {
		GuiLabel(
		(Rectangle){10, 0, 40,20},
			app->currentTool->type == TOOL_BRUSH ? "Brush" : "Eraser");
	}


	// Active layer name
	GuiLabel(
		(Rectangle){10, 20, 120,20},
		app->activeLayer->name);
	
	
	GuiLabel(
		(Rectangle){10, 40, 80, 20},
		app->activeLayer->visible? "Visible" : "Not Visible");
	
	
    // Draw current stroke (while drawing)
    if (app->currentStroke)
    {
        DrawSingleStroke(app->currentStroke);
    }

	// Cursor
	DrawCircleLines(GetMouseX(), GetMouseY(), app->currentTool->size, GRAY);
	
	// GUI
    if (GuiButton((Rectangle){10, 60, 80, 30}, GuiIconText(ICON_BRUSH_CLASSIC, "Brush")))
        app->currentTool = Get(&app->tools, "brush");

    if (GuiButton((Rectangle){10, 100, 80, 30}, GuiIconText(ICON_RUBBER,"Eraser")))
        app->currentTool = Get(&app->tools, "eraser");

    if (GuiButton((Rectangle){10, 140, 120, 30}, GuiIconText(ICON_LINK_MULTI, "Sibling Layer")))
    {
		AddSiblingLayerToApp(app);
    }

	if (GuiButton((Rectangle){10, 180, 120, 30}, GuiIconText(ICON_LINK_BOXES, "Child Layer"))){
		AddChildLayerToApp(app);
	}

	if (GuiButton((Rectangle) {10, 220, 20, 20}, GuiIconText(ICON_UNDO, ""))) {
		Undo(app);
	}
	
	if (GuiButton((Rectangle) {40, 220, 20, 20}, GuiIconText(ICON_REDO, ""))) {
		Redo(app);
	}

	if (GuiButton((Rectangle) {70, 220, 20, 20}, GuiIconText(ICON_EYE_OFF, ""))) {
		ToggleActiveLayerVisibility(app);
	}

	if (GuiSlider((Rectangle) {20, 250, 60, 20}, "R: ", "", &r, 0., 255.)){
		if(r != app->currentTool->color.r) app->currentTool->color.r = r;
	};
	
	if (GuiSlider((Rectangle) {20, 280, 60, 20}, "G: ", "", &g, 0., 255.)){
		if((char)g != app->currentTool->color.g) app->currentTool->color.g = g;
	};
	
	if (GuiSlider((Rectangle) {20, 310, 60, 20}, "B: ", "", &b, 0., 255.)){
		if((char)b != app->currentTool->color.b) app->currentTool->color.b = b;
	};
	
	if (GuiSlider((Rectangle) {20, 340, 60, 20}, "A: ", "", &a, 0., 255.)){
		if((char)a != app->currentTool->color.a) app->currentTool->color.a = a;
	};

	DrawLayerPalette(&app->palette, app->root, app->activeLayer);
}

void DestroyApp(App *app) {
	app->root = FreeLayer(app->root);
}

void AddChildLayerToApp(App *app) {
	Layer* child = AddChildLayer(app->activeLayer, "Child Layer");

	if (child)
	{
		app->activeLayer = child;
		UpdateLayerPaletteButtons(&app->palette, app->root);
	}
}

void AddSiblingLayerToApp(App *app) {
	static int layerCounter = 2;
	layerCounter++;

	char name[32];
	sprintf(name, "Layer %d", layerCounter);

	// Yeni layer oluştur
	Layer* newLayer = CreateLayer(name);
	newLayer->parent = app->root;

	// Root'un child'larının sonuna ekle (sıralı tutmak için)
	if (!app->root->child)
	{
		// Hiç child yoksa direkt ekle
		app->root->child = newLayer;
	}
	else
	{
		// Son child'ı bul
		Layer* last = app->root->child;
		while (last->sibling)
		{
			last = last->sibling;
		}
		last->sibling = newLayer;
	}

	app->activeLayer = newLayer;
	printf("Created layer: %s (Order preserved)\n", name);
	PrintLayerTree(app->root, 6);
	UpdateLayerPaletteButtons(&app->palette, app->root);
}

void ToggleActiveLayerVisibility(App *app) {
	if (app->activeLayer) {
		app->activeLayer->visible = !app->activeLayer->visible;

		printf("Layer %s visibility: %d\n",
			   app->activeLayer->name,
			   app->activeLayer->visible);
	}
}

void Undo(App *app) {
	Stroke* s = Pop(&app->activeLayer->undoStrokes);
	if (s) {
		Push(&app->activeLayer->redoStrokes, s);
	}
}

void Redo(App *app) {
	Stroke* s = Pop(&app->activeLayer->redoStrokes);
	if (s) {
		Push(&app->activeLayer->undoStrokes, s);
	}
}
