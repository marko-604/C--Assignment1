#ifndef TOWER_DECORATOR_H
#define TOWER_DECORATOR_H

#include "Tower.h"

class TowerDecorator : public Tower {
protected:
  Tower *decoratedTower; // Pointer to the wrapped Tower
public:
  TowerDecorator(Tower *tower) : decoratedTower(tower) {}
  virtual ~TowerDecorator() { delete decoratedTower; }

  // Forward non-virtual functions without override.
  int getTid() { return decoratedTower->getTid(); }
  int getX() { return decoratedTower->getX(); }
  int getY() { return decoratedTower->getY(); }
  int getCost() { return decoratedTower->getCost(); }
  int getDamage() { return decoratedTower->getDamage(); }
  int getAttaRate() { return decoratedTower->getAttaRate(); }
  float getHitRate() { return decoratedTower->getHitRate(); }
  int getRange() { return decoratedTower->getRange(); }
  int getResale() { return decoratedTower->getResale(); }
  int getLevel() { return decoratedTower->getLevel(); }
  TowerType getType() { return decoratedTower->getType(); }
  int getLevelUpCost() { return decoratedTower->getLevelUpCost(); }

  void setLevel(int x) { decoratedTower->setLevel(x); }
  void setX(int x_val) { decoratedTower->setX(x_val); }
  void setY(int y_val) { decoratedTower->setY(y_val); }
  void setCost(int cost_val) { decoratedTower->setCost(cost_val); }
  void setResale(int resale_val) { decoratedTower->setResale(resale_val); }
  void setDamage(int dmg_val) { decoratedTower->setDamage(dmg_val); }
  void setAttackRate(int attackRate) {
    decoratedTower->setAttackRate(attackRate);
  }
  void setHitRate(float hit_rate_val) {
    decoratedTower->setHitRate(hit_rate_val);
  }
  void setRange(int range_val) { decoratedTower->setRange(range_val); }

  // Forward virtual functions; these are declared virtual in Tower.
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override {
    return decoratedTower->attack(critters, tick_count, player_points, gameMap);
  }
  virtual void levelUp() override { decoratedTower->levelUp(); }
};

#endif // TOWER_DECORATOR_H
