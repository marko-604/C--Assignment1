#include "Critter.h"
#include "../Maps/Map.h"
#include <utility>
#include <vector>

// We create an id for each critter in the game.
int Critter::nextCID = 100;
Critter::Critter(int hlt_val, int spd_val, int str_val, int row_val,
                 int col_val, CritterType type_val,
                 std::vector<std::pair<int, int>> path_val)
    : health(hlt_val), speed(spd_val), strength(str_val), row(row_val),
      col(col_val), type(type_val), path(path_val), cid(nextCID++) {}

CritterType Critter::getType() { return type; }

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
  if (tick_count % speed != 0) {
    return; // Do nothing
  }

  // If the path is empty this means that we have reached the end of the path
  // eg. we are at the exit
  if (!path.empty()) { // Ensure path is not empty before popping
    map.setToPath(row, col);
    std::pair<int, int> new_tile = path.back(); // Get the last element
    path.pop_back();                            // Remove the last element

    row = new_tile.first;
    col = new_tile.second;
    Notify();
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
