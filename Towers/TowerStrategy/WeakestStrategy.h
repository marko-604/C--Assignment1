
#ifndef WEAKEST_TARGET_STRATEGY_H
#define WEAKEST_TARGET_STRATEGY_H

#include "SelectionStrategy.h"

/**
 * @brief Strategy for selecting the weakest critter based on strength value.
 *
 * Implements the TargetSelectionStrategy interface to prioritize critters
 * with the lowest strength attribute.
 */
class WeakestTargetStrategy : public TargetSelectionStrategy {
public:
  /**
   * @brief Selects the weakest critter within the tower's range.
   *
   * @param critters Vector of available critters.
   * @param tower Pointer to the tower performing the selection.
   * @return Pointer to the selected critter, or nullptr if none are in range.
   */
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // WEAKEST_TARGET_STRATEGY_H
