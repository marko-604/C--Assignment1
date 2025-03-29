#include "Tower.h"
#include "../Critters/Critter.h"
#include "../Maps/Map.h"
#include "TowerStrategy/SelectionStrategy.h"
#include <cmath>
#include <vector>

/// @brief Static member initialization for assigning unique Tower IDs.
int Tower::nextId = 100;

/**
 * @brief Construct a new Tower object with parameters.
 *
 * @param x_val X position.
 * @param y_val Y position.
 * @param cost_val Cost to place the tower.
 * @param damage_val Damage dealt per attack.
 * @param attack_rate_val Number of ticks between attacks.
 * @param hit_rate_val Hit probability (0.0 to 1.0).
 * @param range_val Attack range in tiles.
 * @param type_val Type of the tower (REGULAR, SNIPER, etc.).
 * @param level_val Initial level.
 * @param level_up_cost_val Cost to upgrade.
 * @param resale_val Value received when selling the tower.
 * @param strat Target selection strategy.
 */
Tower::Tower(int x_val, int y_val, int cost_val, int damage_val,
             int attack_rate_val, float hit_rate_val, int range_val,
             TowerType type_val, int level_val, int level_up_cost_val,
             int resale_val, TargetSelectionStrategy *strat)
    : x(x_val), y(y_val), cost(cost_val), damage(damage_val),
      attack_rate(attack_rate_val), hit_rate(hit_rate_val), range(range_val),
      type(type_val), tid(nextId++), level(level_val),
      levelUpCost(level_up_cost_val), resale(resale_val), strategy(strat) {}

/**
 * @brief Default constructor for Tower with safe default values.
 */
Tower::Tower()
    : Tower(-1, -1, 100, 3, 3, 0.65, 1, REGULAR, 1, 50, 50, nullptr) {}

/**
 * @brief Virtual destructor.
 */
Tower::~Tower() {}

// ---------------- Getters ----------------

/** @brief Get tower ID. */
int Tower::getTid() { return tid; }
/** @brief Get X position. */
int Tower::getX() { return x; }
/** @brief Get Y position. */
int Tower::getY() { return y; }
/** @brief Get cost. */
int Tower::getCost() { return cost; }
/** @brief Get damage per attack. */
int Tower::getDamage() { return damage; }
/** @brief Get attack rate (ticks). */
int Tower::getAttaRate() { return attack_rate; }
/** @brief Get hit probability. */
float Tower::getHitRate() { return hit_rate; }
/** @brief Get attack range. */
int Tower::getRange() { return range; }
/** @brief Get resale value. */
int Tower::getResale() { return resale; }
/** @brief Get current upgrade level. */
int Tower::getLevel() { return level; }
/** @brief Get tower type. */
TowerType Tower::getType() { return type; }
/** @brief Get upgrade cost. */
int Tower::getLevelUpCost() { return levelUpCost; }

// ---------------- Setters ----------------

/** @brief Set level. */
void Tower::setLevel(int x_val) {
  level = x_val;
  Notify();
}

/** @brief Set X position. */
void Tower::setX(int x_val) {
  x = x_val;
  Notify();
}

/** @brief Set Y position. */
void Tower::setY(int y_val) {
  y = y_val;
  Notify();
}

/** @brief Set cost. */
void Tower::setCost(int cost_val) {
  cost = cost_val;
  Notify();
}

/** @brief Set damage value. */
void Tower::setDamage(int dmg_val) {
  damage = dmg_val;
  Notify();
}

/** @brief Set attack rate. */
void Tower::setAttackRate(int attackRate) {
  attack_rate = attackRate;
  Notify();
}

/** @brief Set hit rate. */
void Tower::setHitRate(float hit_rate_val) {
  hit_rate = hit_rate_val;
  Notify();
}

/** @brief Set range value. */
void Tower::setRange(int range_val) {
  range = range_val;
  Notify();
}

/** @brief Set resale value. */
void Tower::setResale(int resale_val) {
  resale = resale_val;
  Notify();
}

/** @brief Set level-up cost. */
void Tower::setLevelUpCost(int x) { levelUpCost = x; }

/**
 * @brief Upgrade the tower's stats.
 *
 * Increases damage, reduces attack rate (faster attacks),
 * and increases the cost for next level.
 */
void Tower::levelUp() {
  damage += 1;
  level += 1;
  attack_rate -= 0.5;
  levelUpCost = levelUpCost * 1.35;
}

/**
 * @brief Attempts to attack a critter if it's in range and cooldown allows.
 *
 * @param critters Vector of critters on the map.
 * @param tick_count Current tick for timing.
 * @param player_points Pointer to player's score to update if a critter is
 * eliminated.
 * @param gameMap Reference to the map for updating tile states.
 * @return true if an attack was performed.
 * @return false otherwise.
 */
bool Tower::attack(std::vector<Critter *> &critters, int tick_count,
                   int *player_points, Map &gameMap) {
  if (tick_count % attack_rate == 0) {
    Critter *target = nullptr;

    // Use assigned targeting strategy if available
    if (strategy != nullptr) {
      target = strategy->selectTarget(critters, this);
    }

    // Fallback: simple range-based targeting
    if (target == nullptr) {
      for (auto it = critters.begin(); it != critters.end();) {
        Critter *p = *it;
        int row = p->getRow();
        int col = p->getCol();
        int current_row = getX();
        int current_col = getY();
        int distance = static_cast<int>(
            std::sqrt((current_row - row) * (current_row - row) +
                      (current_col - col) * (current_col - col)));
        if (distance <= getRange()) {
          target = p;
          break;
        } else {
          ++it;
        }
      }
    }

    if (target != nullptr) {
      target->setHealth(target->getHealth() - getDamage());
      std::cout << "Tower " << getTid() << " attacked a critter." << std::endl;

      if (target->getHealth() <= 0) {
        std::cout << "Critter eliminated!" << std::endl;

        // Restore tile to appropriate type after critter is removed
        if (target->getRow() == gameMap.entryRow &&
            target->getCol() == gameMap.entryCol) {
          gameMap.grid[target->getRow()][target->getCol()] = ENTRY;
        } else if (target->getRow() == gameMap.exitRow &&
                   target->getCol() == gameMap.exitCol) {
          gameMap.grid[target->getRow()][target->getCol()] = EXIT;
        } else {
          if (target->getRow() >= 0 && target->getCol() >= 0)
            gameMap.grid[target->getRow()][target->getCol()] = PATH;
        }

        // Update player points with critter's value
        (*player_points) += target->getValue();
        // NOTE: Critter should be removed from vector in calling code
      }

      return true;
    }
  }
  return false;
}

/**
 * @brief Set the target selection strategy.
 *
 * @param strat Pointer to a TargetSelectionStrategy object.
 */
void Tower::setStrategy(TargetSelectionStrategy *strat) { strategy = strat; }

/**
 * @brief Get the current targeting strategy.
 *
 * @return TargetSelectionStrategy*
 */
TargetSelectionStrategy *Tower::getStrategy() const { return strategy; }
