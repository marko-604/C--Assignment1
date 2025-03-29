#ifndef FREEZING_DECORATOR_H
#define FREEZING_DECORATOR_H

#include "TowerDecorator.h"
#include <vector>

/**
 * @brief A decorator that adds a freezing effect to a tower.
 *
 * Instead of dealing extra damage, this decorator slows down critters
 * by reducing their movement speed.
 */
class FreezingDecorator : public TowerDecorator {
private:
  float slowRate; ///< The amount by which to slow the critter's speed.

public:
  /**
   * @brief Constructs a FreezingDecorator with a specified slow rate.
   *
   * @param tower The base tower to decorate.
   * @param slowRateVal The amount to reduce a critter's speed when hit.
   */
  FreezingDecorator(Tower *tower, float slowRateVal);

  /**
   * @brief Overrides the attack method to apply a slowing effect instead of
   * base attack.
   *
   * This implementation does not call the base tower's attack method, and
   * instead applies freezing logic directly.
   *
   * @param critters List of critters to consider as targets.
   * @param tick_count Current game tick.
   * @param player_points Pointer to the player's score (not affected by
   * freezing).
   * @param gameMap Reference to the game map.
   * @return true If a critter was successfully frozen.
   * @return false If no target was affected.
   */
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // FREEZING_DECORATOR_H
