#include "Tower.h"

#include "../Critters/Critter.h"
#include <cmath>
#include <vector>

int Tower::getX() { return x; }

int Tower::getY() { return y; }

int Tower::getCost() { return cost; }

int Tower::getAttaRate() { return attack_rate; }

float Tower::getHitRate() { return hit_rate; }

int Tower::getResale() { return resale; }

int Tower::getRange() { return range; }

int Tower::getDamage() { return damage; }

TowerType Tower::getType() { return type; }

void Tower::setX(int x_val) { x = x_val; }

void Tower::setY(int y_val) { y = y_val; }

void Tower::setAttackRate(int attack_rate_val) {
  attack_rate = attack_rate_val;
}

void Tower::setCost(int cost_val) { cost = cost_val; }

void Tower::setDamage(int dmg_val) { damage = dmg_val; }

void Tower::setRange(int range_val) { range = range_val; }

void Tower::setResale(int resale_val) { resale = resale_val; }

// This function will be used to select and attack a critter that is in range.
void Tower::attack(std::vector<Critter *> critters, int tick_count) {
  if (tick_count % attack_rate == 0) {
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) -
                          (current_col - col) * (current_col - col));

      if (distance <= range) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - damage);
        return;
      } else {
        return;
      }
    }
  }
  // We first need to check if there is a critter in range of the attack.
}

void SniperTower::attack(std::vector<Critter *> critters, int tick_count) {
  if (tick_count % getAttaRate() == 0) {
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) -
                          (current_col - col) * (current_col - col));

      if (distance <= getRange()) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - getDamage());
        return;
      } else {
        return;
      }
    }
  }
}

int BombTower::getSplash() { return splash_area; }

void BombTower::setSplash(int x) { splash_area = x; }

void BombTower::attack(std::vector<Critter *> critters, int tick_count) {
  if (tick_count % getAttaRate() == 0) {
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) -
                          (current_col - col) * (current_col - col));

      if (distance <= getRange()) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - getDamage());

        for (Critter *c : critters) {
          int splash_row = c->getRow();
          int splash_col = c->getCol();

          int splash_distance = sqrt((splash_row - row) * (splash_row - row) -
                                     (splash_col - col) * (splash_col - col));

          if (splash_distance <= getSplash()) {
            p->setHealth(p->getHealth() - getDamage() / 2);
          }
          return;
        }
      } else {
        return;
      }
    }
  }
}

int FreezingTower::getSlowRate() { return slow_rate; }

void FreezingTower::setSlowRate(int x) { slow_rate = x; }

void FreezingTower::attack(std::vector<Critter *> critters, int tick_count) {

  if (tick_count % getAttaRate()) {
    for (Critter *p : critters) {
      int row = p->getRow();
      int col = p->getCol();

      int curret_row = getX();
      int current_col = getY();

      int distance = sqrt((curret_row - row) * (curret_row - row) -
                          (current_col - col) * (current_col - col));

      if (distance <= getRange()) {
        // we can attack the critter and lower it's health by the damage.
        p->setHealth(p->getHealth() - getDamage());
        p->setSpeed(p->getSpeed() + getSlowRate());
        return;
      } else {
        return;
      }
    }
  }
}

void Tower::levelUp() {
  range += 1;
  damage += 1;
}

void BombTower::levelUp() {
  setSplash(getSplash() + 1);
  setDamage(getDamage() + 1);
}

void SniperTower::levelUp() {
  setRange(getRange() + 1);
  setDamage(getDamage() + 1);
}

void FreezingTower::levelUp() {
  setDamage(getDamage() + 1);
  setSlowRate(getSlowRate() + 1);
}
