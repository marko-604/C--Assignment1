#ifndef TOWER_DECORATOR_H
#define TOWER_DECORATOR_H

#include "../Tower.h"
#include <vector>

class TowerDecorator : public Tower {
protected:
  Tower *baseTower; // The wrapped Tower instance.
public:
  // Constructor takes ownership of the passed Tower pointer.
  TowerDecorator(Tower *tower);
  virtual ~TowerDecorator();

  // Forwarding virtual getters.
  virtual int getTid() override;
  virtual int getX() override;
  virtual int getY() override;
  virtual int getCost() override;
  virtual int getDamage() override;
  virtual int getAttaRate() override;
  virtual float getHitRate() override;
  virtual int getRange() override;
  virtual int getResale() override;
  virtual int getLevel() override;
  virtual TowerType getType() override;
  virtual int getLevelUpCost() override;

  // Forwarding virtual setters.
  virtual void setLevel(int lvl) override;
  virtual void setX(int x_val) override;
  virtual void setY(int y_val) override;
  virtual void setCost(int cost_val) override;
  virtual void setDamage(int dmg_val) override;
  virtual void setAttackRate(int attackRate) override;
  virtual void setHitRate(float hit_rate_val) override;
  virtual void setRange(int range_val) override;
  virtual void setResale(int resale_val) override;

  // Forwarding operations.
  virtual void levelUp() override;
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // TOWER_DECORATOR_H
