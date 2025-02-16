/*
 * Charles-Philippe La Richeliere,40113033
 * This is the towers.cpp file that implements some of the methods that were
 * declared in the towers.h file
 */
#include "towers.h"
#include "../Critter/critter.h"
#include <cmath>
#include <iostream>
#include <vector>

// This is a unique id for each tower that gets created in the game starts at
// 100 increments for each tower that gets created.
int Tower::nextId = 100;

// This will be the number of the id for the towers that we create each tower
// gets unique identifier.

// Dummy placeholder that we will use for the bomb tower attack, but the game
// should hold a list of all of the critters that are currently in the game

// This function shows the basic info about a critter.
// The atttack function for the tower, takes a pointer to a critter
// then it lowers the critters health, critter then checks if it has died and
// deletes itself if it is.
void Tower::attack(Critter *target) {
  std::cout << "Tower attacking critter " << target->getCID() << std::endl;
  target->setHealth(target->getHealth() - getDamage());
}

// This function checks if there is a critter that is in range, it then returns
// a pointer to that critter. Takes as an argument a vector of critters.
Critter *Tower::inRange(std::vector<Critter *> &critters) {
  int tx = getX();
  int ty = getY();

  for (Critter *c : critters) {
    int cx = c->getX();
    int cy = c->getY();

    int dx = tx - cx;
    int dy = ty - cy;

    int distance = std::sqrt(dx * dx + dy * dy);

    if (distance <= range) {
      return c;
    }
  }
  // If there are no critters in range we enter this part of the
  return nullptr;
}

// This function shows the towers information
void Tower::showTowerInfo() {
  std::cout << "Basic Tower located at x: " << x << ", y: " << y << "\n";
  std::cout << "Damage: " << damage << "\n";
  std::cout << "Range: " << range << "\n";
  std::cout << "Level: " << level << "\n";
  std::cout << "Fire Rate: " << fireRate << "\n";
  std::cout << "Hit Rate: " << hitRate << "\n";
  std::cout << "Cost: " << cost << "\n";
  std::cout << "Resale Value: " << resaleValue << std::endl;
  std::cout << "Tower ID: " << getId() << std::endl;
}

// When we level up a basic tower we increase the damage and the fire rate.
// The cost will not be included here we will make the cost be paid before the
// function gets called.
int Tower::levelUp() {
  if (getLevel() < 5) {
    setRange(getRange() + 1);
    setDamage(getDamage() + 1);
    setLevel(getLevel() + 1);
    std::cout << "Tower " << getId() << " has leveled up to " << getLevel()
              << std::endl;
    return getResale();
  } else {
    std::cout << "Cannot level up the tower " << getId() << std::endl;
    return 0;
  }
}

// This is the SniperTower implementation of the showTowerInfo() method
// OVERRIDE
void SniperTower::showTowerInfo() {

  std::cout << "Sniper Tower located at (" << getX() << ", " << getY() << ")"
            << std::endl;
  std::cout << "Damage: " << getDamage() << std::endl;
  std::cout << "Range: " << getRange() << std::endl;
  std::cout << "Level: " << getLevel() << std::endl;
  std::cout << "Fire Rate: " << getFireRate() << std::endl;
  std::cout << "Hit Rate: " << getHitRate() << std::endl;
  std::cout << "Cost: " << getCost() << std::endl;
  std::cout << "Resale Value: " << getResale() << std::endl;
  std::cout << "Sniper Tower ID: " << getId() << std::endl;
}

// OVERRIDE of the attack method of the basic Tower::attack method.
void SniperTower::attack(Critter *target) {
  std::cout << "Attacking critter " << target->getCID() << std::endl;
  target->setHealth(target->getHealth() - getDamage());
}

// This will increase the range and the damage of the sniper tower.
int SniperTower::levelUp() {
  if (getLevel() < 5) {
    setRange(getRange() + 1);
    setDamage(getDamage() + 1);
    setLevel(getLevel() + 1);
    std::cout << "Sniper Tower " << getId() << " has leveled up to "
              << getLevel() << std::endl;

    return getResale();

  } else {
    std::cout << "Sniper Tower " << getId() << "could not level up!"
              << std::endl;
    return 0;
  }
}

void BombTower::attack(Critter *target) {

  std::cout << "BombTower " << getId() << " attacking critter "
            << target->getCID() << std::endl;

  target->setHealth(target->getHealth() - getDamage());

  for (Critter *c : gameCritters) {
    int cx = c->getX();
    int cy = c->getY();

    int tx = getX();
    int ty = getY();

    int dx = tx - cx;
    int dy = ty - cy;

    int distance = std::sqrt(dx * dx + dy * dy);

    // We deal damage to all other critters that are in range of the AoE of the
    // bomb
    if (aoeArea >= distance && target->getCID() != c->getCID()) {
      c->setHealth(c->getHealth() - getDamage() / 2);

      std::cout << "Bomb Tower " << getId() << " has AoE attacked critter "
                << c->getCID() << std::endl;
      std::cout << "Critter c has " << c->getHealth() << " left" << std::endl;

      if (c->isDead()) {
        std::cout << "Critter " << c->getCID()
                  << " has died removing the critter from the game"
                  << std::endl;
        // We need to delete it from the vector.
        auto it = std::find(gameCritters.begin(), gameCritters.end(), c);
        if (it != gameCritters.end()) {
          // Delete the critter memory:
          delete *it;
          // Remove the pointer from the vector:
          gameCritters.erase(it);
        }
      }
    }
  }

  // Now we must check the squares adjacent and see if they contain a critter
  // for the bomb tower to attack.
}

// This is the levelUp method override for the BombTower class implementation
int BombTower::levelUp() {
  if (getLevel() < 5) {
    setAoE(getAoE() + 1);
    setDamage(getDamage() + 1);
    setLevel(getLevel() + 1);
    std::cout << "Bomb Tower " << getId() << " has leveled up to " << getLevel()
              << std::endl;
    return getResale();
  } else {
    std::cout << "Bomb Tower " << getId() << "could not level up!" << std::endl;
    return 0;
  }
}

void BombTower::showTowerInfo() {

  std::cout << "Bomb Tower located at (" << getX() << ", " << getY() << ")"
            << std::endl;
  std::cout << "Damage: " << getDamage() << std::endl;
  std::cout << "Range: " << getRange() << std::endl;
  std::cout << "Level: " << getLevel() << std::endl;
  std::cout << "Fire Rate: " << getFireRate() << std::endl;
  std::cout << "Hit Rate: " << getHitRate() << std::endl;
  std::cout << "Cost: " << getCost() << std::endl;
  std::cout << "Resale Value: " << getResale() << std::endl;
  std::cout << "Area of effect range: " << getAoE() << std::endl;
  std::cout << "Bomb Tower ID: " << getId() << std::endl;
}

void FreezingTower::showTowerInfo() {

  std::cout << "Freezing Tower located at (" << getX() << ", " << getY() << ")"
            << std::endl;
  std::cout << "Damage: " << getDamage() << std::endl;
  std::cout << "Range: " << getRange() << std::endl;
  std::cout << "Level: " << getLevel() << std::endl;
  std::cout << "Fire Rate: " << getFireRate() << std::endl;
  std::cout << "Hit Rate: " << getHitRate() << std::endl;
  std::cout << "Cost: " << getCost() << std::endl;
  std::cout << "Resale Value: " << getResale() << std::endl;
  std::cout << "Bomb Tower ID: " << getId() << std::endl;
  std::cout << "Freezing rate: " << slowRate << std::endl;
}

int FreezingTower::levelUp() {
  if (getLevel() < 5) {

    setSlowRate(getSlowRate() + 1);
    setDamage(getDamage() + 1);
    setLevel(getLevel() + 1);

    std::cout << "Freezing tower " << getId() << " has leveled up to "
              << getLevel() << std::endl;
    return getResale();
  } else {
    std::cout << "Could not level up freezing tower " << getId() << std::endl;
    return 0;
  }
}

void FreezingTower::attack(Critter *target) {
  std::cout << "Freezing tower attacking critter " << target->getCID()
            << std::endl;
  // We should also be applying a slow to the target
  target->setHealth(target->getHealth() - getDamage());

  // We apply a permanent slowing to the critter.
  target->setSpeed(target->getSpeed() * slowRate);
}
