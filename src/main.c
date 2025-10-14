#include "raylib.h"
#include "../include/resource_dir.h"
#include "rope.h"

int main() {
	SetTraceLogLevel(LOG_NONE);
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);

	InitWindow(1280, 800, "Verlet demo");
	SetTargetFPS(60);

	SearchAndSetResourceDir("resources");

	Rope rope = {0};
	RopeInit(&rope, (Vector2){300, 100});

	while(!WindowShouldClose()) {
		float delta_time = GetFrameTime();
		RopeUpdate(&rope, delta_time);

		BeginDrawing();
		ClearBackground(BLACK);
		RopeDraw(&rope);
		EndDrawing();
	}

	CloseWindow();
	RopeClose(&rope);

	return 0;
}

