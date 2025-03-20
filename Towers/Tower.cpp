#include "tower.h"
#include "../Critters/Critter.h"
#include "../Maps/Map.h"
#include <cmath>
#include <vector>

// Initialize the static member
int Tower::nextId = 100;

// Constructor with parameters
Tower::Tower(int x_val, int y_val, int cost_val, int damage_val,
             int attack_rate_val, float hit_rate_val, int range_val,
             TowerType type_val, int level_val, int level_up_cost_val,
             int resale_val)
    : x(x_val), y(y_val), cost(cost_val), damage(damage_val),
      attack_rate(attack_rate_val), hit_rate(hit_rate_val), range(range_val),
      type(type_val), tid(nextId++), level(level_val),
      levelUpCost(level_up_cost_val), resale(resale_val) {}

// Default constructor
Tower::Tower() : Tower(-1, -1, 100, 3, 3, 0.65, 1, REGULAR, 1, 50, 50) {}

// Virtual destructor
Tower::~Tower() {}

// Virtual getters
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

// Virtual setters
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

// Virtual operations

// Even though levelUp() is virtual, we can define a default behavior here.
// Derived classes or decorators can override this if needed.
void Tower::levelUp() {
  // Default level-up behavior (if any) can be defined here.
  std::cout << "Tower leveling up!" << std::endl;
}

// The attack function performs the tower's attack.
// This function is virtual and can be overridden in decorators.
bool Tower::attack(std::vector<Critter *> &critters, int tick_count,
                   int *player_points, Map &gameMap) {
  if (tick_count % attack_rate == 0) {
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
        p->setHealth(p->getHealth() - getDamage());
        std::cout << "Tower " << getTid() << " attacked a critter."
                  << std::endl;
        if (p->getHealth() <= 0) {
          std::cout << "Critter eliminated!" << std::endl;
          // Update the map based on the critter's position.
          if (row == gameMap.entryRow && col == gameMap.entryCol) {
            gameMap.grid[row][col] = ENTRY;
          } else if (row == gameMap.exitRow && col == gameMap.exitCol) {
            gameMap.grid[row][col] = EXIT;
          } else {
            gameMap.grid[row][col] = PATH;
          }
          (*player_points) += p->getValue();
          delete p;
          it = critters.erase(it);
        } else {
          ++it;
        }
        return true; // Attack one critter per call.
      } else {
        ++it;
      }
    }
  }
  return false;
}
