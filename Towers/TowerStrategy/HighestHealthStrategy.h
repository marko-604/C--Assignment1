
#ifndef HIGHEST_HEALTH_TARGET_STRATEGY_H
#define HIGHEST_HEALTH_TARGET_STRATEGY_H

#include "../../Critters/Critter.h"
#include "SelectionStrategy.h"

/**
 * @brief Strategy for selecting the critter with the highest health.
 *
 * Implements the TargetSelectionStrategy interface to prioritize tankier
 * enemies.
 */
class HighestHealthTargetStrategy : public TargetSelectionStrategy {
public:
  /**
   * @brief Selects the critter with the highest health within the tower's
   * range.
   *
   * @param critters Vector of available critters.
   * @param tower Pointer to the tower performing the selection.
   * @return Pointer to the selected critter, or nullptr if none are in range.
   */
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // HIGHEST_HEALTH_TARGET_STRATEGY_H
