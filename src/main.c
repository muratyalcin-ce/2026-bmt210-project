#include <raylib.h>
#include "app.h"

int main(void)
{
    InitWindow(800, 600, "Paint App");
    SetTargetFPS(60);

    App app;
    InitApp(&app, 800, 600);

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
