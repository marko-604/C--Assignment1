#ifndef BOMB_DECORATOR_H
#define BOMB_DECORATOR_H

#include "TowerDecorator.h"
#include <vector>
class SelectionStrategy;
class BombDecorator : public TowerDecorator {
private:
  int splashRadius;         // The radius (in grid units) for splash damage.
  float splashDamageFactor; // The fraction of base damage applied as splash
                            // damage.
public:
  BombDecorator(Tower *tower, int splashRadiusVal, float splashDamageFactorVal);

  // Override attack to add the bomb (splash damage) effect.
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // BOMB_DECORATOR_H
