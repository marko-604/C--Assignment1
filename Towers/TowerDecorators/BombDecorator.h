
#ifndef BOMB_DECORATOR_H
#define BOMB_DECORATOR_H

#include "TowerDecorator.h"
#include <vector>

class SelectionStrategy;

/**
 * @brief A decorator that adds splash damage to a tower.
 *
 * This decorator allows the tower to deal area-of-effect (AoE) damage
 * to critters within a certain radius around the main target.
 */
class BombDecorator : public TowerDecorator {
private:
  int splashRadius;         ///< The radius (in grid units) for splash damage.
  float splashDamageFactor; ///< The fraction of base damage applied as splash
                            ///< damage.

public:
  /**
   * @brief Constructs a BombDecorator with specified splash parameters.
   *
   * @param tower The base tower to decorate.
   * @param splashRadiusVal Radius of the splash damage.
   * @param splashDamageFactorVal Fraction of base damage applied to nearby
   * critters.
   */
  BombDecorator(Tower *tower, int splashRadiusVal, float splashDamageFactorVal);

  /**
   * @brief Overrides the attack method to implement splash damage.
   *
   * Applies full damage to a main target and reduced splash damage to all
   * surrounding critters within the specified radius.
   *
   * @param critters List of critters to consider as targets.
   * @param tick_count Current game tick.
   * @param player_points Pointer to the player's score.
   * @param gameMap Reference to the game map.
   * @return true If an attack was performed.
   * @return false If no valid target was found or cooldown not met.
   */
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // BOMB_DECORATOR_H
