#include "Tower.h"
#include "../Critters/Critter.h"
#include "../Maps/Map.h"
#include <cmath>
#include <vector>

int Tower::nextId = 100;

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
          gameMap.setToPath(p->getRow(), p->getCol());

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
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) +
                          (current_col - col) * (current_col - col));

      if (distance <= getRange()) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - getDamage());
        std::cout << "SniperTower " << getTid() << " attacked critter..."
                  << std::endl;
        if (p->getHealth() <= 0) {
          std::cout << "Critter eliminated!!" << std::endl;
          (*player_points) += p->getValue();
          // We also have to remove the critter from the vector and free the
          // memory of the dead critter!
        }
        return true;
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
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) +
                          (current_col - col) * (current_col - col));

      if (distance <= getRange()) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - getDamage());
        if (p->getHealth() <= 0) {

          std::cout << "Critter eliminated!!" << std::endl;
          (*player_points) += p->getValue();
        }

        for (Critter *c : critters) {
          int splash_row = c->getRow();
          int splash_col = c->getCol();

          int splash_distance = sqrt((splash_row - row) * (splash_row - row) +
                                     (splash_col - col) * (splash_col - col));

          if (splash_distance <= getSplash()) {
            c->setHealth(c->getHealth() - getDamage() / 2);
            std::cout << "BombTower has caused splash damage to critter ... "
                      << std::endl;
            if (c->getHealth() <= 0) {
              (*player_points) += c->getValue();
            }
          }

          return true;
        }
      }
    }
  }
  return false;
}

int FreezingTower::getSlowRate() { return slow_rate; }

void FreezingTower::setSlowRate(int x) { slow_rate = x; }

bool FreezingTower::attack(std::vector<Critter *> &critters, int tick_count,
                           int *player_points, Map &gameMap) {
  if (tick_count % getAttaRate()) {
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) +
                          (current_col - col) * (current_col - col));

      if (distance <= getRange()) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - getDamage());
        p->setSpeed(p->getSpeed() + getSlowRate());
        if (p->getHealth() <= 0) {
          (*player_points) += p->getValue();
        }

        std::cout << "FreezingTower " << getTid() << " attacked critter..."
                  << std::endl;
        return true;

      } else {
        return false;
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
