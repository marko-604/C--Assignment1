#include "Tower.h"

#include "../Critters/Critter.h"
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

// This function will be used to select and attack a critter that is in range.
bool Tower::attack(std::vector<Critter *> &critters, int tick_count) {
  if (tick_count % attack_rate == 0) {
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) +
                          (current_col - col) * (current_col - col));

      if (distance <= range) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - damage);
        std::cout << "Tower " << getTid() << " attacked critter..."
                  << std::endl;

        if (p->getHealth() <= 0) {
          std::cout << "Critter eliminated!!" << std::endl;
          std::cout << "HERE\n";
        }
        return true;
      }
    }
  }
  return false;
  // We first need to check if there is a critter in range of the attack.
}

bool SniperTower::attack(std::vector<Critter *> &critters, int tick_count) {
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
        return true;
      }
    }
  }
  return false;
}

int BombTower::getSplash() { return splash_area; }

void BombTower::setSplash(int x) { splash_area = x; }

bool BombTower::attack(std::vector<Critter *> &critters, int tick_count) {
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

        for (Critter *c : critters) {
          int splash_row = c->getRow();
          int splash_col = c->getCol();

          int splash_distance = sqrt((splash_row - row) * (splash_row - row) +
                                     (splash_col - col) * (splash_col - col));

          if (splash_distance <= getSplash()) {
            p->setHealth(p->getHealth() - getDamage() / 2);
            std::cout << "BombTower has caused splash damage to critter ... "
                      << std::endl;
          }

          std::cout << "BombTower " << getTid() << " attacked critter..."
                    << std::endl;
          return true;
        }
      }
    }
  }
  return false;
}

int FreezingTower::getSlowRate() { return slow_rate; }

void FreezingTower::setSlowRate(int x) { slow_rate = x; }

bool FreezingTower::attack(std::vector<Critter *> &critters, int tick_count) {
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
