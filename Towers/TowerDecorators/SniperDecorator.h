#ifndef SNIPER_DECORATOR_H
#define SNIPER_DECORATOR_H

#include "TowerDecorator.h"
#include <vector>

/**
 * @brief A decorator that enhances a tower with sniper abilities.
 *
 * Increases the tower's range and damage, and overrides the attack behavior
 * to implement sniper-like precision.
 */
class SniperDecorator : public TowerDecorator {
private:
  int extraRange;  ///< Extra range provided by the sniper effect.
  int extraDamage; ///< Additional damage bonus.

public:
  /**
   * @brief Constructs a SniperDecorator that adds bonus range and damage.
   *
   * @param tower The base tower to decorate.
   * @param extraRangeVal Additional range to apply.
   * @param extraDamageVal Additional damage to apply.
   */
  SniperDecorator(Tower *tower, int extraRangeVal, int extraDamageVal);

  /**
   * @brief Overrides the attack method to implement sniper-specific logic.
   *
   * Targets enemies using sniper rules and applies increased damage and range.
   *
   * @param critters List of critters to consider as targets.
   * @param tick_count Current tick in the game loop.
   * @param player_points Pointer to the player's score, updated on kill.
   * @param gameMap Reference to the game map.
   * @return true If an attack was made.
   * @return false If no valid target was found or cooldown not met.
   */
  virtual bool attack(std::vector<Critter *> &critters, int tick_count,
                      int *player_points, Map &gameMap) override;
};

#endif // SNIPER_DECORATOR_H
