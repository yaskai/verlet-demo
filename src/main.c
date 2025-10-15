#include "raylib.h"
#include "../include/resource_dir.h"
#include "rope.h"
#include "ui.h"

int main() {
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);

	InitWindow(1280, 800, "Verlet demo");
	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	Rope rope = {0};
	RopeInit(&rope, (Vector2){300, 100});
	
	Ui ui = {0};
	UiInit(&ui, &rope);

	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();

		UiUpdate(&ui);

		rope.allow_move = ((ui.flags & UI_FOCUSED) == 0);
		RopeUpdate(&rope, delta_time);

		BeginDrawing();
		ClearBackground(BLACK);

		RopeDraw(&rope);
		UiDraw(&ui);

		EndDrawing();
	}

	CloseWindow();
	RopeClose(&rope);
	UiClose(&ui);

	return 0;
}

