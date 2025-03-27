
#ifndef LEVELUP_DECORATOR_H
#define LEVELUP_DECORATOR_H

#include "TowerDecorator.h"
#include <vector>

// Forward declare classes you need:
class Critter;
class Map;

/**
 * A decorator that levels up the tower once (or multiple times) right after
 * creation. Afterwards, it behaves like the wrapped tower.
 */
class LevelUpDecorator : public TowerDecorator {
public:
  /**
   * @param baseTower  The tower to decorate.
   * @param levelsToGain  How many times to call levelUp() on the wrapped tower.
   */
  LevelUpDecorator(Tower *baseTower, int levelsToGain = 1);

  /**
   * Attack override:
   * - On the first call, level up the base tower 'levelsToGain' times
   * (one-time).
   * - Then delegate the rest of the logic to the base tower as usual.
   */
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;

private:
  int levelsToGain;
  bool hasLeveledUp; // ensures we only level up once
};

#endif // LEVELUP_DECORATOR_H
