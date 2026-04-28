#include "app.h"
#include "canvas.h"
#include "input.h"
#include "draw.h"
#include "stack.h"
#include "stroke.h"
#include "layer.h"

#include <raylib.h>
#include <stddef.h>
#include <stdio.h>


void InitApp(App *app, int width, int height)
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

	if (!app->currentTool)
	{
		printf("ERROR: brush tool not found\n");
	}
}

void UpdateApp(App *app)
{
    HandleInput(app);
}

void RenderApp(App *app)
{
	ClearBackground(BLACK);

    // Draw completed strokes
	DrawLayerTree(app->root);


	if (app->currentTool) {
		DrawText(
			app->currentTool->type == TOOL_BRUSH ? "Brush" : "Eraser",
			10, 10, 8, WHITE
			);
	}
	
	DrawText(
		app->activeLayer->name,
		20, 20, 8, WHITE
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
