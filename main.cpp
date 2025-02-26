#include "raylib.h"

int main() {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "My RAYLIB window");

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Hello, raylib!\n", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
