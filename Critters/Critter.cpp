#include "Critter.h"
#include "../Maps/Map.h"
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

/// Static member to assign a unique ID to each critter.
int Critter::nextCID = 100;

/**
 * @brief Construct a new Critter object.
 *
 * @param hlt_val Initial health.
 * @param spd_val Movement speed (ticks per move).
 * @param str_val Strength value.
 * @param row_val Starting row.
 * @param col_val Starting column.
 * @param type_val Type of the critter.
 * @param path_val Movement path for the critter.
 * @param val Value (e.g., reward when defeated).
 */
Critter::Critter(int hlt_val, int spd_val, int str_val, int row_val,
                 int col_val, CritterType type_val,
                 std::vector<std::pair<int, int>> path_val, int val)
    : health(hlt_val), speed(spd_val), strength(str_val), row(row_val),
      col(col_val), type(type_val), path(path_val), cid(nextCID++),
      pathIndex(0), value(val) {}

/**
 * @brief Get the critter type.
 * @return CritterType
 */
CritterType Critter::getType() { return type; }

/**
 * @brief Get the value of the critter.
 * @return int
 */
int Critter::getValue() { return value; }

/**
 * @brief Get the current index in the path.
 * @return int
 */
int Critter::getPathIndex() { return pathIndex; }

/**
 * @brief Get the current row position.
 * @return int
 */
int Critter::getRow() { return row; }

/**
 * @brief Get the critter's unique ID.
 * @return int
 */
int Critter::getCID() { return cid; }

/**
 * @brief Get the current column position.
 * @return int
 */
int Critter::getCol() { return col; }

/**
 * @brief Get the critter's current health.
 * @return int
 */
int Critter::getHealth() { return health; }

/**
 * @brief Get the critter's movement speed.
 * @return int
 */
int Critter::getSpeed() { return speed; }

/**
 * @brief Get the critter's strength.
 * @return int
 */
int Critter::getStr() { return strength; }

/**
 * @brief Get the path the critter is following.
 * @return std::vector<std::pair<int, int>>
 */
std::vector<std::pair<int, int>> Critter::getPath() { return path; }

/**
 * @brief Set the row position of the critter.
 * @param x New row value.
 */
void Critter::setRow(int x) {
  row = x;
  Notify();
}

/**
 * @brief Set the column position of the critter.
 * @param x New column value.
 */
void Critter::setCol(int x) {
  col = x;
  Notify();
}

/**
 * @brief Set the health value.
 * @param x New health value.
 */
void Critter::setHealth(int x) {
  health = x;
  Notify();
}

/**
 * @brief Set the movement speed.
 * @param x New speed value.
 */
void Critter::setSpeed(int x) {
  speed = x;
  Notify();
}

/**
 * @brief Set the strength value.
 * @param s New strength value.
 */
void Critter::setStr(int s) {
  strength = s;
  Notify();
}

/**
 * @brief Update critter's state on each game tick.
 *
 * @param map Reference to the game map.
 * @param tick_count Current tick count.
 */
void Critter::Update(Map &map, int tick_count) {

  if (this == NULL || this == nullptr) {
    return;
  }

  // Only move once every `speed` ticks
  if (tick_count % speed != 0) {
    return;
  }

  // If path is empty, we're done moving
  if (path.empty()) {
    return;
  }

  // If not placed yet, set to entry position
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
      while ((int)path.size() > foundIndex) {
        path.pop_back();
      }
    }
  }

  if (row == map.exitRow && col == map.exitCol) {
    return;
  }

  // Move to next tile in path
  if (!path.empty()) {
    map.setToPath(row, col); // reset previous tile
    std::pair<int, int> new_tile = path.back();
    path.pop_back();
    row = new_tile.first;
    col = new_tile.second;
    Notify();
  }
}

/**
 * @brief Move the critter to a specified location (currently incomplete).
 *
 * @param map Reference to the game map.
 * @param row Target row.
 * @param col Target column.
 */
void Critter::move(Map &map, int row, int col) {
  bool current_found = false;
  if (row == -1 && col == -1) {
    row = map.entryRow;
    col = map.entryCol;
    return;
  }
}

/**
 * @brief Check if there are any critters left to deploy.
 * @return true if empty, false otherwise.
 */
bool CritterGenerator::isEmpty() { return critters.empty(); }

/**
 * @brief Get the next critter in the queue.
 * @return Pointer to a Critter.
 */
Critter *CritterGenerator::getCritter() {
  Critter *p = critters.back();
  critters.pop_back();
  return p;
}

/**
 * @brief Construct a new Critter Generator object.
 */
CritterGenerator::CritterGenerator() : level(1) {}

/**
 * @brief Generate critters based on the current level.
 *
 * @param path The path that generated critters will follow.
 */
void CritterGenerator::generateCritters(std::vector<std::pair<int, int>> path) {
  for (int i = 0; i <= (level * 5); i++) {
    if (i % 2 == 0 && i % 3 != 0 && i % 5 != 0)
      critters.push_back(new Squirrel(path));
    else if (i % 3 == 0)
      critters.push_back(new Wolf(path));
    else if (i % 5 == 0)
      critters.push_back(new Bear(path));
  }
}

/**
 * @brief Increase the generator level and generate more critters.
 *
 * @param path The path to assign to newly generated critters.
 */
void CritterGenerator::levelUp(std::vector<std::pair<int, int>> path) {
  level += 1;
  generateCritters(path);
}
