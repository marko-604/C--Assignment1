#include "Critter.h"
#include "../Maps/Map.h"
#include <utility>
#include <vector>

Critter::Critter(int hlt_val, int spd_val, int str_val, int row_val,
                 int col_val, CritterType type_val,
                 std::vector<std::pair<int, int>> path_val)
    : health(hlt_val), speed(spd_val), strength(str_val), row(row_val),
      col(col_val), type(type_val), path(path_val) {}

CritterType Critter::getType() { return type; }

int Critter::getRow() { return row; }

int Critter::getCol() { return col; }

int Critter::getHealth() { return health; }

int Critter::getSpeed() { return speed; }

int Critter::getStr() { return strength; }

std::vector<std::pair<int, int>> Critter::getPath() { return path; }

void Critter::setRow(int x) { row = x; }

void Critter::setCol(int x) { col = x; }

void Critter::setHealth(int x) { health = x; }

void Critter::setSpeed(int x) { speed = x; }

void Critter::setStr(int s) { strength = s; }

// So simple!

void Critter::Update(Map &map, int tick_count) {
  if (tick_count % speed != 0) {
    return; // Do nothing
  }

  if (!path.empty()) { // Ensure path is not empty before popping
    map.setToPath(row, col);
    std::pair<int, int> new_tile = path.back(); // Get the last element
    path.pop_back();                            // Remove the last element

    row = new_tile.first;
    col = new_tile.second;
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
  for (int i = 0; i <= (level * 3); i++) {
    if (i % 2 == 0)
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
