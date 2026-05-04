#include <raylib.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "layer.h"
#include "stroke.h"
#include "app.h"

extern StrokeList strokes;
extern Stroke* currentStroke;

int layerCount = 1;
void HandleInput(App *app)
{
    Vector2 mouse = GetMousePosition();
	
	if (IsKeyPressed(KEY_L)) {
		app->activeLayer = GetNextLayer(app->activeLayer, app->root);
	}
	if (IsKeyPressed(KEY_K)) {
		app->activeLayer = GetPreviousLayer(app->activeLayer, app->root);
	}

	if (IsKeyPressed(KEY_V)) {
		ToggleActiveLayerVisibility(app);
	}

	if (IsKeyPressed(KEY_N)) {
		AddSiblingLayerToApp(app);
	}
	
	if (IsKeyPressed(KEY_M)) {
		AddChildLayerToApp(app);
	}

	if (IsKeyPressed(KEY_Z)) {
		Undo(app);
	}
	
	if (IsKeyPressed(KEY_R)) {
		Redo(app);
	}

	if (IsKeyPressed(KEY_ONE))
		app->currentTool->color = BLACK;

	if (IsKeyPressed(KEY_TWO))
		app->currentTool->color = RED;

	if (IsKeyPressed(KEY_THREE))
		app->currentTool->color = BLUE;
	
	// Switch tools
	if (IsKeyPressed(KEY_B)) {
		app->currentTool = Get(&app->tools, "brush");
	}

	if (IsKeyPressed(KEY_E)) {
		app->currentTool = Get(&app->tools, "eraser");
	}
	
	
	int mouseOnUI = CheckCollisionPointRec(mouse, (Rectangle){0, 0, 140, 240});
	
	// Start stroke
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !mouseOnUI) {
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
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !mouseOnUI) {
		if (app->currentStroke)
		{
			AddPointToStroke(app->currentStroke, (int)mouse.x, (int)mouse.y);
		}
	}

	// End stroke
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !mouseOnUI)
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
