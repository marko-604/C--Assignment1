#include "Tower.h"
#include "../Critters/Critter.h"
#include "../Maps/Map.h"
#include "TowerStrategy/SelectionStrategy.h"
#include <cmath>
#include <vector>

// Initialize the static member.
int Tower::nextId = 100;

// Constructor with parameters.
Tower::Tower(int x_val, int y_val, int cost_val, int damage_val,
             int attack_rate_val, float hit_rate_val, int range_val,
             TowerType type_val, int level_val, int level_up_cost_val,
             int resale_val, TargetSelectionStrategy *strat)
    : x(x_val), y(y_val), cost(cost_val), damage(damage_val),
      attack_rate(attack_rate_val), hit_rate(hit_rate_val), range(range_val),
      type(type_val), tid(nextId++), level(level_val),
      levelUpCost(level_up_cost_val), resale(resale_val), strategy(strat) {}

// Default constructor uses default values and a nullptr strategy.
Tower::Tower()
    : Tower(-1, -1, 100, 3, 3, 0.65, 1, REGULAR, 1, 50, 50, nullptr) {}

// Virtual destructor.
Tower::~Tower() {}

// Virtual getters.
int Tower::getTid() { return tid; }
int Tower::getX() { return x; }
int Tower::getY() { return y; }
int Tower::getCost() { return cost; }
int Tower::getDamage() { return damage; }
int Tower::getAttaRate() { return attack_rate; }
float Tower::getHitRate() { return hit_rate; }
int Tower::getRange() { return range; }
int Tower::getResale() { return resale; }
int Tower::getLevel() { return level; }
TowerType Tower::getType() { return type; }
int Tower::getLevelUpCost() { return levelUpCost; }

// Virtual setters.
void Tower::setLevel(int x_val) {
  level = x_val;
  Notify();
}
void Tower::setX(int x_val) {
  x = x_val;
  Notify();
}
void Tower::setY(int y_val) {
  y = y_val;
  Notify();
}
void Tower::setCost(int cost_val) {
  cost = cost_val;
  Notify();
}
void Tower::setDamage(int dmg_val) {
  damage = dmg_val;
  Notify();
}
void Tower::setAttackRate(int attackRate) {
  attack_rate = attackRate;
  Notify();
}
void Tower::setHitRate(float hit_rate_val) {
  hit_rate = hit_rate_val;
  Notify();
}
void Tower::setRange(int range_val) {
  range = range_val;
  Notify();
}
void Tower::setResale(int resale_val) {
  resale = resale_val;
  Notify();
}

// Virtual operations.
void Tower::levelUp() {
  damage += 1;
  level += 1;
  attack_rate -= 0.5;
  levelUpCost = levelUpCost * 1.35; // We increase the cost of leveling up
                                    // by 1.35 each time 35% more expensive
}

bool Tower::attack(std::vector<Critter *> &critters, int tick_count,
                   int *player_points, Map &gameMap) {
  if (tick_count % attack_rate == 0) {
    Critter *target = nullptr;
    // Use the strategy if one is set.
    if (strategy != nullptr) {
      target = strategy->selectTarget(critters, this);
    }
    // Fallback: if no target selected by strategy, default behavior.
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
    // If a target was found, perform the attack.
    if (target != nullptr) {
      target->setHealth(target->getHealth() - getDamage());
      std::cout << "Tower " << getTid() << " attacked a critter." << std::endl;
      if (target->getHealth() <= 0) {
        std::cout << "Critter eliminated!" << std::endl;
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
        (*player_points) += target->getValue();
        // You would remove the critter from the vector here.
      }
      return true;
    }
  }
  return false;
}

void Tower::setLevelUpCost(int x) { levelUpCost = x; }
// Strategy management:
void Tower::setStrategy(TargetSelectionStrategy *strat) { strategy = strat; }

TargetSelectionStrategy *Tower::getStrategy() const { return strategy; }
