#include "Critters/Critter.h"
#include "Maps/Map.h"
#include "Towers/Tower.h"
#include <ostream>
#include <utility>
#include <vector>

int main() {

  std::vector<Projectile *> projectiles;
  std::vector<Critter *> critters;
  std::vector<Tower *> towers;
  int rows, cols;

  std::cout << "Enter the number of rows: ";
  std::cin >> rows;
  std::cout << "Enter the number of columns: ";
  std::cin >> cols;

  Map gameMap(rows, cols, 80);

  bool isValidMap = gameMap.RunEditor();

  int player_health = 1000;
  int points = 100000;

  int tick_count = 0;
  int max_tick_count = 1000;

  CritterGenerator *generator = new CritterGenerator();
  if (isValidMap) {
    int screenWidth = gameMap.gridWidth * gameMap.tileSize;
    int screenHeight = gameMap.gridHeight * gameMap.tileSize;
    std::vector<std::pair<int, int>> path = gameMap.getPath();
    generator->generateCritters(path);
    InitWindow(screenWidth, screenHeight, "Game");
    SetTargetFPS(60);

    float timeAccum = 0.0f;

    SetWindowFocused();
    while (!WindowShouldClose()) {

      bool ticked = false;
      float dt = GetFrameTime();
      timeAccum += dt;
      if (timeAccum >= 1.0f) {
        timeAccum -= 1.0f;
        tick_count++;
        ticked = true;
        std::cout << "Tick count: " << tick_count << std::endl;
      }

      if (ticked) {

        if (tick_count % 10 == 0) {
          generator->levelUp(path);
        }

        if (tick_count % 2 == 0 && !generator->isEmpty()) {
          std::cout << "Added a critter to the game..." << std::endl;
          critters.push_back(generator->getCritter());
        } else {
        }

        for (Tower *t : towers) {
          t->attack(critters, tick_count);
        }
        for (auto it = critters.begin(); it != critters.end();) {
          std::cout << "HERE1" << std::endl;
          Critter *c = *it;
          if (c->getHealth() <= 0) {
            // revert tile
            gameMap.setToPath(c->getRow(), c->getCol());

            // free memory if you used 'new'

            it = critters.erase(it);
          } else if (c->getRow() == gameMap.exitRow &&
                     c->getCol() == gameMap.exitCol) {

            // Critter attacks the player.
            player_health -= c->getStr();
            std::cout << "Critter has done " << c->getStr()
                      << " to the player, player now has " << player_health
                      << " health left" << std::endl;

          } else {
            std::cout << "Updating critter location ..." << std::endl;
            c->Update(gameMap, tick_count);
            gameMap.ToggleCritter(c, c->getRow(), c->getCol());
            ++it;
          }
        }
      }

      if (IsKeyPressed(KEY_T)) {
        if (points >= 100) {
          points -= 100;
          Vector2 position = GetMousePosition();
          int col = position.x / gameMap.tileSize;
          int row = position.y / gameMap.tileSize;
          Tower *t = new Tower();
          t->setX(row);
          t->setY(col);
          towers.push_back(t);
          gameMap.ToggleTower(t, row, col);
        }
      }
      if (IsKeyPressed(KEY_S)) {
        if (points >= 150) {
          points -= 150;
          Vector2 position = GetMousePosition();
          int col = position.x / gameMap.tileSize;
          int row = position.y / gameMap.tileSize;
          SniperTower *t = new SniperTower();
          t->setX(row);
          t->setY(col);
          towers.push_back(t);
          gameMap.ToggleTower(t, row, col);
        }
      }
      if (IsKeyPressed(KEY_B)) {
        if (points >= 175) {
          points -= 175;
          Vector2 position = GetMousePosition();
          int col = position.x / gameMap.tileSize;
          int row = position.y / gameMap.tileSize;
          BombTower *t = new BombTower(1);
          t->setX(row);
          t->setY(col);
          towers.push_back(t);
          gameMap.ToggleTower(t, row, col);
        }
      }

      if (IsKeyPressed(KEY_F)) {
        if (points >= 200) {
          points -= 200;
          Vector2 position = GetMousePosition();
          int col = position.x / gameMap.tileSize;
          int row = position.y / gameMap.tileSize;
          FreezingTower *t = new FreezingTower(1);
          t->setX(row);
          t->setY(col);
          towers.push_back(t);
          gameMap.ToggleTower(t, row, col);
        }
      }

      if (IsKeyPressed(KEY_R)) {

        Vector2 position = GetMousePosition();
        int col = position.x / gameMap.tileSize;
        int row = position.y / gameMap.tileSize;

        // We will have to sell the tower in the game this can be simple or
        // complicated.
      }
      if (IsKeyPressed(KEY_Q)) {
        break;
      }

      BeginDrawing();
      ClearBackground(RAYWHITE);
      gameMap.Draw();
      EndDrawing();

      // We want to check if the player
    }
    CloseWindow();

  } else {
    std::cout << "Map is invalid!" << std::endl;
  }

  return 0;
}
