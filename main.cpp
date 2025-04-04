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
// based on the tower's decorator type.
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
    oss << "Tower " << t->getTid() << " (Lv " << t->getLevel() << "), "
        << "DMG=" << t->getDamage() << ", Range=" << t->getRange()
        << ", Strategy=" << StrategyName(t->getStrategy())
        << ", LvlUpCost=" << t->getLevelUpCost();
    lines.push_back(oss.str());
  }
  return lines;
}

// ADDED: A function to draw the small bottom panel showing tower stats
void DrawTowerStatsPanel(int startX, int startY, int width, int height,
                         const std::vector<Tower *> &towers, int scrollOffset) {
  // background
  DrawRectangle(startX, startY, width, height, RAYWHITE);
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

//static bool TextField(int x, int y, int width, int height, std::string &text) {
//  Rectangle box = {(float)x, (float)y, (float)width, (float)height};
//  DrawRectangleRec(box, LIGHTGRAY);
//  DrawText(text.c_str(), x + 4, y + 4, 20, BLACK);

//  int key = GetCharPressed();
//  while (key > 0) {
//    if (key == KEY_BACKSPACE && !text.empty()) {
//      text.pop_back();
//    } else if (key >= 32 && key <= 126 && text.size() < 5) {
//      text.push_back((char)key);
//    }
//    key = GetCharPressed();
//  }
//  return false;
//}

int main() {
  int rows = 10, cols = 10;
  std::string rowText = "10", colText = "10";
  bool ready = false;
  bool rowActive = false, colActive = false;
  bool rowCleared = false, colCleared = false;

  InitWindow(400, 200, "Configure Map");
  SetTargetFPS(60);

  while (!WindowShouldClose() && !ready) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Enter map dimensions:", 20, 20, 20, BLACK);

    // Rows textbox
    DrawText("Rows:", 20, 60, 20, BLACK);
    Rectangle rowBox{100, 55, 80, 30};
    DrawRectangleRec(rowBox, rowActive ? LIGHTGRAY : GRAY);
    DrawText(rowText.c_str(), rowBox.x + 4, rowBox.y + 4, 20, BLACK);

    // Cols textbox
    DrawText("Cols:", 200, 60, 20, BLACK);
    Rectangle colBox{260, 55, 80, 30};
    DrawRectangleRec(colBox, colActive ? LIGHTGRAY : GRAY);
    DrawText(colText.c_str(), colBox.x + 4, colBox.y + 4, 20, BLACK);

    // Handle clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mp = GetMousePosition();
      bool clickedRow = CheckCollisionPointRec(mp, rowBox);
      bool clickedCol = CheckCollisionPointRec(mp, colBox);

      if (clickedRow) {
        rowActive = true;
        colActive = false;
        if (!rowCleared) {
          rowText.clear();
          rowCleared = true;
        }
      } else if (clickedCol) {
        colActive = true;
        rowActive = false;
        if (!colCleared) {
          colText.clear();
          colCleared = true;
        }
      } else {
        rowActive = colActive = false;
      }
    }

    // Keyboard input
    int key = GetCharPressed();
    while (key > 0) {
      std::string *target =
          rowActive ? &rowText : (colActive ? &colText : nullptr);
      if (target) {
        if (key == KEY_BACKSPACE && !target->empty())
          target->pop_back();
        else if (key >= 32 && key <= 126 && target->size() < 5)
          target->push_back((char)key);
      }
      key = GetCharPressed();
    }

    // Start button
    Rectangle startBtn{150, 120, 100, 40};
    DrawRectangleRec(startBtn, GRAY);
    DrawText("Start", 175, 128, 20, WHITE);
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(GetMousePosition(), startBtn)) {
      rows = std::atoi(rowText.c_str());
      cols = std::atoi(colText.c_str());
      if (rows > 0 && cols > 0)
        ready = true;
    }

    EndDrawing();
  }

  CloseWindow();
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
  //int observerPanelWidth = sidePanelWidth - 20;
  int availableHeightForObserver = mapHeight - observerOutputY - 10;
  int scrollOffset = 0;
  double tickInterval = 1.0; // seconds
  double lastTick = GetTime();
  int tickCount = 0;

  // NEW: boolean to keep track of pause state
  bool paused = false;

  // Start with some critters in the vector
  generator.levelUp(critter_path);

  while (!WindowShouldClose()) {
    SetWindowFocused();
    double currentTime = GetTime();

    // Check toggle pause with P
    if (IsKeyPressed(KEY_P)) {
      paused = !paused;
    }

    // Only do game logic updates if NOT paused
    if (!paused && currentTime - lastTick >= tickInterval) {
      tickCount++;
      lastTick = currentTime;

      if (tickCount >= max_ticks) {
        std::cout << "Game Timer elapsed - game over!" << std::endl;
        break;
      }

      if (player_health <= 0) {
        std::cout << "GAME OVER - YOU LOSE!" << std::endl;
        break;
      }

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

    // Also update the scroll for the bottom stats panel
    bottomPanelScrollOffset -= (int)(wheelMove * 20);
    if (bottomPanelScrollOffset < 0)
      bottomPanelScrollOffset = 0;
    {
      const int fontSize = 14;
      const int lineSpacing = fontSize + 2;
      int totalTextHeight = (int)towers.size() * lineSpacing;
      int visibleHeight = BOTTOM_PANEL_HEIGHT - 40;
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
                    << ") strategy changed to Lowest Health.\n";
        } else if (IsKeyPressed(KEY_TWO)) {
          existingTower->setStrategy(new HighestHealthTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Highest Health.\n";
        } else if (IsKeyPressed(KEY_THREE)) {
          existingTower->setStrategy(new StrongestTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Strongest.\n";
        } else if (IsKeyPressed(KEY_FOUR)) {
          existingTower->setStrategy(new FarthestTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Farthest Critter.\n";
        } else if (IsKeyPressed(KEY_FIVE)) {
          existingTower->setStrategy(new WeakestTargetStrategy());
          std::cout << "Tower at (" << row << "," << col
                    << ") strategy changed to Weakest Critter.\n";
        }
      }

      // Place regular tower (T)
      if (IsKeyPressed(KEY_T)) {
        if (!existingTower && player_points >= 100 &&
            (map->grid[row][col] == EMPTY)) {
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

      // Place or upgrade to freezing (F)
      if (IsKeyPressed(KEY_F)) {
        if (player_points >= 100 && map->grid[row][col] == EMPTY) {
          player_points -= 100;
          if (existingTower) {
            Tower *upgraded = new FreezingDecorator(existingTower, 0.5f);
            upgraded->setStrategy(new WeakestTargetStrategy());
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

      // Place or upgrade to sniper (S)
      if (IsKeyPressed(KEY_S)) {
        if (player_points >= 100 && map->grid[row][col] == EMPTY) {
          player_points -= 100;
          if (existingTower) {
            Tower *upgraded = new SniperDecorator(existingTower, 2, 10);
            upgraded->setStrategy(new WeakestTargetStrategy());
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

      // Place or upgrade to bomb (B)
      if (IsKeyPressed(KEY_B)) {
        if (player_points >= 100 && map->grid[row][col] == EMPTY) {
          player_points -= 100;
          if (existingTower) {
            Tower *upgraded = new BombDecorator(existingTower, 2, 0.5f);
            upgraded->setStrategy(new WeakestTargetStrategy());
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

    // Remove tower (X)
    if (IsKeyPressed(KEY_X)) {
      Vector2 pos = GetMousePosition();
      int col = pos.x / map->tileSize;
      int row = pos.y / map->tileSize;
      if (map->grid[row][col] == PATH) {
        // Just ignore removing from path
      } else {
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
    }

    // Level up (L)
    if (IsKeyPressed(KEY_L)) {
      Vector2 pos = GetMousePosition();
      int col = pos.x / map->tileSize;
      int row = pos.y / map->tileSize;
      for (Tower *t : towers) {
        if (t->getX() == row && t->getY() == col) {
          if (player_points < t->getLevelUpCost())
            break;
          player_points -= t->getLevelUpCost();
          t->levelUp();
          std::cout << "Tower " << t->getTid() << " leveled up to "
                    << t->getLevel() << std::endl;
        }
      }
    }

    // Quit (Q)
    if (IsKeyPressed(KEY_Q)) {
      break;
    }

    // ----- Draw Section -----
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Draw the map.
    map->Draw();

    // Draw the side panel.
    DrawRectangle(mapWidth, 0, sidePanelWidth, mapHeight, LIGHTGRAY);

    // Draw HUD (points + health).
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

    // Draw upgrade indicators + stats on tower tiles
    for (Tower *t : towers) {
      int tileX = t->getY() * map->tileSize;
      int tileY = t->getX() * map->tileSize;

      // Decorator letter
      if (dynamic_cast<FreezingDecorator *>(t) != nullptr) {
        DrawText("F", tileX + map->tileSize - 15, tileY + 5, 20, BLACK);
      } else if (dynamic_cast<SniperDecorator *>(t) != nullptr) {
        DrawText("S", tileX + map->tileSize - 15, tileY + 5, 20, BLACK);
      } else if (dynamic_cast<BombDecorator *>(t) != nullptr) {
        DrawText("B", tileX + map->tileSize - 15, tileY + 5, 20, BLACK);
      }

      // Tower level
      std::string levelText = "L" + std::to_string(t->getLevel());
      DrawText(levelText.c_str(), tileX + 5, tileY + map->tileSize - 25, 20,
               BLACK);

      // Build lines of stats
      std::ostringstream line1, line2, line3;
      line1 << "DMG:" << t->getDamage() << " Rng:" << t->getRange();
      line2 << "Rate:" << t->getAttaRate();
      line3 << "$:" << t->getResale();

      const int fontSize = 10;
      int lineSpacing = fontSize + 2;

      int w1 = MeasureText(line1.str().c_str(), fontSize);
      int w2 = MeasureText(line2.str().c_str(), fontSize);
      int w3 = MeasureText(line3.str().c_str(), fontSize);

      int centerX = tileX + map->tileSize / 2;
      int x1 = centerX - w1 / 2;
      int x2 = centerX - w2 / 2;
      int x3 = centerX - w3 / 2;

      int startY = tileY + (map->tileSize - (3 * fontSize + 2 * 2)) / 2;
      DrawText(line1.str().c_str(), x1, startY, fontSize, BLACK);
      DrawText(line2.str().c_str(), x2, startY + lineSpacing, fontSize, BLACK);
      DrawText(line3.str().c_str(), x3, startY + 2 * lineSpacing, fontSize,
               BLACK);

      // Tower ID
      std::string idText = std::to_string(t->getTid());
      int idTextWidth = MeasureText(idText.c_str(), 20);
      int bottomRightX = tileX + map->tileSize - idTextWidth - 5;
      int bottomRightY = tileY + map->tileSize - 25;
      DrawText(idText.c_str(), bottomRightX, bottomRightY, 20, BLACK);
    }
  
    // draw critter health above them
      for (Critter *c : generator.critters) {

          // critter position
          Vector2 pos = {
                  static_cast<float>(c->getCol() * map->tileSize),
                  static_cast<float>(c->getRow() * map->tileSize)
          };
          char healthText[10];
          snprintf(healthText, sizeof(healthText), "%d", c->getHealth());
          int fontSize = 30;
          int offset = 1;
          //  bold
          DrawText(healthText, pos.x + map->tileSize / 4 + offset, pos.y - 20 + offset, fontSize, ORANGE);
          DrawText(healthText, pos.x + map->tileSize / 4, pos.y - 20, fontSize, RED);  
      }

    // Draw the bottom panel with tower stats
    DrawTowerStatsPanel(0,                      // startX
                        mapHeight,              // startY => below the map
                        screenWidth,            // full width
                        BOTTOM_PANEL_HEIGHT,    // panel height
                        towers,                 // pass the tower list
                        bottomPanelScrollOffset // scroll offset
    );

    // If the game is paused, draw a "PAUSED" overlay
    if (paused) {
      int overlayWidth = 300;
      int overlayHeight = 80;
      int overlayX = (screenWidth - overlayWidth) / 2;
      int overlayY = (screenHeight - overlayHeight) / 2;

      // A semi-transparent rectangle behind the text
      DrawRectangle(overlayX, overlayY, overlayWidth, overlayHeight,
                    Fade(GRAY, 0.8f));
      DrawText("PAUSED", overlayX + 30, overlayY + 20, 40, BLACK);
      DrawText("Press P to Unpause", overlayX + 10, overlayY + 50, 20, BLACK);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
