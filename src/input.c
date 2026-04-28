#include <raylib.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "layer.h"
#include "stroke.h"
#include "app.h"

extern StrokeList strokes;
extern Stroke* currentStroke;

void HandleInput(App *app)
{
    Vector2 mouse = GetMousePosition();
	
	if (IsKeyPressed(KEY_L))
	{
		app->activeLayer = GetNextLayer(app->activeLayer, app->root);
	}
	if (IsKeyPressed(KEY_K))
	{
		app->activeLayer = GetPreviousLayer(app->activeLayer, app->root);
	}

	if (IsKeyPressed(KEY_V))
	{
		if (app->activeLayer)
		{
			app->activeLayer->visible = !app->activeLayer->visible;

			printf("Layer %s visibility: %d\n",
				   app->activeLayer->name,
				   app->activeLayer->visible);
		}
	}

	if (IsKeyPressed(KEY_N))
	{
		static int layerCount = 1;

		char name[32];
		sprintf(name, "Layer %d", layerCount++);

		Layer* newLayer = AddSiblingLayer(app->activeLayer, name);

		if (newLayer)
		{
			app->activeLayer = newLayer;
			printf("Created layer: %s\n", name);
			PrintLayerTree(app->root, 6);
		}
	}
	
	if (IsKeyPressed(KEY_M)) // M for "make child"
	{
		Layer* child = AddChildLayer(app->activeLayer, "Child Layer");

		if (child)
			app->activeLayer = child;
	}

	if (IsKeyPressed(KEY_Z))
	{
		Stroke* s = Pop(&app->activeLayer->undoStrokes);
		if (s)
		{
			Push(&app->activeLayer->redoStrokes, s);
		}
	}
	
	if (IsKeyPressed(KEY_Y))
	{
		Stroke* s = Pop(&app->activeLayer->redoStrokes);
		if (s)
		{
			Push(&app->activeLayer->undoStrokes, s);
		}
	}

	if (IsKeyPressed(KEY_ONE))
		app->currentTool->color = BLACK;

	if (IsKeyPressed(KEY_TWO))
		app->currentTool->color = RED;

	if (IsKeyPressed(KEY_THREE))
		app->currentTool->color = BLUE;
	
	// Switch tools
	if (IsKeyPressed(KEY_B))
	{
		app->currentTool = Get(&app->tools, "brush");
	}

	if (IsKeyPressed(KEY_E))
	{
		app->currentTool = Get(&app->tools, "eraser");
	}
	

	// Start stroke
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		printf("Started stroke.\n");
		
		
		Tool* t = app->currentTool;
		if (t) {
			app->currentStroke = CreateStroke(t->color, t->size);

			if (t->type == TOOL_ERASER)
				app->currentStroke->type = STROKE_ERASE;
			else
				app->currentStroke->type = STROKE_DRAW;
		}
	}

	// Continue stroke
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		if (app->currentStroke)
		{
			AddPointToStroke(app->currentStroke, (int)mouse.x, (int)mouse.y);
		}
	}

	// End stroke
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
	{
		printf("Ended stroke.\n");
		if (app->currentStroke)
		{
			Push(&app->activeLayer->undoStrokes, app->currentStroke);
			ClearStack(&app->activeLayer->redoStrokes);
		}

		app->currentStroke = NULL;
	}
	
}
