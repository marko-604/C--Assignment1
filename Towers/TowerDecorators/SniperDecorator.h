
#ifndef SNIPER_DECORATOR_H
#define SNIPER_DECORATOR_H

#include "TowerDecorator.h"
#include <vector>

class SniperDecorator : public TowerDecorator {
private:
  int extraRange;  // Extra range provided by the sniper effect.
  int extraDamage; // Additional damage bonus.
public:
  // Constructor: wraps a tower and adds extraRange and extraDamage.
  SniperDecorator(Tower *tower, int extraRangeVal, int extraDamageVal);

  // Override attack to implement sniper targeting.
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // SNIPER_DECORATOR_H
       //
