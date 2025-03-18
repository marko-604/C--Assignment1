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

bool SniperTower::attack(std::vector<Critter *> &critters, int tick_count,
                         int *player_points, Map &gameMap) {
  if (tick_count % getAttaRate() == 0) {
    for (auto it = critters.begin(); it != critters.end();) {
      Critter *p = *it;
      int row = p->getRow();
      int col = p->getCol();
      int current_row = getX();
      int current_col = getY();

      int distance =
          static_cast<int>(sqrt((current_row - row) * (current_row - row) +
                                (current_col - col) * (current_col - col)));

      if (distance <= getRange()) {
        // Attack the critter.
        p->setHealth(p->getHealth() - getDamage());
        std::cout << "SniperTower " << getTid() << " attacked critter..."
                  << std::endl;

        if (p->getHealth() <= 0) {
          std::cout << "Critter eliminated!!" << std::endl;
          // Reset the tile if needed.
          if (row == gameMap.entryRow && col == gameMap.entryCol) {
            gameMap.grid[row][col] = ENTRY;
          } else if (row == gameMap.exitRow && col == gameMap.exitCol) {
            gameMap.grid[row][col] = EXIT;
          } else {
            gameMap.grid[row][col] = PATH;
          }
          (*player_points) += p->getValue();
          delete p;                // free memory
          it = critters.erase(it); // remove pointer from vector
        } else {
          ++it;
        }
        return true; // Attack only one critter per call.
      } else {
        ++it;
      }
    }
  }
  return false;
}

int BombTower::getSplash() { return splash_area; }

void BombTower::setSplash(int x) { splash_area = x; }

bool BombTower::attack(std::vector<Critter *> &critters, int tick_count,
                       int *player_points, Map &gameMap) {
  if (tick_count % getAttaRate() == 0) {
    // Primary target attack: iterate with an iterator.
    for (auto it = critters.begin(); it != critters.end();) {
      Critter *p = *it;
      int row = p->getRow();
      int col = p->getCol();
      int current_row = getX();
      int current_col = getY();
      int distance =
          static_cast<int>(sqrt((current_row - row) * (current_row - row) +
                                (current_col - col) * (current_col - col)));
      if (distance <= getRange()) {
        // Attack the primary target.
        p->setHealth(p->getHealth() - getDamage());
        std::cout << "BombTower " << getTid() << " attacked critter..."
                  << std::endl;
        if (p->getHealth() <= 0) {
          std::cout << "Critter eliminated!!" << std::endl;
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

        // Apply splash damage to other critters.
        // Use a separate loop so as not to interfere with our primary target
        // loop.
        for (auto splash_it = critters.begin(); splash_it != critters.end();) {
          Critter *c = *splash_it;
          int splash_row = c->getRow();
          int splash_col = c->getCol();
          int splash_distance =
              static_cast<int>(sqrt((splash_row - row) * (splash_row - row) +
                                    (splash_col - col) * (splash_col - col)));
          if (splash_distance <= getSplash()) {
            c->setHealth(c->getHealth() - (getDamage() / 2));
            std::cout << "BombTower caused splash damage to critter..."
                      << std::endl;
            if (c->getHealth() <= 0) {
              std::cout << "Critter eliminated by splash damage!!" << std::endl;
              if (splash_row == gameMap.entryRow &&
                  splash_col == gameMap.entryCol) {
                gameMap.grid[splash_row][splash_col] = ENTRY;
              } else if (splash_row == gameMap.exitRow &&
                         splash_col == gameMap.exitCol) {
                gameMap.grid[splash_row][splash_col] = EXIT;
              } else {
                gameMap.grid[splash_row][splash_col] = PATH;
              }
              (*player_points) += c->getValue();
              delete c;
              splash_it = critters.erase(splash_it);
            } else {
              ++splash_it;
            }
          } else {
            ++splash_it;
          }
        }
        return true;
      } else {
        ++it;
      }
    }
  }
  return false;
}

int FreezingTower::getSlowRate() { return slow_rate; }

void FreezingTower::setSlowRate(int x) { slow_rate = x; }

bool FreezingTower::attack(std::vector<Critter *> &critters, int tick_count,
                           int *player_points, Map &gameMap) {
  if (tick_count % getAttaRate() == 0) {
    for (auto it = critters.begin(); it != critters.end();) {
      Critter *p = *it;
      int row = p->getRow();
      int col = p->getCol();
      int current_row = getX();
      int current_col = getY();

      int distance =
          static_cast<int>(sqrt((current_row - row) * (current_row - row) +
                                (current_col - col) * (current_col - col)));

      if (distance <= getRange()) {
        // Attack the critter.
        p->setHealth(p->getHealth() - getDamage());
        // Apply slowing effect.
        p->setSpeed(p->getSpeed() + getSlowRate());
        std::cout << "FreezingTower " << getTid() << " attacked critter..."
                  << std::endl;
        if (p->getHealth() <= 0) {
          std::cout << "Critter eliminated!!" << std::endl;
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
        return true;
      } else {
        ++it;
      }
    }
  }
  return false;
}

void Tower::levelUp() {
  level += 1;
  range += 1;
  damage += 1;
  Notify();
}

void BombTower::levelUp() {
  setLevel(getLevel() + 1);
  setSplash(getSplash() + 1);
  setDamage(getDamage() + 1);
  Notify();
}

void SniperTower::levelUp() {
  setLevel(getLevel() + 1);
  setRange(getRange() + 1);
  setDamage(getDamage() + 1);
  Notify();
}

void FreezingTower::levelUp() {
  setLevel(getLevel() + 1);
  setDamage(getDamage() + 1);
  setSlowRate(getSlowRate() + 1);
  Notify();
}
