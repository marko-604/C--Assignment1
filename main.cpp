#include "Critters/Critter.h"
#include "Maps/Map.h"
#include "Observer/CritterObserver.h"
#include "Observer/MapObserver.h"
#include "Observer/Subject.h"
#include "Observer/TowerObserver.h"
#include "Towers/Tower.h"
#include "Towers/TowerDecorators/BombDecorator.h"
#include "Towers/TowerDecorators/FreezingDecorator.h"
#include "Towers/TowerDecorators/SniperDecorator.h"
#include "Towers/TowerDecorators/TowerDecorator.h"

// Include the strategy headers.
#include "Towers/TowerStrategy/HighestHealthStrategy.h"
#include "Towers/TowerStrategy/LowestHealthStrategy.h"
#include "Towers/TowerStrategy/Strategies.h"

#include "Towers/TowerStrategy/WeakestStrategy.h"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// Helper: Update the color of the tile where a tower is placed,
// based on the tower's decorator type. (Assumes FREEZINGTOWER, SNIPERTOWER,
// BOMBTOWER, and REGULARTOWER are defined color constants.)
void updateTowerTileColor(Map *map, int row, int col, Tower *t) {
  if (dynamic_cast<FreezingDecorator *>(t) != nullptr) {
    map->grid[row][col] = FREEZINGTOWER;
  } else if (dynamic_cast<SniperDecorator *>(t) != nullptr) {
    map->grid[row][col] = SNIPERTOWER;
  } else if (dynamic_cast<BombDecorator *>(t) != nullptr) {
    map->grid[row][col] = BOMBTOWER;
  } else {
    map->grid[row][col] = REGULARTOWER;
  }
}

// Draws the legend in the side panel (below the HUD).
void DrawLegend(int startX, int startY) {
  int boxSize = 20;
  int spacing = 30; // vertical spacing between items

  DrawText("Legend", startX, startY, 20, BLACK);

  // Map elements
  DrawRectangle(startX, startY + spacing, boxSize, boxSize, LIGHTGRAY);
  DrawText("Empty", startX + boxSize + 10, startY + spacing, 20, BLACK);

  DrawRectangle(startX, startY + 2 * spacing, boxSize, boxSize, GREEN);
  DrawText("Path", startX + boxSize + 10, startY + 2 * spacing, 20, BLACK);

  DrawRectangle(startX, startY + 3 * spacing, boxSize, boxSize, BLUE);
  DrawText("Entry", startX + boxSize + 10, startY + 3 * spacing, 20, BLACK);

  DrawRectangle(startX, startY + 4 * spacing, boxSize, boxSize, RED);
  DrawText("Exit", startX + boxSize + 10, startY + 4 * spacing, 20, BLACK);

  // Towers
  DrawRectangle(startX, startY + 5 * spacing, boxSize, boxSize, YELLOW);
  DrawText("Regular Tower", startX + boxSize + 10, startY + 5 * spacing, 20,
           BLACK);

  DrawRectangle(startX, startY + 6 * spacing, boxSize, boxSize, GOLD);
  DrawText("Freezing Tower", startX + boxSize + 10, startY + 6 * spacing, 20,
           BLACK);

  DrawRectangle(startX, startY + 7 * spacing, boxSize, boxSize, ORANGE);
  DrawText("Sniper Tower", startX + boxSize + 10, startY + 7 * spacing, 20,
           BLACK);

  DrawRectangle(startX, startY + 8 * spacing, boxSize, boxSize, PINK);
  DrawText("Bomb Tower", startX + boxSize + 10, startY + 8 * spacing, 20,
           BLACK);

  // Critters
  DrawRectangle(startX, startY + 9 * spacing, boxSize, boxSize, BROWN);
  DrawText("Squirrel Critter", startX + boxSize + 10, startY + 9 * spacing, 20,
           BLACK);

  DrawRectangle(startX, startY + 10 * spacing, boxSize, boxSize, PURPLE);
  DrawText("Wolf Critter", startX + boxSize + 10, startY + 10 * spacing, 20,
           BLACK);

  DrawRectangle(startX, startY + 11 * spacing, boxSize, boxSize, BLACK);
  DrawText("Bear Critter", startX + boxSize + 10, startY + 11 * spacing, 20,
           BLACK);
}

// Draws the observer output inside a fixed, scrollable region.
void DrawObserverOutputScrollable(int startX, int startY, int panelWidth,
                                  int availableHeight,
                                  const std::vector<std::string> &messages,
                                  int scrollOffset) {
  const int fontSize = 14;
  const int lineSpacing = fontSize + 2;

  DrawText("Observer Output:", startX, startY, fontSize, BLACK);
  int regionY = startY + lineSpacing;
  int regionHeight = availableHeight - lineSpacing;

  BeginScissorMode(startX, regionY, panelWidth, regionHeight);
  for (size_t i = 0; i < messages.size(); i++) {
    int textY = regionY + i * lineSpacing - scrollOffset;
    DrawText(messages[i].c_str(), startX, textY, fontSize, BLACK);
  }
  EndScissorMode();
}

int main() {
  int rows, cols;
  std::cout << "Enter the number of rows: ";
  std::cin >> rows;
  std::cout << "Enter the number of cols: ";
  std::cin >> cols;

  // Create the map.
  Map *map = new Map(cols, rows, 80);

  // Create a shared messages vector for observer updates.
  std::vector<std::string> messages;
  MapObserver *m_obs = new MapObserver(&messages);
  map->Attach(m_obs);
  bool isValidMap = map->RunEditor();

  if (!isValidMap) {
    std::cout << "INVALID MAP!!" << std::endl;
    return 1;
  }

  int mapWidth = map->gridWidth * map->tileSize;
  int mapHeight = map->gridHeight * map->tileSize;
  int sidePanelWidth = 300;
  int screenWidth = mapWidth + sidePanelWidth;
  int screenHeight = mapHeight;

  InitWindow(screenWidth, screenHeight, "Game");
#ifdef _WIN32
  HWND handle = GetWindowHandle();
  SetForegroundWindow(handle);
#endif

  SetTargetFPS(60);

  std::vector<Tower *> towers;
  CritterGenerator generator;
  std::vector<std::pair<int, int>> critter_path = map->getPath();

  int hudY = 10;
  int hudFontSize = 20;
  int hudHeight = hudFontSize + 10;
  int player_points = 1000;
  int player_health = 1000;
  int max_ticks = 120; // 2 minutes
  int legendStartY = hudY + hudHeight + 10;
  int legendHeight = 12 * 30;
  int observerOutputY = legendStartY + legendHeight + 20;
  int observerPanelWidth = sidePanelWidth - 20;
  int availableHeightForObserver = mapHeight - observerOutputY - 10;
  int scrollOffset = 0;
  double tickInterval = 1.0; // seconds
  double lastTick = GetTime();
  int tickCount = 0;

  generator.levelUp(critter_path);

  while (!WindowShouldClose()) {
    SetWindowFocused();
    double currentTime = GetTime();
    if (currentTime - lastTick >= tickInterval) {
      tickCount++;
      lastTick = currentTime;
      if (tickCount >= max_ticks)
        break;

      // Update towers.
      // Update critters.
      for (Critter *c : generator.critters) {
        if (c->getCol() == map->exitCol && c->getRow() == map->exitRow) {
          player_health -= c->getStr();
          continue;
        }

        c->Update(*map, tickCount);
        map->ToggleCritter(c, c->getRow(), c->getCol());
      }

      for (Tower *t : towers) {
        t->attack(generator.critters, tickCount, &player_points, *map);
      }

      if (tickCount % 10 == 0) {
        generator.levelUp(critter_path);
      }
    }

    // Update scroll offset.
    float wheelMove = GetMouseWheelMove();
    scrollOffset -= static_cast<int>(wheelMove * 20);
    if (scrollOffset < 0)
      scrollOffset = 0;
    const int lineSpacing = 14 + 2;
    int totalTextHeight = messages.size() * lineSpacing;
    if (totalTextHeight > availableHeightForObserver) {
      int maxScroll = totalTextHeight - availableHeightForObserver;
      if (scrollOffset > maxScroll)
        scrollOffset = maxScroll;
    } else {
      scrollOffset = 0;
    }

    // Tower Placement / Upgrade / Strategy Change.
    if (GetMouseX() < mapWidth && GetMouseY() < mapHeight) {
      Vector2 pos = GetMousePosition();
      int col = pos.x / map->tileSize;
      int row = pos.y / map->tileSize;

      // Check if there's a tower at this tile.
      Tower *existingTower = nullptr;
      int index = -1;
      for (int i = 0; i < towers.size(); i++) {
        if (towers[i]->getX() == row && towers[i]->getY() == col) {
          existingTower = towers[i];
          index = i;
          break;
        }
      }

      // --- Strategy Change Section ---
      // If a tower exists, allow strategy changes via keys 1-4.
      if (existingTower != nullptr) {
        if (IsKeyPressed(KEY_ONE)) {
          existingTower->setStrategy(new LowestHealthTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Lowest Health." << std::endl;
        } else if (IsKeyPressed(KEY_TWO)) {
          existingTower->setStrategy(new HighestHealthTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Highest Health." << std::endl;
        } else if (IsKeyPressed(KEY_THREE)) {
          existingTower->setStrategy(new StrongestTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Strongest." << std::endl;
        }
      }

      // --- Tower Placement / Upgrade for Different Tower Types ---
      // Regular Tower (T): place only if no tower exists.
      if (IsKeyPressed(KEY_T)) {
        if (existingTower == nullptr && player_points >= 100) {
          player_points -= 100;
          Tower *t = new Tower();
          // (Optionally set a default strategy here)
          t->setStrategy(new WeakestTargetStrategy()); // Set a default strategy
                                                       // of the weakest target.
          TowerObserver *obs = new TowerObserver();
          t->Attach(obs);
          t->setX(row);
          t->setY(col);
          map->ToggleTower(t, row, col);
          towers.push_back(t);
          updateTowerTileColor(map, row, col, t);
        }
      }
      // Freezing Tower (F): upgrade if exists, else place new.
      if (IsKeyPressed(KEY_F)) {
        if (player_points >= 100) {
          player_points -= 100;
          if (existingTower != nullptr) {
            Tower *upgraded = new FreezingDecorator(existingTower, 0.5f);
            TowerObserver *obs = new TowerObserver();
            upgraded->Attach(obs);
            towers[index] = upgraded;
            map->ToggleTower(upgraded, row, col);
            updateTowerTileColor(map, row, col, upgraded);
          } else {
            Tower *base = new Tower();
            Tower *t = new FreezingDecorator(base, 0.5f);
            TowerObserver *obs = new TowerObserver();
            t->Attach(obs);
            t->setX(row);
            t->setY(col);
            map->ToggleTower(t, row, col);
            towers.push_back(t);
            updateTowerTileColor(map, row, col, t);
          }
        }
      }
      // Sniper Tower (S): upgrade if exists, else place new.
      if (IsKeyPressed(KEY_S)) {
        if (player_points >= 100) {
          player_points -= 100;
          if (existingTower != nullptr) {
            Tower *upgraded = new SniperDecorator(existingTower, 2, 10);
            TowerObserver *obs = new TowerObserver();
            upgraded->Attach(obs);
            towers[index] = upgraded;
            map->ToggleTower(upgraded, row, col);
            updateTowerTileColor(map, row, col, upgraded);
          } else {
            Tower *base = new Tower();
            Tower *t = new SniperDecorator(base, 2, 10);
            TowerObserver *obs = new TowerObserver();
            t->Attach(obs);
            t->setX(row);
            t->setY(col);
            map->ToggleTower(t, row, col);
            towers.push_back(t);
            updateTowerTileColor(map, row, col, t);
          }
        }
      }
      // Bomb Tower (B): upgrade if exists, else place new.
      if (IsKeyPressed(KEY_B)) {
        if (player_points >= 100) {
          player_points -= 100;
          if (existingTower != nullptr) {
            Tower *upgraded = new BombDecorator(existingTower, 2, 0.5f);
            TowerObserver *obs = new TowerObserver();
            upgraded->Attach(obs);
            towers[index] = upgraded;
            map->ToggleTower(upgraded, row, col);
            updateTowerTileColor(map, row, col, upgraded);
          } else {
            Tower *base = new Tower();
            Tower *t = new BombDecorator(base, 2, 0.5f);
            TowerObserver *obs = new TowerObserver();
            t->Attach(obs);
            t->setX(row);
            t->setY(col);
            map->ToggleTower(t, row, col);
            towers.push_back(t);
            updateTowerTileColor(map, row, col, t);
          }
        }
      }
    }

    if (IsKeyPressed(KEY_X)) {
      Vector2 pos = GetMousePosition();
      int col = pos.x / map->tileSize;
      int row = pos.y / map->tileSize;
      map->setToScenery(row, col);
      for (auto it = towers.begin(); it != towers.end();) {
        if (col == (*it)->getY() && row == (*it)->getX()) {
          player_points += (*it)->getResale();
          delete *it;
          it = towers.erase(it);
        }
      }
    }
    if (IsKeyPressed(KEY_L)) {
      Vector2 pos = GetMousePosition();
      int col = pos.x / map->tileSize;
      int row = pos.y / map->tileSize;

      for (Tower *t : towers) {
        if (t->getX() == row && t->getY() == col) {
          t->levelUp();
          std::cout << "Tower " << t->getTid() << " has been leveled up to "
                    << t->getLevel() << std::endl;
        }
      }
    }
    if (IsKeyPressed(KEY_Q)) {
      break;
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw the map.
    map->Draw();

    // Draw the side panel.
    DrawRectangle(mapWidth, 0, sidePanelWidth, mapHeight, LIGHTGRAY);

    // Draw HUD.
    std::string hudText =
        TextFormat("Points: %d   Health: %d", player_points, player_health);
    DrawText(hudText.c_str(), mapWidth + 10, 10, 20, BLACK);

    // Draw the legend.
    int legendStartY = 10 + 20 + 10;
    DrawLegend(mapWidth + 10, legendStartY);

    // Draw observer output.
    int innerLegendHeight = 12 * 30;
    int observerOutputYFinal = legendStartY + innerLegendHeight + 20;
    int availableHeightForObserver = mapHeight - observerOutputYFinal - 10;
    DrawObserverOutputScrollable(
        mapWidth + 10, observerOutputYFinal, sidePanelWidth - 20,
        availableHeightForObserver, messages, scrollOffset);

    // Draw upgrade indicators on tower tiles.
    for (Tower *t : towers) {
      int tileX = t->getY() * map->tileSize;
      int tileY = t->getX() * map->tileSize;
      if (dynamic_cast<FreezingDecorator *>(t) != nullptr) {
        DrawText("F", tileX + map->tileSize - 15, tileY + 5, 20, BLACK);
      } else if (dynamic_cast<SniperDecorator *>(t) != nullptr) {
        DrawText("S", tileX + map->tileSize - 15, tileY + 5, 20, BLACK);
      } else if (dynamic_cast<BombDecorator *>(t) != nullptr) {
        DrawText("B", tileX + map->tileSize - 15, tileY + 5, 20, BLACK);
      }
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
