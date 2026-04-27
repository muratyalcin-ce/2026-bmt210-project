#include "app.h"
#include "input.h"
#include "draw.h"
#include "stack.h"
#include "stroke.h"

#include <raylib.h>
#include <stddef.h>


void InitApp(App *app, int width, int height)
{

    app->strokes.head = NULL;
    app->strokes.tail = NULL;
    app->currentStroke = NULL;

	InitStack(&app->undoStack);
	InitStack(&app->redoStack);
}

void UpdateApp(App *app)
{
    HandleInput(app);
}

void RenderApp(App *app)
{
	ClearBackground(BLACK);

    // Draw completed strokes
    DrawStrokesFromStack(&app->undoStack);

    // Draw current stroke (while drawing)
    if (app->currentStroke)
    {
        DrawSingleStroke(app->currentStroke);
    }
}

void DestroyApp(App *app)
{
	FreeStrokes(&app->strokes);
	ClearStack(&app->undoStack);
	ClearStack(&app->redoStack);
}
