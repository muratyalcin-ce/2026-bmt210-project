#include "app.h"
#include "input.h"
#include "stack.h"
#include "stroke.h"
#include "layer.h"

#include <raylib.h>
#include <stddef.h>
#include <stdio.h>


void InitApp(App *app)
{
	
	app->root = CreateLayer("Root");
	app->currentStroke = NULL; 
	app->activeLayer = app->root;
    InitStack(&app->redoStack);

    InitHashMap(&app->tools);

    Tool brush = { TOOL_BRUSH, WHITE, 5 };
    Tool eraser = { TOOL_ERASER, BLACK, 10 };

    Insert(&app->tools, "brush", brush);
    Insert(&app->tools, "eraser", eraser);

    app->currentTool = Get(&app->tools, "brush");

}

void UpdateApp(App *app)
{
    HandleInput(app);
}

void RenderApp(App *app)
{
	ClearBackground(BLACK);

    // Draw completed strokes
	DrawLayerTree(app->activeLayer);

	// Current tool name
	if (app->currentTool) {
		DrawText(
			app->currentTool->type == TOOL_BRUSH ? "Brush" : "Eraser",
			10, 10, 8, WHITE
			);
	}

	// Active layer name
	DrawText(
		app->activeLayer->name,
		10, 20, 8, WHITE
		);
	
	DrawText(
		app->activeLayer->visible? "Visible" : "Not Visible",
		10, 30, 8, WHITE
		);
	
    // Draw current stroke (while drawing)
    if (app->currentStroke)
    {
        DrawSingleStroke(app->currentStroke);
    }
}

void DestroyApp(App *app)
{
	ClearStack(&app->redoStack);
}
