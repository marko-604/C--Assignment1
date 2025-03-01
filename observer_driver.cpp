#include "Critters/Critter.h"
#include "Maps/Map.h"
#include "Observer/CritterObserver.h"
#include "Observer/MapObserver.h"
#include "Observer/Subject.h"
#include "Observer/TowerObserver.h"
#include "Towers/Tower.h"
#include <algorithm>
#include <utility>
#include <vector>

int main() {

  Map *map = new Map(10, 10, 80);
  MapObserver *m_obs = new MapObserver();
  map->Attach(m_obs);
  bool isValidMap = map->RunEditor();

  if (isValidMap) {
    // Play the game loop.

    int screenWidth = map->gridWidth * map->tileSize;
    int screenHeight = map->gridHeight * map->tileSize;
    std::vector<std::pair<int, int>> path = map->getPath();
    InitWindow(screenWidth, screenHeight, "Game");
    SetTargetFPS(60);

    std::vector<Tower *> towers;
    std::vector<Critter *> critters;

    std::vector<std::pair<int, int>> critter_path = map->getPath();
    while (!WindowShouldClose()) {

      if (IsKeyPressed(KEY_T)) {
        SetWindowFocused();
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
      if (IsKeyPressed(KEY_N)) {
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
      if (IsKeyPressed(KEY_F)) {
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
      if (IsKeyPressed(KEY_B)) {
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
      if (IsKeyPressed(KEY_L)) {
        Vector2 position = GetMousePosition();

        int col = position.x / map->tileSize;
        int row = position.y / map->tileSize;

        for (Tower *t : towers) {
          if (t->getX() == row && t->getY() == col) {
            t->levelUp();
          }
        }
      }
      if (IsKeyPressed(KEY_C)) {
        Vector2 position = GetMousePosition();
        int col = position.x / map->tileSize;
        int row = position.y / map->tileSize;

        Squirrel *s = new Squirrel(critter_path);
        CritterObserver *obs = new CritterObserver();
        s->Attach(obs);

        s->setRow(row);
        s->setCol(col);
        map->ToggleCritter(s, row, col);
        critters.push_back(s);
      } else if (IsKeyPressed(KEY_W)) {
        Vector2 position = GetMousePosition();
        int col = position.x / map->tileSize;
        int row = position.y / map->tileSize;

        Wolf *w = new Wolf(critter_path);
        CritterObserver *obs = new CritterObserver();
        w->Attach(obs);

        w->setRow(row);
        w->setCol(col);
        map->ToggleCritter(w, row, col);
        critters.push_back(w);
      } else if (IsKeyPressed(KEY_R)) {
        Vector2 position = GetMousePosition();
        int col = position.x / map->tileSize;
        int row = position.y / map->tileSize;

        Bear *w = new Bear(critter_path);
        CritterObserver *obs = new CritterObserver();
        w->Attach(obs);

        w->setRow(row);
        w->setCol(col);
        map->ToggleCritter(w, row, col);
        critters.push_back(w);
      }

      if (IsKeyPressed(KEY_A)) {
        for (Tower *t : towers) {
          t->attack(critters, 0);
        }
      }
      if (IsKeyPressed(KEY_M)) {

        // Good pattern
        for (auto it = critters.begin(); it != critters.end();
             /* no ++it here */) {
          Critter *c = *it;
          if (c->getHealth() <= 0) {
            // 1) Detach Observers, remove from the map, etc.
            c->DetachAll();
            // Unless we are at the exit tile
            if (c->getCol() == map->exitCol && c->getRow() == map->exitRow) {
              map->grid[c->getRow()][c->getCol()] = EXIT;
            } else
              map->setToPath(c->getRow(), c->getCol());
            // 2) Delete the critter
            delete c;
            // 3) Erase the pointer from the vector
            it = critters.erase(it);
          } else {
            c->Update(*map, 0);
            map->ToggleCritter(c, c->getRow(), c->getCol());
            ++it;
          }
        }
      }
      if (IsKeyPressed(KEY_Q)) {
        break;
      }
      BeginDrawing();
      ClearBackground(RAYWHITE);
      map->Draw();
      EndDrawing();
    }
    CloseWindow();
  } else {
    std::cout << "INVALID MAP!!" << std::endl;
  }

  return 0;
}
