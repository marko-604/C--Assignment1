#include "GameMaster.h"

void RunGame(const Map &map) {
  // Calculate the window size based on the map's grid and tile size.
  int screenWidth = map.gridWidth * map.tileSize;
  int screenHeight = map.gridHeight * map.tileSize;

  InitWindow(screenWidth, screenHeight, "Game");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Draw the map without any instructional text.
    for (int row = 0; row < map.gridHeight; row++) {
      for (int col = 0; col < map.gridWidth; col++) {
        Rectangle tileRect = {col * (float)map.tileSize,
                              row * (float)map.tileSize, (float)map.tileSize,
                              (float)map.tileSize};
        Color tileColor;
        switch (map.grid[row][col]) {
        case EMPTY:
          tileColor = LIGHTGRAY;
          break;
        case PATH:
          tileColor = GREEN;
          break;
        case ENTRY:
          tileColor = BLUE;
          break;
        case EXIT:
          tileColor = RED;
          break;
        default:
          tileColor = LIGHTGRAY;
          break;
        }
        DrawRectangleRec(tileRect, tileColor);
        DrawRectangleLines(tileRect.x, tileRect.y, tileRect.width,
                           tileRect.height, DARKGRAY);
      }
    }
    EndDrawing();
  }

  CloseWindow();
}
