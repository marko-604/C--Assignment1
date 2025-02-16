
/**
 * @file towers.cpp
 * @brief Implements the methods declared in Towers.h for the Tower Defense
 * game.
 *
 * This file provides the implementation for the base Tower class and its
 * derived classes: SniperTower, BombTower, and FreezingTower. It defines how
 * towers attack, level up, and display their information.
 *
 * @author
 * @date
 */

#include "towers.h"
#include "../Critter/critter.h"
#include <cmath>
#include <iostream>
#include <vector>

/**
 * @brief Static variable that assigns a unique identifier to each tower.
 *
 * Towers are assigned unique IDs starting from 100.
 */
int Tower::nextId = 100;

/**
 * @brief Attacks a target critter by reducing its health.
 *
 * This is the base attack implementation for a Tower. It outputs a message and
 * subtracts the tower's damage from the target's health.
 *
 * @param target Pointer to the critter being attacked.
 */
void Tower::attack(Critter *target) {
  std::cout << "Tower attacking critter " << target->getCID() << std::endl;
  target->setHealth(target->getHealth() - getDamage());
}

/**
 * @brief Checks the vector of critters to find one within the tower's range.
 *
 * Iterates over the provided vector of critters and returns a pointer to the
 * first critter found whose distance from the tower is less than or equal to
 * the tower's range.
 *
 * @param critters A vector of pointers to critters.
 * @return Pointer to a critter in range; nullptr if no critter is within range.
 */
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
  // Return nullptr if no critter is found within range.
  return nullptr;
}

/**
 * @brief Displays basic information about the Tower.
 *
 * Outputs the tower's coordinates, damage, range, level, fire rate, hit rate,
 * cost, resale value, and unique ID.
 */
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

/**
 * @brief Levels up the Tower, increasing its range and damage.
 *
 * If the tower's level is less than 5, the tower's range and damage are
 * increased by 1, and the level is incremented. The function then outputs the
 * new level and returns the tower's resale value.
 *
 * @return The tower's resale value after leveling up; returns 0 if the tower is
 * at max level.
 */
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

/**
 * @brief Displays information specific to the Sniper Tower.
 *
 * This override outputs detailed stats for a Sniper Tower, including its unique
 * ID.
 */
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

/**
 * @brief Attacks a target critter with the Sniper Tower's capabilities.
 *
 * This override subtracts the sniper tower's damage from the target's health
 * and outputs a message.
 *
 * @param target Pointer to the critter to be attacked.
 */
void SniperTower::attack(Critter *target) {
  std::cout << "Attacking critter " << target->getCID() << std::endl;
  target->setHealth(target->getHealth() - getDamage());
}

/**
 * @brief Levels up the Sniper Tower, increasing its range and damage.
 *
 * If the tower is not at max level, its range and damage are increased by 1,
 * the level is incremented, and the new level is output.
 *
 * @return The new resale value after leveling up; returns 0 if the tower is at
 * max level.
 */
int SniperTower::levelUp() {
  if (getLevel() < 5) {
    setRange(getRange() + 1);
    setDamage(getDamage() + 1);
    setLevel(getLevel() + 1);
    std::cout << "Sniper Tower " << getId() << " has leveled up to "
              << getLevel() << std::endl;
    return getResale();
  } else {
    std::cout << "Sniper Tower " << getId() << " could not level up!"
              << std::endl;
    return 0;
  }
}

/**
 * @brief Attacks a target critter with the Bomb Tower's area-of-effect damage.
 *
 * The Bomb Tower first applies full damage to the target and then iterates over
 * the global gameCritters vector to apply half damage to any other critters
 * within its AoE.
 *
 * @param target Pointer to the primary critter to be attacked.
 */
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

    // Apply AoE damage (half damage) to critters other than the primary target
    if (aoeArea >= distance && target->getCID() != c->getCID()) {
      c->setHealth(c->getHealth() - getDamage() / 2);

      std::cout << "Bomb Tower " << getId() << " has AoE attacked critter "
                << c->getCID() << std::endl;
      std::cout << "Critter " << c->getCID() << " has " << c->getHealth()
                << " left" << std::endl;

      if (c->isDead()) {
        std::cout << "Critter " << c->getCID()
                  << " has died, removing the critter from the game"
                  << std::endl;
        auto it = std::find(gameCritters.begin(), gameCritters.end(), c);
        if (it != gameCritters.end()) {
          // Free the critter memory and remove it from the vector.
          delete *it;
          gameCritters.erase(it);
        }
      }
    }
  }
  // Note: Additional adjacent cell checking could be added here if needed.
}

/**
 * @brief Levels up the Bomb Tower, increasing its AoE and damage.
 *
 * If the tower is not at max level, its AoE and damage are increased by 1, the
 * level is incremented, and the new level is output.
 *
 * @return The new resale value after leveling up; returns 0 if the tower is at
 * max level.
 */
int BombTower::levelUp() {
  if (getLevel() < 5) {
    setAoE(getAoE() + 1);
    setDamage(getDamage() + 1);
    setLevel(getLevel() + 1);
    std::cout << "Bomb Tower " << getId() << " has leveled up to " << getLevel()
              << std::endl;
    return getResale();
  } else {
    std::cout << "Bomb Tower " << getId() << " could not level up!"
              << std::endl;
    return 0;
  }
}

/**
 * @brief Displays detailed information about the Bomb Tower.
 *
 * Outputs the Bomb Tower's coordinates, damage, range, level, fire rate, hit
 * rate, cost, resale value, area-of-effect range, and unique identifier.
 */
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

/**
 * @brief Displays detailed information about the Freezing Tower.
 *
 * Outputs the Freezing Tower's coordinates, damage, range, level, fire rate,
 * hit rate, cost, resale value, and slow rate, along with its unique
 * identifier.
 */
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
  std::cout << "Freezing Tower ID: " << getId() << std::endl;
  std::cout << "Freezing rate: " << slowRate << std::endl;
}

/**
 * @brief Levels up the Freezing Tower, increasing its slow rate and damage.
 *
 * If the tower is not at max level, its slow rate and damage are increased by
 * 1, the level is incremented, and the new level is output.
 *
 * @return The new resale value after leveling up; returns 0 if the tower is at
 * max level.
 */
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

/**
 * @brief Attacks a target critter with the Freezing Tower's capabilities.
 *
 * Applies damage to the target and permanently slows the critter by modifying
 * its speed.
 *
 * @param target Pointer to the critter to be attacked.
 */
void FreezingTower::attack(Critter *target) {
  std::cout << "Freezing tower attacking critter " << target->getCID()
            << std::endl;
  // Apply damage to the target.
  target->setHealth(target->getHealth() - getDamage());
  // Apply a permanent slowing effect.
  target->setSpeed(target->getSpeed() * slowRate);
}
