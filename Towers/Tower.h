#ifndef TOWERS_H
#define TOWERS_H

#include "raylib.h"
#include <iostream>
#include <vector>

class Critter;
enum TowerType { REGULAR, SNIPER, BOMB, FREEZING };

struct Projectile {
  float x, y;   // current position in tile or pixel coordinates
  float vx, vy; // velocity (pixels/sec or tiles/sec)
  float speed;  // how fast the projectile moves
  float damage; // how much damage it deals
  bool active;  // whether it's still flying

  // Optionally store a color, or towerType so you can draw different colors
  Color color;

  // If you want to track a target or keep the direction locked
  Critter *target; // or std::weak_ptr<Critter> if using shared_ptr
};

// The Tower class
class Tower {

public:
  Tower(int x_val, int y_val, int cost_val, int damage_val, int attack_rate_val,
        float hit_rate_val, int range_val, TowerType type_val)
      : x(x_val), y(y_val), cost(cost_val), damage(damage_val),
        attack_rate(attack_rate_val), hit_rate(hit_rate_val), range(range_val),
        type(type_val), tid(nextId++) {} // Incorrect order

  // Constructor with no arguments.
  Tower() : Tower(-1, -1, 100, 3, 3, 0.65, 1, REGULAR) {}

  int getTid();
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
  virtual bool attack(std::vector<Critter *> critters,
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
  int tid;
  static int nextId;
};

// Same as regular tower but more damage and greater range.
class SniperTower : public Tower {
public:
  SniperTower() : Tower(-1, -1, 150, 5, 3, 0.75, 3, SNIPER) {}
  void levelUp() override;
  bool attack(std::vector<Critter *> critters, int tick_count) override;
};

// This is the bomb tower class
class BombTower : public Tower {
public:
  BombTower(int splash_area_val)
      : Tower(-1, -1, 200, 4, 3, 1, 1, BOMB), splash_area(splash_area_val) {}

  int getSplash();
  void setSplash(int x);
  void levelUp() override;
  bool attack(std::vector<Critter *> critters, int tick_count) override;

private:
  int splash_area;
};

// This is the freezing tower
class FreezingTower : public Tower {
public:
  FreezingTower(int slow_rate_val)
      : Tower(-1, -1, 200, 5, 3, 0.75, 1, FREEZING) {}

  bool attack(std::vector<Critter *> critters, int tick_count) override;
  int getSlowRate();
  void setSlowRate(int x);
  void levelUp() override;

private:
  int slow_rate;
};

#endif // !TOWERS_H
