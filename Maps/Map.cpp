#include "Map.h"
#include "../Critters/Critter.h"
#include "../Towers/Tower.h"
#include <queue>
#include <utility>
#include <vector>

/**
 * @brief Constructs the map and initializes the grid and entry/exit to unset.
 */
Map::Map(int width, int height, int tileSize)
    : gridWidth(width), gridHeight(height), tileSize(tileSize), entryRow(-1),
      entryCol(-1), exitRow(-1), exitCol(-1) {
  grid.resize(gridHeight, std::vector<TileType>(gridWidth, EMPTY));
}

/**
 * @brief Destructor for the Map class.
 */
Map::~Map() {
  // Clean up if needed.
}

/**
 * @brief Sets a tile back to EMPTY (scenery).
 */
void Map::setToScenery(int row, int col) {
  if (row < gridHeight && col < gridWidth) {
    grid[row][col] = EMPTY;
  }
}

/**
 * @brief Renders the grid and tiles on the screen using raylib.
 */
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

/**
 * @brief Places a critter on the specified tile.
 */
void Map::ToggleCritter(Critter *critter, int row, int col) {
  if (row < 0 || row >= gridHeight || col < 0 || col >= gridWidth)
    return;

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

  Notify();
}

/**
 * @brief Marks a tile as PATH.
 */
void Map::setToPath(int row, int col) {
  if (row < 0 || row >= gridHeight || col < 0 || col >= gridWidth ||
      grid[row][col] == ENTRY)
    return;

  grid[row][col] = PATH;
  Notify();
}

/**
 * @brief Places a tower on the map if the tile is empty.
 */
void Map::ToggleTower(Tower *tower, int row, int col) {
  if (row < 0 || row >= gridHeight || col < 0 || col >= gridWidth)
    return;

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
  Notify();
}

/**
 * @brief Toggles a tile between EMPTY and PATH, unless it's ENTRY or EXIT.
 */
void Map::TogglePath(int row, int col) {
  if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
    if (grid[row][col] == ENTRY || grid[row][col] == EXIT)
      return;
    grid[row][col] = (grid[row][col] == EMPTY) ? PATH : EMPTY;
    Notify();
  }
}

/**
 * @brief Sets the tile as ENTRY and updates the internal entry coordinates.
 */
void Map::SetEntry(int row, int col) {
  if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
    if (entryRow != -1 && entryCol != -1)
      grid[entryRow][entryCol] = PATH;
    grid[row][col] = ENTRY;
    entryRow = row;
    entryCol = col;
    Notify();
  }
}

/**
 * @brief Sets the tile as EXIT and updates the internal exit coordinates.
 */
void Map::SetExit(int row, int col) {
  if (row >= 0 && row < gridHeight && col >= 0 && col < gridWidth) {
    if (exitRow != -1 && exitCol != -1)
      grid[exitRow][exitCol] = PATH;
    grid[row][col] = EXIT;
    exitRow = row;
    exitCol = col;
    Notify();
  }
}

/**
 * @brief Checks whether there is a valid path from ENTRY to EXIT using BFS.
 */
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

/**
 * @brief Runs the interactive map editor.
 *
 * @return true if a valid map is confirmed.
 */
bool Map::RunEditor() {
  InitWindow(100, 100, "Temp");
  int monitorWidth = GetMonitorWidth(0);
  int monitorHeight = GetMonitorHeight(0);
  CloseWindow();

  int desiredWidth = static_cast<int>(monitorWidth * 0.8);
  int desiredHeight = static_cast<int>(monitorHeight * 0.8);
  int computedTileSize = desiredWidth / gridWidth;

  if (gridHeight * computedTileSize > desiredHeight) {
    computedTileSize = desiredHeight / gridHeight;
  }
  if (computedTileSize < 1)
    computedTileSize = 1;
  tileSize = computedTileSize;

  int screenWidth = gridWidth * tileSize;
  int screenHeight = gridHeight * tileSize;

  InitWindow(screenWidth, screenHeight, "Map Editor");
  SetTargetFPS(60);
  bool mapConfirmed = false;

  while (!WindowShouldClose() && !mapConfirmed) {
    SetWindowFocused();

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
    Draw();
    DrawText("Left Click: Toggle Path  |  E: Set Entry  |  X: Set Exit", 10, 10,
             20, BLACK);
    DrawText("Press S to start game (if path is valid)", 10, 40, 20, BLACK);
    DrawText(TextFormat("Tile Size: %d", tileSize), 10, screenHeight - 30, 20,
             BLACK);
    EndDrawing();
  }

  CloseWindow();
  return mapConfirmed;
}

/**
 * @brief Returns the path from entry to exit using BFS.
 *
 * @return std::vector<std::pair<int, int>> Path as a list of (row, col)
 * coordinates.
 */
std::vector<std::pair<int, int>> Map::getPath() {
  if (entryRow == -1 || exitRow == -1)
    return {};

  std::vector<std::vector<bool>> visited(gridHeight,
                                         std::vector<bool>(gridWidth, false));
  std::vector<std::vector<std::pair<int, int>>> parent(
      gridHeight, std::vector<std::pair<int, int>>(gridWidth, {-1, -1}));
  std::queue<std::pair<int, int>> q;

  q.push({entryRow, entryCol});
  visited[entryRow][entryCol] = true;

  int dr[4] = {-1, 1, 0, 0};
  int dc[4] = {0, 0, -1, 1};
  bool foundExit = false;

  while (!q.empty() && !foundExit) {
    auto [r, c] = q.front();
    q.pop();
    if (r == exitRow && c == exitCol) {
      foundExit = true;
      break;
    }

    for (int i = 0; i < 4; i++) {
      int nr = r + dr[i], nc = c + dc[i];
      if (nr >= 0 && nr < gridHeight && nc >= 0 && nc < gridWidth) {
        if (!visited[nr][nc] &&
            (grid[nr][nc] == PATH || grid[nr][nc] == ENTRY ||
             grid[nr][nc] == EXIT || grid[nr][nc] == SQUIRRELCRITTER ||
             grid[nr][nc] == WOLFCRITTER || grid[nr][nc] == BEARCRITTER)) {
          visited[nr][nc] = true;
          parent[nr][nc] = {r, c};
          q.push({nr, nc});
        }
      }
    }
  }

  if (!foundExit)
    return {};

  std::vector<std::pair<int, int>> path;
  for (int r = exitRow, c = exitCol; !(r == entryRow && c == entryCol);) {
    path.push_back({r, c});
    std::tie(r, c) = parent[r][c];
  }
  path.push_back({entryRow, entryCol});
  return path;
}

/**
 * @brief Runs the main game loop for drawing the map.
 */
void RunGame(Map &map) {
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
