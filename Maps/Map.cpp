
#include "Map.h"
#include "../Critters/Critter.h"
#include "../Towers/Tower.h"
#include <queue>
#include <utility>
#include <vector>

Map::Map(int width, int height, int tileSize)
    : gridWidth(width), gridHeight(height), tileSize(tileSize), entryRow(-1),
      entryCol(-1), exitRow(-1), exitCol(-1) {
  grid.resize(gridHeight, std::vector<TileType>(gridWidth, EMPTY));
}

Map::~Map() {
  // Clean up if needed.
}

void Map::Draw() {
  for (int row = 0; row < gridHeight; row++) {
    for (int col = 0; col < gridWidth; col++) {
      Rectangle tileRect = {col * (float)tileSize, row * (float)tileSize,
                            (float)tileSize, (float)tileSize};
      Color tileColor;
      switch (grid[row][col]) {
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
      case REGULARTOWER:
        tileColor = YELLOW;
        break;
      case FREEZINGTOWER:
        tileColor = GOLD;
        break;
      case SNIPERTOWER:
        tileColor = ORANGE;
        break;
      case BOMBTOWER:
        tileColor = PINK;
        break;
      case SQUIRRELCRITTER:
        tileColor = BROWN;
        break;
      case WOLFCRITTER:
        tileColor = PURPLE;
        break;
      case BEARCRITTER:
        tileColor = BLACK;
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
}

void Map::ToggleCritter(Critter *critter, int row, int col) {
  if (row < 0 || row >= gridHeight || col < 0 || col >= gridWidth) {
    return;
  }
  if (grid[row][col] == ENTRY || grid[row][col] == EXIT ||
      grid[row][col] == REGULARTOWER || grid[row][col] == FREEZINGTOWER ||
      grid[row][col] == BOMBTOWER || grid[row][col] == SNIPERTOWER)
    return;

  if (critter->getType() == SQUIRREL)
    grid[row][col] = SQUIRRELCRITTER;
  else if (critter->getType() == WOLF)
    grid[row][col] = WOLFCRITTER;
  else
    grid[row][col] = BEARCRITTER;
}

void Map::setToPath(int row, int col) {
  if (row < 0 || row >= gridHeight || col < 0 || col >= gridWidth ||
      grid[row][col] == ENTRY)
    return;

  grid[row][col] = PATH;
}

void Map::ToggleTower(Tower *tower, int row, int col) {

  if (row < 0 || row >= gridHeight || col < 0 || col >= gridWidth) {
    return;
  }

  if (grid[row][col] != EMPTY)
    return;

  if (tower->getType() == FREEZING)
    grid[row][col] = FREEZINGTOWER;
  else if (tower->getType() == REGULAR)
    grid[row][col] = REGULARTOWER;
  else if (tower->getType() == SNIPER)
    grid[row][col] = SNIPERTOWER;
  else
    grid[row][col] = BOMBTOWER;

  tower->setX(row);
  tower->setY(col);
  return;
}

void Map::TogglePath(int row, int col) {
  if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
    if (grid[row][col] == ENTRY || grid[row][col] == EXIT)
      return;
    grid[row][col] = (grid[row][col] == EMPTY) ? PATH : EMPTY;
  }
}

void Map::SetEntry(int row, int col) {
  if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
    // Clear any previous entry.
    if (entryRow != -1 && entryCol != -1)
      grid[entryRow][entryCol] = PATH;
    grid[row][col] = ENTRY;
    entryRow = row;
    entryCol = col;
  }
}

void Map::SetExit(int row, int col) {
  if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
    // Clear any previous exit.
    if (exitRow != -1 && exitCol != -1)
      grid[exitRow][exitCol] = PATH;
    grid[row][col] = EXIT;
    exitRow = row;
    exitCol = col;
  }
}

bool Map::IsValidPath() {
  if (entryRow == -1 || exitRow == -1)
    return false;

  std::vector<std::vector<bool>> visited(gridHeight,
                                         std::vector<bool>(gridWidth, false));
  std::queue<std::pair<int, int>> q;
  q.push({entryRow, entryCol});
  visited[entryRow][entryCol] = true;

  int dr[4] = {-1, 1, 0, 0};
  int dc[4] = {0, 0, -1, 1};

  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();

    if (r == exitRow && c == exitCol)
      return true;

    for (int i = 0; i < 4; i++) {
      int nr = r + dr[i], nc = c + dc[i];
      if (nr >= 0 && nr < gridHeight && nc >= 0 && nc < gridWidth) {
        if (!visited[nr][nc] &&
            (grid[nr][nc] == PATH || grid[nr][nc] == ENTRY ||
             grid[nr][nc] == EXIT)) {
          visited[nr][nc] = true;
          q.push({nr, nc});
        }
      }
    }
  }
  return false;
}

bool Map::RunEditor() {
  // Create a temporary window to get monitor resolution
  InitWindow(100, 100, "Temp");
  int monitorWidth = GetMonitorWidth(0);
  int monitorHeight = GetMonitorHeight(0);
  CloseWindow();

  // Set desired window dimensions to 80% of the monitor resolution.
  int desiredWidth = static_cast<int>(monitorWidth * 0.8);
  int desiredHeight = static_cast<int>(monitorHeight * 0.8);

  // Compute a tile size that makes the grid fit within the desired dimensions.
  int computedTileSize = desiredWidth / gridWidth;
  if (gridHeight * computedTileSize > desiredHeight) {
    computedTileSize = desiredHeight / gridHeight;
  }
  if (computedTileSize < 1)
    computedTileSize = 1; // Ensure tile size is at least 1 pixel.
  tileSize = computedTileSize;

  // Compute window size from grid dimensions and tile size.
  int screenWidth = gridWidth * tileSize;
  int screenHeight = gridHeight * tileSize;

  // Now initialize the actual editor window.
  InitWindow(screenWidth, screenHeight, "Map Editor");
  SetTargetFPS(60);
  bool mapConfirmed = false;

  while (!WindowShouldClose() && !mapConfirmed) {
    // Handle input:
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePos = GetMousePosition();
      int col = mousePos.x / tileSize;
      int row = mousePos.y / tileSize;
      TogglePath(row, col);
    }
    if (IsKeyPressed(KEY_E)) {
      Vector2 mousePos = GetMousePosition();
      int col = mousePos.x / tileSize;
      int row = mousePos.y / tileSize;
      SetEntry(row, col);
    }
    if (IsKeyPressed(KEY_X)) {
      Vector2 mousePos = GetMousePosition();
      int col = mousePos.x / tileSize;
      int row = mousePos.y / tileSize;
      SetExit(row, col);
    }
    if (IsKeyPressed(KEY_S)) {
      if (IsValidPath())
        mapConfirmed = true;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int row = 0; row < gridHeight; row++) {
      for (int col = 0; col < gridWidth; col++) {
        Rectangle tileRect = {col * (float)tileSize, row * (float)tileSize,
                              (float)tileSize, (float)tileSize};
        Color tileColor;
        switch (grid[row][col]) {
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
        case REGULARTOWER:
          tileColor = YELLOW;
          break;
        case FREEZINGTOWER:
          tileColor = GOLD;
          break;
        case SNIPERTOWER:
          tileColor = ORANGE;
          break;
        case BOMBTOWER:
          tileColor = PINK;
          break;
        case SQUIRRELCRITTER:
          tileColor = BROWN;
          break;
        case WOLFCRITTER:
          tileColor = PURPLE;
          break;
        case BEARCRITTER:
          tileColor = BLACK;
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
    DrawText("Left Click: Toggle Path  |  E: Set Entry  |  X: Set Exit", 10, 10,
             20, BLACK);
    DrawText("Press S to start game (if path is valid)", 10, 40, 20, BLACK);

    // Debug: show tile size on screen (remove this later if desired)
    DrawText(TextFormat("Tile Size: %d", tileSize), 10, screenHeight - 30, 20,
             BLACK);
    EndDrawing();
  }

  CloseWindow();
  return mapConfirmed;
}

std::vector<std::pair<int, int>> Map::getPath() {
  // If entry/exit are not set, return an empty path
  if (entryRow == -1 || exitRow == -1) {
    return {};
  }

  // BFS setup
  std::vector<std::vector<bool>> visited(gridHeight,
                                         std::vector<bool>(gridWidth, false));
  // parent[r][c] will store the coordinates we came from to reach (r,c)
  std::vector<std::vector<std::pair<int, int>>> parent(
      gridHeight, std::vector<std::pair<int, int>>(gridWidth, {-1, -1}));

  std::queue<std::pair<int, int>> q;
  q.push({entryRow, entryCol});
  visited[entryRow][entryCol] = true;

  // We’ll break once we reach exit
  bool foundExit = false;

  // BFS directions (up, down, left, right)
  int dr[4] = {-1, 1, 0, 0};
  int dc[4] = {0, 0, -1, 1};

  while (!q.empty() && !foundExit) {
    auto [r, c] = q.front();
    q.pop();

    // If we reached the exit, stop BFS
    if (r == exitRow && c == exitCol) {
      foundExit = true;
      break;
    }

    for (int i = 0; i < 4; i++) {
      int nr = r + dr[i];
      int nc = c + dc[i];

      // Check boundaries and visited
      if (nr >= 0 && nr < gridHeight && nc >= 0 && nc < gridWidth) {
        // We can move if not visited and if tile is PATH, ENTRY, or EXIT
        if (!visited[nr][nc] &&
            (grid[nr][nc] == PATH || grid[nr][nc] == ENTRY ||
             grid[nr][nc] == EXIT)) {
          visited[nr][nc] = true;
          parent[nr][nc] = {r, c}; // we came to (nr, nc) from (r, c)
          q.push({nr, nc});
        }
      }
    }
  }

  // If we didn’t find exit, return empty
  if (!foundExit) {
    return {};
  }

  // Otherwise, reconstruct the path
  std::vector<std::pair<int, int>> path;
  int rr = exitRow;
  int cc = exitCol;

  // backtrack from exit to entry
  while (!(rr == entryRow && cc == entryCol)) {
    path.push_back({rr, cc});
    auto [pr, pc] = parent[rr][cc];
    rr = pr;
    cc = pc;
  }
  // finally push the entry cell
  path.push_back({entryRow, entryCol});

  // reverse the path so it goes from entry -> exit

  return path;
}
// This will be the main game loop we will place this function in the
void RunGame(Map &map) {
  // Calculate the window size based on the map's grid and tile size.
  int screenWidth = map.gridWidth * map.tileSize;
  int screenHeight = map.gridHeight * map.tileSize;

  InitWindow(screenWidth, screenHeight, "Game");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    map.Draw();
    EndDrawing();
  }

  CloseWindow();
}
