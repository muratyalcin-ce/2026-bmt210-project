#include <raylib.h>
#include "app.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_terminal.h"
int main(void)
{
	SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_TOPMOST);
	SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    InitWindow(800, 600, "Paint App");
    SetTargetFPS(60);
	
    int monitor = GetCurrentMonitor();
    int width = GetMonitorWidth(monitor);
    int height = GetMonitorHeight(monitor);

    SetWindowSize(width, height);
    SetWindowPosition(0, 0);
	
    App app;
	
    InitApp(&app);
	GuiLoadStyleTerminal();

    while (!WindowShouldClose())
    {
		if(IsKeyDown(KEY_Q)){
			break;
		}

        UpdateApp(&app);

        BeginDrawing();

        RenderApp(&app);

        EndDrawing();
    }

    DestroyApp(&app);
    CloseWindow();
    return 0;
}
