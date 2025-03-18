#include "Critters/Critter.h"
#include "Maps/Map.h"
#include "Observer/CritterObserver.h"
#include "Observer/MapObserver.h"
#include "Observer/Subject.h"
#include "Observer/TowerObserver.h"
#include "Towers/Tower.h"
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
  const int lineSpacing = fontSize + 2; // spacing per line

  // Draw a title above the scrolling region.
  DrawText("Observer Output:", startX, startY, fontSize, BLACK);

  // Define the rectangle for the scrollable area (below the title).
  int regionY = startY + lineSpacing;
  int regionHeight = availableHeight - lineSpacing;

  // Set scissor mode to clip drawing within the observer output region.
  BeginScissorMode(startX, regionY, panelWidth, regionHeight);

  // Draw all messages with a vertical offset.
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

  // Create the map. (Map(width, height, tileSize) where width = cols and height
  // = rows)
  Map *map = new Map(cols, rows, 80);

  // Create a shared messages vector for observer updates.
  std::vector<std::string> messages;
  // Create and attach the MapObserver using a pointer to the messages vector.
  MapObserver *m_obs = new MapObserver(&messages);
  map->Attach(m_obs);
  bool isValidMap = map->RunEditor();

  if (isValidMap) {
    int mapWidth = map->gridWidth * map->tileSize;
    int mapHeight = map->gridHeight * map->tileSize;

    // Define the side panel dimensions.
    int sidePanelWidth = 300;
    int screenWidth = mapWidth + sidePanelWidth;
    int screenHeight = mapHeight;

    InitWindow(screenWidth, screenHeight, "Game");

#ifdef _WIN32
    // Force the window to be focused on Windows.
    HWND handle = GetWindowHandle();
    SetForegroundWindow(handle);
#endif

    SetTargetFPS(60);

    std::vector<Tower *> towers;
    CritterGenerator generator;
    std::vector<std::pair<int, int>> critter_path = map->getPath();

    // Define side panel layout.
    int hudY = 10;
    int hudFontSize = 20;
    int hudHeight = hudFontSize + 10;

    // Player HUD values.
    int player_points = 1000;
    int player_health = 1000;

    // HUD is drawn at the top of the side panel.
    // Legend area starts below the HUD.
    int legendStartY = hudY + hudHeight + 10;
    int legendHeight = 12 * 30; // fixed height for legend area

    // Observer output area starts below the legend.
    int observerOutputY = legendStartY + legendHeight + 20;
    int observerPanelWidth = sidePanelWidth - 20;
    int availableHeightForObserver = mapHeight - observerOutputY - 10;
    int scrollOffset = 0;

    // Ticking system: tick interval of 1 second.
    double tickInterval = 1.0; // in seconds
    double lastTick = GetTime();
    int tickCount = 0;

    generator.levelUp(critter_path);
    while (!WindowShouldClose()) {
      SetWindowFocused(); // Ensure the window stays focused

      // --- Tick System ---
      double currentTime = GetTime();
      if (currentTime - lastTick >= tickInterval) {
        tickCount++; // increment global tick
        lastTick = currentTime;

        // Update towers: each tower attempts an attack.
        for (Tower *t : towers) {
          t->attack(generator.critters, tickCount, &player_points, *map);
        }
        // Update critters: move them according to the tick.
        for (Critter *c : generator.critters) {
          if (c->getCol() == map->exitCol && c->getRow() == map->exitRow) {
            player_health -= c->getStr();
            continue; // Skip we are at the end point so we do damage to the
                      // player.
          }
          c->Update(*map, tickCount);
          map->ToggleCritter(c, c->getRow(), c->getCol());
        }

        if (tickCount % 10 == 0) {
          generator.levelUp(critter_path);
        }
      }

      // --- Update Scroll Offset via Mouse Wheel ---
      float wheelMove = GetMouseWheelMove(); // positive when scrolling up
      scrollOffset -= static_cast<int>(
          wheelMove * 20); // adjust sensitivity (20 pixels per notch)
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

      // --- Process Input for Map (only if mouse is in the map area) ---
      if (GetMouseX() < mapWidth && GetMouseY() < mapHeight) {
        if (IsKeyPressed(KEY_T)) {
          if (player_points >= 100) {
            player_points -= 100;
            Vector2 position = GetMousePosition();
            Tower *t = new Tower();
            TowerObserver *obs = new TowerObserver();
            t->Attach(obs);
            int col = position.x / map->tileSize;
            int row = position.y / map->tileSize;
            t->setX(row);
            t->setY(col);
            map->ToggleTower(t, row, col);
            towers.push_back(t);
          }
        }
        if (IsKeyPressed(KEY_N)) {
          if (player_points >= 150) {
            player_points -= 150;
            Vector2 position = GetMousePosition();
            SniperTower *t = new SniperTower();
            TowerObserver *obs = new TowerObserver();
            t->Attach(obs);
            int col = position.x / map->tileSize;
            int row = position.y / map->tileSize;
            t->setX(row);
            t->setY(col);
            map->ToggleTower(t, row, col);
            towers.push_back(t);
          }
        }
        if (IsKeyPressed(KEY_F)) {
          if (player_points >= 200) {
            player_points -= 200;
            Vector2 position = GetMousePosition();
            FreezingTower *t = new FreezingTower(1);
            TowerObserver *obs = new TowerObserver();
            t->Attach(obs);
            int col = position.x / map->tileSize;
            int row = position.y / map->tileSize;
            t->setX(row);
            t->setY(col);
            map->ToggleTower(t, row, col);
            towers.push_back(t);
          }
        }
        if (IsKeyPressed(KEY_B)) {
          if (player_points >= 200) {
            player_points -= 200;
            Vector2 position = GetMousePosition();
            BombTower *t = new BombTower(1);
            TowerObserver *obs = new TowerObserver();
            t->Attach(obs);
            int col = position.x / map->tileSize;
            int row = position.y / map->tileSize;
            t->setX(row);
            t->setY(col);
            map->ToggleTower(t, row, col);
            towers.push_back(t);
          }
        }
        if (IsKeyPressed(KEY_L)) {
          Vector2 position = GetMousePosition();
          int col = position.x / map->tileSize;
          int row = position.y / map->tileSize;
          for (Tower *t : towers) {
            if (t->getX() == row && t->getY() == col) {
              if (player_points >= t->getCost()) {
                player_points -= t->getCost();
                t->levelUp();
              }
            }
          }
        }
      }
      if (IsKeyPressed(KEY_X)) {
        Vector2 position = GetMousePosition();
        int col = position.x / map->tileSize;
        int row = position.y / map->tileSize;

        map->setToScenery(row, col);

        for (auto it = towers.begin(); it != towers.end();) {
          player_points += (*it)->getCost();
          delete *it;
          it = towers.erase(it);
        }
      }
      if (IsKeyPressed(KEY_Q)) {
        break;
      }

      BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw the map in the left area.
      map->Draw();

      // Draw the side panel background.
      DrawRectangle(mapWidth, 0, sidePanelWidth, mapHeight, LIGHTGRAY);

      // Draw the HUD (player points and health) at the top of the side panel.
      std::string hudText =
          TextFormat("Points: %d   Health: %d", player_points, player_health);
      DrawText(hudText.c_str(), mapWidth + 10, 10, 20, BLACK);

      // Draw the legend below the HUD.
      int legendStartY = 10 + 20 + 10;
      DrawLegend(mapWidth + 10, legendStartY);

      // Observer output area: start below the legend.
      int innerLegendHeight = 12 * 30; // adjust if needed
      int observerOutputYFinal = legendStartY + innerLegendHeight + 20;
      int availableHeightForObserver = mapHeight - observerOutputYFinal - 10;
      DrawObserverOutputScrollable(
          mapWidth + 10, observerOutputYFinal, sidePanelWidth - 20,
          availableHeightForObserver, messages, scrollOffset);

      EndDrawing();
    }
    CloseWindow();
  } else {
    std::cout << "INVALID MAP!!" << std::endl;
  }

  // Here we will need to free any resources before the program terminates.

  return 0;
}
