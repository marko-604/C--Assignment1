#ifndef TOWERS_H
#define TOWERS_H

#include "raylib.h"
#include <iostream>
#include <vector>

class Critter;
enum TowerType { REGULAR, SNIPER, BOMB, FREEZING };

// The Tower class
class Tower {

public:
  Tower(int x_val, int y_val, int cost_val, int damage_val, int attack_rate_val,
        float hit_rate_val, int range_val, TowerType type_val)
      : x(x_val), y(y_val), cost(cost_val), damage(damage_val),
        attack_rate(attack_rate_val), hit_rate(hit_rate_val), range(range_val),
        type(type_val) {} // Incorrect order

  // Constructor with no arguments.
  Tower() : Tower(-1, -1, 100, 3, 3, 0.65, 1, REGULAR) {}

  int getX();
  int getY();
  int getCost();
  int getDamage();
  int getAttaRate();
  float getHitRate();
  int getRange();
  int getResale();
  TowerType getType();

  void setX(int x_val);
  void setY(int y_val);
  void setCost(int cost_val);
  void setResale(int resale_val);
  void setDamage(int damage_val);
  void setAttackRate(int attackRate);
  void setHitRate(float hit_ratte_val);
  void setRange(int range);

  virtual void levelUp();
  virtual void attack(std::vector<Critter *> critters,
                      int tick_count); // The tower attacks.

private:
  TowerType type;
  int x;
  int y;
  int cost;
  int resale;
  int damage;
  int attack_rate;
  float hit_rate;
  int range;
};

// Same as regular tower but more damage and greater range.
class SniperTower : public Tower {
public:
  SniperTower() : Tower(-1, -1, 150, 5, 3, 0.75, 3, SNIPER) {}
  void levelUp() override;
  void attack(std::vector<Critter *> critters, int tick_count) override;
};

// This is the bomb tower class
class BombTower : public Tower {
public:
  BombTower(int splash_area_val)
      : Tower(-1, -1, 200, 4, 3, 1, 1, BOMB), splash_area(splash_area_val) {}

  int getSplash();
  void setSplash(int x);
  void levelUp() override;
  void attack(std::vector<Critter *> critters, int tick_count) override;

private:
  int splash_area;
};

// This is the freezing tower
class FreezingTower : public Tower {
public:
  FreezingTower(int slow_rate_val)
      : Tower(-1, -1, 200, 5, 3, 0.75, 1, FREEZING) {}

  void attack(std::vector<Critter *> critters, int tick_count) override;
  int getSlowRate();
  void setSlowRate(int x);
  void levelUp() override;

private:
  int slow_rate;
};

#endif // !TOWERS_H
