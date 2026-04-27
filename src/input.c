#include <raylib.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "stroke.h"
#include "app.h"

extern StrokeList strokes;
extern Stroke* currentStroke;

void HandleInput(App *app)
{
    Vector2 mouse = GetMousePosition();

	if (IsKeyPressed(KEY_Z))
	{
		Stroke* s = Pop(&app->undoStack);
		if (s)
		{
			Push(&app->redoStack, s);
		}
	}
	
	if (IsKeyPressed(KEY_Y))
	{
		Stroke* s = Pop(&app->redoStack);
		if (s)
		{
			Push(&app->undoStack, s);
		}
	}


// Start stroke
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		printf("Started stroke.\n");
		app->currentStroke = CreateStroke(WHITE, 3);
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
			Push(&app->undoStack, app->currentStroke);
			ClearStack(&app->redoStack);
		}

		app->currentStroke = NULL;
	}
	
}
