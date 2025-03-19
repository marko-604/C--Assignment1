#include "Tower.h"
#include "../Critters/Critter.h"
#include "../Maps/Map.h"
#include <cmath>
#include <vector>

int Tower::nextId = 100;

int Tower::getLevelUpCost() { return levelUpCost; }

int Tower::getX() { return x; }

int Tower::getTid() { return tid; }

int Tower::getY() { return y; }

int Tower::getLevel() { return level; };

int Tower::getCost() { return cost; }

int Tower::getAttaRate() { return attack_rate; }

float Tower::getHitRate() { return hit_rate; }

int Tower::getResale() { return resale; }

int Tower::getRange() { return range; }

int Tower::getDamage() { return damage; }

TowerType Tower::getType() { return type; }

void Tower::setLevel(int x) {
  level = x;
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

void Tower::setAttackRate(int attack_rate_val) {
  attack_rate = attack_rate_val;
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

void Tower::setRange(int range_val) {
  range = range_val;
  Notify();
}

void Tower::setResale(int resale_val) {
  resale = resale_val;
  Notify();
}

bool Tower::attack(std::vector<Critter *> &critters, int tick_count,
                   int *player_points, Map &gameMap) {
  if (tick_count % attack_rate == 0) {
    // Use an iterator-based loop to allow removal of elements.
    for (auto it = critters.begin(); it != critters.end();) {
      Critter *p = *it;
      int row = p->getRow();
      int col = p->getCol();
      int current_row = getX();
      int current_col = getY();

      int distance =
          static_cast<int>(sqrt((current_row - row) * (current_row - row) +
                                (current_col - col) * (current_col - col)));

      if (distance <= range) {
        // Attack the critter
        p->setHealth(p->getHealth() - damage);
        std::cout << "Tower " << getTid() << " attacked critter..."
                  << std::endl;

        // If critter is dead, update points, delete it, and remove it from the
        // vector.
        if (p->getHealth() <= 0) {
          std::cout << "Critter eliminated!!" << std::endl;

          // We always make sure we make the right reset.
          if (row == gameMap.entryRow && col == gameMap.entryCol) {
            gameMap.grid[row][col] = ENTRY;
          } else if (row == gameMap.exitRow && col == gameMap.exitCol) {
            gameMap.grid[row][col] = EXIT;
          } else {
            gameMap.grid[row][col] = PATH;
          }

          (*player_points) += p->getValue();
          delete p; // free memory
                    //
          it = critters.erase(
              it); // remove pointer from vector and update iterator
        } else {
          ++it;
        }
        return true; // Attack only one critter per call
      } else {
        ++it;
      }
    }
  }
  return false;
}

// The level up function of the tower class.
void Tower::levelUp() {
  level += 1;
  if (attack_rate > 1) {
    attack_rate -= 1;
  }
  damage += 1;
}
