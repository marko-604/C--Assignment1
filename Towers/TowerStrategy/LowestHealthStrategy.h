#ifndef LOWEST_HEALTH_TARGET_STRATEGY_H
#define LOWEST_HEALTH_TARGET_STRATEGY_H

#include "SelectionStrategy.h"

/**
 * @brief Strategy for selecting the critter with the lowest health.
 *
 * Implements the TargetSelectionStrategy interface to prioritize easy-to-kill
 * targets.
 */
class LowestHealthTargetStrategy : public TargetSelectionStrategy {
public:
  /**
   * @brief Selects the critter with the lowest health within the tower's range.
   *
   * @param critters Vector of available critters.
   * @param tower Pointer to the tower performing the selection.
   * @return Pointer to the selected critter, or nullptr if none are in range.
   */
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // LOWEST_HEALTH_TARGET_STRATEGY_H
