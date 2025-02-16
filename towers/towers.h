
/**
 * @file Towers.h
 * @brief Definitions of the Tower classes used in the Tower Defense game.
 *
 * This file declares the base Tower class and several derived classes:
 * - SniperTower: A tower with high damage and long range.
 * - BombTower: A tower that inflicts area-of-effect damage.
 * - FreezingTower: A tower that slows critters.
 *
 * It also declares an external global vector, \c gameCritters, which holds
 * pointers to active critters in the game.
 *
 * @note The implementation of these classes is provided in towers.cpp.
 *
 * @author
 * @date
 */

#ifndef TOWERS_H
#define TOWERS_H

#include "../Critter/critter.h"
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

/**
 * @brief Global vector of pointers to active critters.
 *
 * This vector is used by towers (e.g., BombTower) to check for critters in
 * range.
 */
extern std::vector<Critter *> gameCritters;

/**
 * @brief Base class representing a generic tower in the game.
 *
 * The Tower class encapsulates the common attributes and behaviors of towers,
 * such as location, cost, fire rate, damage, range, and leveling mechanics.
 */
class Tower {
public:
  /**
   * @brief Virtual destructor for proper cleanup of derived classes.
   */
  virtual ~Tower() {}

  /**
   * @brief Constructs a new Tower object with default stats.
   * @param x_val The x-coordinate of the tower.
   * @param y_val The y-coordinate of the tower.
   */
  Tower(int x_val, int y_val)
      : levelUpCost(50), x(x_val), y(y_val), hitRate(0.75), fireRate(1),
        range(2), level(1), cost(100), resaleValue(50), damage(2.0),
        tid(nextId++) {
    std::cout << "Tower placed at coordinates x: " << x << " and y: " << y
              << std::endl;
  }

  /**
   * @brief Constructs a new Tower object with custom parameters.
   * @param lvl_cost_val The cost to level up the tower.
   * @param x_val The x-coordinate of the tower.
   * @param y_val The y-coordinate of the tower.
   * @param hr_val The hit rate of the tower.
   * @param fr_val The fire rate of the tower.
   * @param rng_val The firing range of the tower.
   * @param dmg_val The damage caused by the tower.
   * @param cost_val The initial cost of the tower.
   * @param resale_val The resale value of the tower.
   */
  Tower(int lvl_cost_val, int x_val, int y_val, double hr_val, double fr_val,
        double rng_val, double dmg_val, int cost_val, int resale_val)
      : levelUpCost(lvl_cost_val), x(x_val), y(y_val), hitRate(hr_val),
        fireRate(fr_val), range(rng_val), level(1), cost(cost_val),
        resaleValue(resale_val), damage(dmg_val), tid(nextId++) {
    std::cout << "Tower placed at coordinates x: " << x << " and y: " << y
              << std::endl;
  }

  /**
   * @brief Checks for a critter within the tower's range.
   * @param critters A vector of pointers to critters to search.
   * @return Pointer to the first critter found within range; otherwise,
   * nullptr.
   */
  Critter *inRange(std::vector<Critter *> &critters);

  // Getter functions.
  /**
   * @brief Gets the x-coordinate of the tower.
   * @return The x-coordinate.
   */
  int getX() const { return x; }

  /**
   * @brief Gets the y-coordinate of the tower.
   * @return The y-coordinate.
   */
  int getY() const { return y; }

  /**
   * @brief Gets the hit rate of the tower.
   * @return The hit rate.
   */
  double getHitRate() const { return hitRate; }

  /**
   * @brief Gets the fire rate (shots per second) of the tower.
   * @return The fire rate.
   */
  double getFireRate() const { return fireRate; }

  /**
   * @brief Gets the current level of the tower.
   * @return The level.
   */
  int getLevel() const { return level; }

  /**
   * @brief Gets the initial cost of the tower.
   * @return The cost.
   */
  int getCost() const { return cost; }

  /**
   * @brief Gets the damage inflicted by the tower.
   * @return The damage.
   */
  double getDamage() const { return damage; }

  /**
   * @brief Gets the resale value of the tower.
   * @return The resale value.
   */
  int getResale() const { return resaleValue; }

  /**
   * @brief Gets the firing range of the tower.
   * @return The range.
   */
  int getRange() const { return range; }

  /**
   * @brief Gets the unique identifier of the tower.
   * @return The tower's ID.
   */
  int getId() const { return tid; }

  /**
   * @brief Gets the current tick accumulator of the tower.
   * @return The accumulator value.
   */
  int getAccumulator() { return tick_accumulator; }

  /**
   * @brief Gets the cost required to level up the tower.
   * @return The level-up cost.
   */
  int getLevelUpCost() { return levelUpCost; }

  // Setter functions.
  /**
   * @brief Sets the hit rate of the tower.
   * @param hr The new hit rate.
   */
  void setHitRate(double hr) { hitRate = hr; }

  /**
   * @brief Sets the fire rate of the tower.
   * @param fr The new fire rate.
   */
  void setFireRate(double fr) { fireRate = fr; }

  /**
   * @brief Sets the current level of the tower.
   * @param lvl The new level.
   */
  void setLevel(int lvl) { level = lvl; }

  /**
   * @brief Sets the damage value for the tower.
   * @param dmg The new damage value.
   */
  void setDamage(double dmg) { damage = dmg; }

  /**
   * @brief Sets the firing range of the tower.
   * @param rng The new range.
   */
  void setRange(int rng) { range = rng; }

  /**
   * @brief Increments the tick accumulator by one.
   */
  void increment_accumulator() { tick_accumulator += 1; }

  /**
   * @brief Sets the tick accumulator to a specific value.
   * @param val The value to set.
   */
  void setAccumulator(int val) { tick_accumulator = val; }

  /**
   * @brief Displays information about the tower.
   *
   * This virtual function should be overridden by derived classes
   * to display tower-specific information.
   */
  virtual void showTowerInfo();

  /**
   * @brief Levels up the tower, enhancing its capabilities.
   *
   * This virtual function should be overridden by derived classes to adjust
   * stats appropriately. The function returns the resale value after leveling
   * up.
   *
   * @return The new resale value after leveling up, or 0 if leveling up is not
   * possible.
   */
  virtual int levelUp();

  /**
   * @brief Attacks a specified critter.
   *
   * This virtual function should be overridden by derived classes to implement
   * specific attack behaviors.
   *
   * @param target Pointer to the critter to attack.
   */
  virtual void attack(Critter *target);

private:
  int levelUpCost;      ///< Cost required to level up the tower.
  int tick_accumulator; ///< Accumulates ticks to control the firing rate.
  int x;                ///< The x-coordinate of the tower.
  int y;                ///< The y-coordinate of the tower.
  double hitRate;       ///< The hit rate of the tower.
  double fireRate;      ///< The fire rate (shots per second) of the tower.
  int range;            ///< The firing range of the tower.
  int level;            ///< The current level of the tower (e.g., 1 to 5).
  int cost;             ///< The initial cost of the tower.
  int resaleValue;      ///< The resale value of the tower.
  double damage;        ///< The damage inflicted by the tower.
  int tid;              ///< Unique identifier for the tower.
  static int nextId; ///< Static variable used to assign unique IDs to towers.
}; // END TOWER class

/**
 * @brief Derived class representing a Sniper Tower.
 *
 * The SniperTower offers better damage, longer range, and a higher hit rate
 * compared to a regular tower, at the cost of a lower fire rate.
 */
class SniperTower : public Tower {
public:
  /**
   * @brief Constructs a new Sniper Tower.
   * @param x_val The x-coordinate where the tower is placed.
   * @param y_val The y-coordinate where the tower is placed.
   */
  SniperTower(int x_val, int y_val)
      : Tower(75, x_val, y_val, 1.0, 1.0, 5, 5.0, 150, 75) {
    std::cout << "Sniper Tower created at coordinates x: " << getX()
              << " y: " << getY() << std::endl;
  }

  /**
   * @brief Displays the sniper tower's information.
   */
  void showTowerInfo() override;

  /**
   * @brief Attacks a target critter.
   * @param target Pointer to the critter to attack.
   */
  void attack(Critter *target) override;

  /**
   * @brief Levels up the sniper tower, increasing its range and damage.
   * @return The new resale value after leveling up.
   */
  int levelUp() override;
}; // END SNIPERTOWER class

/**
 * @brief Derived class representing a Bomb Tower.
 *
 * The BombTower inflicts area-of-effect (AoE) damage, affecting multiple
 * critters near the primary target.
 */
class BombTower : public Tower {
public:
  /**
   * @brief Constructs a new Bomb Tower.
   * @param x_val The x-coordinate where the tower is placed.
   * @param y_val The y-coordinate where the tower is placed.
   */
  BombTower(int x_val, int y_val)
      : Tower(100, x_val, y_val, 0.75, 3, 2, 2, 175, 100), aoeArea(2) {
    std::cout << "Bomb Tower created at coordinates (" << getX() << ", "
              << getY() << ")" << std::endl;
  }

  /**
   * @brief Gets the area-of-effect (AoE) range of the Bomb Tower.
   * @return The AoE range.
   */
  int getAoE() const { return aoeArea; }

  /**
   * @brief Sets the area-of-effect (AoE) range of the Bomb Tower.
   * @param aoe_val The new AoE range.
   */
  void setAoE(int aoe_val) {
    aoeArea = aoe_val;
    return;
  }

  /**
   * @brief Displays the bomb tower's information.
   */
  void showTowerInfo() override;

  /**
   * @brief Attacks a target critter with area-of-effect damage.
   *
   * The attack function damages the primary target and any other critters
   * within the AoE.
   *
   * @param target Pointer to the primary critter to attack.
   */
  void attack(Critter *target) override;

  /**
   * @brief Levels up the bomb tower, increasing its AoE and damage.
   * @return The new resale value after leveling up.
   */
  int levelUp() override;

  /**
   * @brief Gets the area-of-effect range (duplicate getter).
   * @return The AoE range.
   */
  int getAoE() { return aoeArea; }

private:
  int aoeArea; ///< The area-of-effect range (number of adjacent squares
               ///< affected).
}; // END BOMBTOWER class

/**
 * @brief Derived class representing a Freezing Tower.
 *
 * The FreezingTower slows down critters permanently while attacking them,
 * at the cost of a higher purchase price.
 */
class FreezingTower : public Tower {
public:
  /**
   * @brief Constructs a new Freezing Tower.
   * @param x_val The x-coordinate where the tower is placed.
   * @param y_val The y-coordinate where the tower is placed.
   */
  FreezingTower(int x_val, int y_val)
      : Tower(100, x_val, y_val, 0.75, 2, 2, 2, 200, 125), slowRate(0.3) {
    std::cout << "Created freezing tower at coordinates (" << getX() << ", "
              << getY() << ")" << std::endl;
  }

  /**
   * @brief Gets the slow rate applied by the freezing tower.
   * @return The slow rate.
   */
  int getSlowRate() const { return slowRate; }

  /**
   * @brief Sets the slow rate applied by the freezing tower.
   * @param slow_val The new slow rate.
   */
  void setSlowRate(int slow_val) { slowRate = slow_val; }

  /**
   * @brief Displays the freezing tower's information.
   */
  void showTowerInfo() override;

  /**
   * @brief Attacks a target critter, applying damage and a permanent slowing
   * effect.
   * @param target Pointer to the critter to attack.
   */
  void attack(Critter *target) override;

  /**
   * @brief Levels up the freezing tower, increasing its slow rate and damage.
   * @return The new resale value after leveling up.
   */
  int levelUp() override;

private:
  double slowRate; ///< The rate at which critters are slowed by the tower.
};

#endif // TOWERS_H
