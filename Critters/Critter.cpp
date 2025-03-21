#include "Critter.h"
#include "../Maps/Map.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

// We create an id for each critter in the game.
int Critter::nextCID = 100;
Critter::Critter(int hlt_val, int spd_val, int str_val, int row_val,
                 int col_val, CritterType type_val,
                 std::vector<std::pair<int, int>> path_val, int val)
    : health(hlt_val), speed(spd_val), strength(str_val), row(row_val),
      col(col_val), type(type_val), path(path_val), cid(nextCID++),
      pathIndex(0), value(val) {}

CritterType Critter::getType() { return type; }

int Critter::getValue() { return value; }

int Critter::getPathIndex() { return pathIndex; }

int Critter::getRow() { return row; }

int Critter::getCID() { return cid; }

int Critter::getCol() { return col; }

int Critter::getHealth() { return health; }

int Critter::getSpeed() { return speed; }

int Critter::getStr() { return strength; }

std::vector<std::pair<int, int>> Critter::getPath() { return path; }

void Critter::setRow(int x) {
  row = x;
  Notify();
}

void Critter::setCol(int x) {
  col = x;
  Notify();
}

void Critter::setHealth(int x) {
  health = x;
  Notify();
}

void Critter::setSpeed(int x) {
  speed = x;
  Notify();
}

void Critter::setStr(int s) {
  strength = s;
  Notify();
}

void Critter::Update(Map &map, int tick_count) {

  // How can we solve the problem from here?

  if (this == NULL || this == nullptr) {
    return;
  }

  // Only move once every `speed` ticks
  if (tick_count % speed != 0) {
    return; // do nothing this tick
  }

  // If the path is empty, we’re effectively at or past the exit
  if (path.empty()) {
    return;
  }

  // We place the critter along the path if it is not already on it.
  if (row == -1 && col == -1) {
    row = map.entryRow;
    col = map.entryCol;
  }
  TileType currentTile = map.grid[row][col];

  if (currentTile == PATH || currentTile == ENTRY) {
    int foundIndex = -1;
    for (int i = (int)path.size() - 1; i >= 0; i--) {
      if (path[i].first == row && path[i].second == col) {
        foundIndex = i;
        break;
      }
    }

    if (foundIndex != -1) {
      // Pop all elements beyond `foundIndex`, so the next tile to move onto
      // will be `path[foundIndex - 1]` (assuming foundIndex > 0).
      while ((int)path.size() > foundIndex) {
        path.pop_back();
      }
    }
  }

  if (row == map.exitRow && col == map.exitCol) {
    return;
  }

  // 2) Now pick up the “next” tile in the path by popping back
  //    and moving there. (This is the same logic you already had.)
  if (!path.empty()) {
    // Optionally mark our old tile back to PATH
    map.setToPath(row, col);

    // Move onto path.back()
    std::pair<int, int> new_tile = path.back();
    path.pop_back();

    row = new_tile.first;
    col = new_tile.second;

    // Notify observers
    Notify();
  }
}

void Critter::move(Map &map, int row, int col) {
  // First we need to get the current tile that the critter is on.

  bool current_found = false;
  if (row == -1 && col == -1) {
    row = map.entryRow;
    col = map.entryCol;
    return;
  }
}

bool CritterGenerator::isEmpty() { return critters.empty(); }
Critter *CritterGenerator::getCritter() {
  Critter *p = critters.back();
  critters.pop_back();
  return p;
}

CritterGenerator::CritterGenerator() : level(1) {}
// This will generate the critters based on the level of the critter generator.
void CritterGenerator::generateCritters(
    std ::vector<std::pair<int, int>> path) {
  for (int i = 0; i <= (level * 5); i++) {
    if (i % 2 == 0 && i % 3 != 0 && i % 5 != 0)
      critters.push_back(new Squirrel(path));
    else if (i % 3 == 0)
      critters.push_back(new Wolf(path));
    else if (i % 5 == 0)
      critters.push_back(new Bear(path));
  }
}

void CritterGenerator::levelUp(std::vector<std::pair<int, int>> path) {
  level += 1;
  generateCritters(path);
}

// We will change the generator so that it works as so, we generate the number
// of critters based on a tikc just as in the original
