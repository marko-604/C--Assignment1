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
#include "Towers/TowerStrategy/FarthestStrategy.h"
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

// ADDED: We'll show a small bottom panel that displays each tower's stats
static const int BOTTOM_PANEL_HEIGHT = 120; // how tall the bottom area is
static int bottomPanelScrollOffset = 0; // scroll offset for the bottom panel

// ADDED: We define a helper that returns a string name for each strategy
std::string StrategyName(TargetSelectionStrategy *strat) {
  if (!strat)
    return "None";
  if (dynamic_cast<LowestHealthTargetStrategy *>(strat))
    return "Lowest Health";
  if (dynamic_cast<HighestHealthTargetStrategy *>(strat))
    return "Highest Health";
  if (dynamic_cast<StrongestTargetStrategy *>(strat))
    return "Strongest";
  if (dynamic_cast<FarthestTargetStrategy *>(strat))
    return "Farthest";
  if (dynamic_cast<WeakestTargetStrategy *>(strat))
    return "Weakest";
  return "Unknown";
}

// ADDED: build lines that describe each tower's stats
std::vector<std::string> BuildTowerStats(const std::vector<Tower *> &towers) {
  std::vector<std::string> lines;
  for (auto *t : towers) {
    std::ostringstream oss;
    oss << "Tower " << t->getTid() << " (Lv " << t->getLevel()
        << "), DMG=" << t->getDamage() << ", Range=" << t->getRange()
        << ", Strategy=" << StrategyName(t->getStrategy())
        << ", Level= " << t->getLevel()
        << ", LvlUpCost=" << t->getLevelUpCost();

    lines.push_back(oss.str());
  }
  return lines;
}

// ADDED: A function to draw the small bottom panel showing tower stats
void DrawTowerStatsPanel(int startX, int startY, int width, int height,
                         const std::vector<Tower *> &towers, int scrollOffset) {
  // background
  DrawRectangle(startX, startY, width, height, DARKGRAY);
  // title
  DrawText("Tower Stats:", startX + 10, startY + 10, 20, BLACK);

  // build lines for each tower
  auto lines = BuildTowerStats(towers);

  const int fontSize = 14;
  const int lineSpacing = fontSize + 2;
  int regionY = startY + 40;
  int regionHeight = height - 40;
  BeginScissorMode(startX, regionY, width, regionHeight);
  for (size_t i = 0; i < lines.size(); i++) {
    int textY = regionY + (int)i * lineSpacing - scrollOffset;
    DrawText(lines[i].c_str(), startX + 10, textY, fontSize, BLACK);
  }
  EndScissorMode();
}
// END ADDED

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

  // CHANGED: the total window height is mapHeight + BOTTOM_PANEL_HEIGHT
  int screenWidth = mapWidth + sidePanelWidth;
  int screenHeight = mapHeight + BOTTOM_PANEL_HEIGHT; // CHANGED

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

    // Update scroll offset for side panel
    float wheelMove = GetMouseWheelMove();
    scrollOffset -= static_cast<int>(wheelMove * 20);
    if (scrollOffset < 0)
      scrollOffset = 0;
    {
      const int lineSpacing = 14 + 2;
      int totalTextHeight = (int)messages.size() * lineSpacing;
      if (totalTextHeight > availableHeightForObserver) {
        int maxScroll = totalTextHeight - availableHeightForObserver;
        if (scrollOffset > maxScroll)
          scrollOffset = maxScroll;
      } else {
        scrollOffset = 0;
      }
    }

    // ADDED: also update the scroll for the bottom stats panel
    bottomPanelScrollOffset -= (int)(wheelMove * 20);
    if (bottomPanelScrollOffset < 0)
      bottomPanelScrollOffset = 0;
    {
      // number of lines = number of towers
      const int fontSize = 14;
      const int lineSpacing = fontSize + 2;
      int totalTextHeight = (int)towers.size() * lineSpacing;
      int visibleHeight = BOTTOM_PANEL_HEIGHT - 40; // minus panel title area
      if (totalTextHeight > visibleHeight) {
        int maxScroll = totalTextHeight - visibleHeight;
        if (bottomPanelScrollOffset > maxScroll) {
          bottomPanelScrollOffset = maxScroll;
        }
      } else {
        bottomPanelScrollOffset = 0;
      }
    }

    // Tower Placement / Upgrade / Strategy Change.
    if (GetMouseX() < mapWidth && GetMouseY() < mapHeight) {
      Vector2 pos = GetMousePosition();
      int col = pos.x / map->tileSize;
      int row = pos.y / map->tileSize;

      // Check if there's a tower at this tile.
      Tower *existingTower = nullptr;
      int index = -1;
      for (int i = 0; i < (int)towers.size(); i++) {
        if (towers[i]->getX() == row && towers[i]->getY() == col) {
          existingTower = towers[i];
          index = i;
          break;
        }
      }

      // Strategy changes
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
        } else if (IsKeyPressed(KEY_FOUR)) {
          existingTower->setStrategy(new FarthestTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Farthest critter." << std::endl;
        } else if (IsKeyPressed(KEY_FIVE)) {
          existingTower->setStrategy(new WeakestTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Weakest critter." << std::endl;
        }
      }

      // Place / upgrade towers ...
      if (IsKeyPressed(KEY_T)) {
        if (!existingTower && player_points >= 100) {
          player_points -= 100;
          Tower *t = new Tower();
          t->setStrategy(new WeakestTargetStrategy());
          TowerObserver *obs = new TowerObserver();
          t->Attach(obs);
          t->setX(row);
          t->setY(col);
          map->ToggleTower(t, row, col);
          towers.push_back(t);
          updateTowerTileColor(map, row, col, t);
        }
      }
      if (IsKeyPressed(KEY_F)) {
        if (player_points >= 100) {
          player_points -= 100;
          if (existingTower) {
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
      if (IsKeyPressed(KEY_S)) {
        if (player_points >= 100) {
          player_points -= 100;
          if (existingTower) {
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
      if (IsKeyPressed(KEY_B)) {
        if (player_points >= 100) {
          player_points -= 100;
          if (existingTower) {
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

    // Remove tower
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
          break;
        } else {
          ++it;
        }
      }
    }
    // Level up
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
    int availableHeightForObserver2 = mapHeight - observerOutputYFinal - 10;
    DrawObserverOutputScrollable(
        mapWidth + 10, observerOutputYFinal, sidePanelWidth - 20,
        availableHeightForObserver2, messages, scrollOffset);

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

      // ADDED: Show the tower's level in the bottom-left corner of the tile
      // For example, "L2" if level=2
      std::string levelText = "L" + std::to_string(t->getLevel());
      // We'll place it near the bottom-left: tileX + 5, tileY + tileSize - 25
      DrawText(levelText.c_str(), tileX + 5, tileY + map->tileSize - 25, 20,
               BLACK);
    }

    // ADDED: draw the bottom panel with tower stats
    DrawTowerStatsPanel(0,                      // startX
                        mapHeight,              // startY => below the map
                        screenWidth,            // full width
                        BOTTOM_PANEL_HEIGHT,    // panel height
                        towers,                 // pass the tower list
                        bottomPanelScrollOffset // scroll offset
    );

    EndDrawing();
  }
  CloseWindow();
  return 0;
}
