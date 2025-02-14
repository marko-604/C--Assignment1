/*
 * Charles-Philippe La Richeliere,40113033
 * This is the Towers.h file that provides the definition of the tower class,
 * the implementation for the class is in the towers.cpp file
 * test_driver_towers.cpp is the test driver
 */

#ifndef TOWERS_H
#define TOWERS_H
#include "../Critter/critter.h"
#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
// This is a temporary critter class that will be use as a dummy class for the
// moment when we get the actual version from teamate we will integrate theres

class Tower {
public:
  virtual ~Tower() {}

  Tower(int x_val, int y_val)
      : x(x_val), y(y_val), hitRate(0.75), fireRate(1.50), range(1), level(1),
        cost(100), resaleValue(50), damage(2.0), tid(nextId++) {
    std::cout << "Tower placed at coordinates x: " << x << " and y: " << y
              << std::endl;
  }

  Critter *inRange(std::vector<Critter *> &critters);

  Tower(int x_val, int y_val, double hr_val, double fr_val, double rng_val,
        double dmg_val, int cost_val, int resale_val)
      : x(x_val), y(y_val), hitRate(hr_val), fireRate(fr_val), range(rng_val),
        level(1), cost(cost_val), resaleValue(resale_val), damage(dmg_val),
        tid(nextId++) {
    std::cout << "Tower placed at coordinates x: " << x << " and y: " << y
              << std::endl;
  }

  // Getter functions for the tower objects.
  int getX() const { return x; }
  int getY() const { return y; }
  double getHitRate() const {
    return hitRate;
  } // gets the hit rate of the tower.
  double getFireRate() const {
    return fireRate;
  } // returns the fire rate of the tower.
  int getLevel() const { return level; } // returns the level of the tower.
  int getCost() const { return cost; }   // getter for the cost
  double getDamage() const { return damage; }
  int getResale() const { return resaleValue; }
  int getRange() const { return range; }
  int getId() const { return tid; }

  // Setter functions.
  void setHitRate(double hr) { hitRate = hr; }
  void setFireRate(double fr) { fireRate = fr; }
  void setLevel(int lvl) { level = lvl; }
  void setDamage(double dmg) { damage = dmg; }
  void setRange(int rng) { range = rng; }

  // int get_target() const; This function will acquire a critter, the tower
  // must first check that the critter is in range than it will return the
  // critter object that it will do damage to.

  // This will return the id of the critter that is being attacked.
  // Virtual function to show the tower info.
  virtual void showTowerInfo();

  // This function will be used to level up the tower, what this will do is
  // change some stats of the tower.
  virtual void levelUp();

  // Virtual function to attack critters.
  virtual void attack(Critter *target);

private:
  int x;             // The x-coordinate of the tower.
  int y;             // The y-coordinate of the tower.
  double hitRate;    // The hit rate of the tower.
  double fireRate;   // The fire rate of the tower (shots/second).
  int range;         // The firing range of the tower.
  int level;         // The level of the tower [1,5]
  int cost;          // The initial cost of the tower.
  int resaleValue;   // The resale value of the tower.
  double damage;     // Damage caused by the tower.
  int tid;           // The towers unique identifier
  static int nextId; // A shared starting id that we will use to provide each
                     // tower with a unique tower id.
}; // END TOWER class

// This SniperTower class, this class is essentially the same but has better
// damage, longer range, higher hit rate and lower fire rate.
class SniperTower : public Tower {
public:
  // The constructor for a sniper tower uses the Tower constructor.
  SniperTower(int x_val, int y_val)
      : Tower(x_val, y_val, 1.0, 1.0, 3, 5.0, 150, 75) {
    // Since x and y are now private in Tower, use the getter functions.
    std::cout << "Sniper Tower created at coordinates x: " << getX()
              << " y: " << getY() << std::endl;
  }

  void showTowerInfo() override;
  void attack(Critter *target) override;
  void levelUp() override;
}; // END SNIPERTOWER class

// This is the BombTower class this does area damage rather than damage to a
// single critter, will have slower fire rate and do more damage, and has a
// lower hit rate.
class BombTower : public Tower {
public:
  BombTower(int x_val, int y_val)
      : Tower(x_val, y_val, 0.75, 1.5, 2, 2, 175, 100), aoeArea(2) {
    std::cout << "Bomb Tower created at coordinates (" << getX() << ", "
              << getY() << ")" << std::endl;
  }

  int getAoE() const { return aoeArea; }
  void setAoE(int aoe_val) {
    aoeArea = aoe_val;
    return;
  }

  void showTowerInfo() override;
  void attack(Critter *target) override;
  void levelUp() override;

  int getAoE() { return aoeArea; }

private:
  int aoeArea; // The area of effect range, eg the number of squares next to the
               // target that will be hit by the bomb tower.
}; // END BOMBTOWER  class

// This is the freezing tower class it creates a freezing tower that will slow
// down a critter permanently. This tower costs the most.
class FreezingTower : public Tower {

public:
  FreezingTower(int x_val, int y_val)
      : Tower(x_val, y_val, 0.75, 1.5, 2, 2, 200, 125), slowRate(0.3) {
    std::cout << "Created freezing tower at coordinates (" << getX() << ", "
              << getY() << ")" << std::endl;
  }

  int getSlowRate() const { return slowRate; }
  void setSlowRate(int slow_val) { slowRate = slow_val; }
  void showTowerInfo() override;
  void attack(Critter *target) override;
  void levelUp() override;

private:
  double slowRate;
};
#endif // TOWERS_H
