#ifndef TOWERS_H
#define TOWERS_H

#include "../Observer/Subject.h"
#include "TowerStrategy/SelectionStrategy.h"
#include "TowerStrategy/Strategies.h"
#include "raylib.h"
#include <iostream>
#include <vector>

// Forward declarations:
class Map;
class Critter;

enum TowerType { REGULAR, SNIPER, BOMB, FREEZING };
enum TowerStrategy { WEAKEST, CLOSEST, STRONGEST, HEALTHIEST, LEASTHEALTHY };

// A struct for projectiles.
struct Projectile {
  float x, y;   // current position
  float vx, vy; // velocity
  float speed;  // speed of movement
  float damage; // damage dealt
  bool active;  // active flag
  Color color;
  Critter *target; // target critter
};

class Tower : public Subject {
public:
  // Constructors:
  Tower(int x_val, int y_val, int cost_val, int damage_val, int attack_rate_val,
        float hit_rate_val, int range_val, TowerType type_val, int level_val,
        int level_up_cost_val, int resale_val, TargetSelectionStrategy *strat);
  Tower(); // Default constructor

  // Virtual destructor is important for proper cleanup in derived classes.
  virtual ~Tower();

  // Virtual getters:
  virtual int getTid();
  virtual int getX();
  virtual int getY();
  virtual int getCost();
  virtual int getDamage();
  virtual int getAttaRate();
  virtual float getHitRate();
  virtual int getRange();
  virtual int getResale();
  virtual int getLevel();
  virtual TowerType getType();
  virtual int getLevelUpCost();

  // Virtual setters:
  virtual void setLevel(int x);
  virtual void setX(int x_val);
  virtual void setY(int y_val);
  virtual void setCost(int cost_val);
  virtual void setResale(int resale_val);
  virtual void setDamage(int damage_val);
  virtual void setAttackRate(int attackRate);
  virtual void setHitRate(float hit_rate_val);
  virtual void setRange(int range);

  // Virtual operations:
  virtual void levelUp();
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap);

  // Strategy management:
  void setStrategy(TargetSelectionStrategy *strat);
  TargetSelectionStrategy *getStrategy() const;

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
  int level;
  int levelUpCost;

  // The target selection strategy is stored as a pointer.
  TargetSelectionStrategy *strategy;
};

#endif // TOWERS_H
