#ifndef FREEZING_DECORATOR_H
#define FREEZING_DECORATOR_H

#include "TowerDecorator.h"
#include <vector>

class FreezingDecorator : public TowerDecorator {
private:
  float slowRate; // The amount by which to increment the critter's speed.
public:
  FreezingDecorator(Tower *tower, float slowRateVal);

  // We override attack fully, without calling baseTower->attack(...)
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // FREEZING_DECORATOR_H
