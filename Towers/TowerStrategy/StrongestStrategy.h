
#ifndef STRONGEST_TARGET_STRATEGY_H
#define STRONGEST_TARGET_STRATEGY_H

#include "SelectionStrategy.h"

/**
 * @brief Strategy for selecting the strongest critter based on strength value.
 *
 * Implements the TargetSelectionStrategy interface to prioritize critters
 * with the highest strength attribute.
 */
class StrongestTargetStrategy : public TargetSelectionStrategy {
public:
  /**
   * @brief Selects the strongest critter within the tower's range.
   *
   * @param critters Vector of available critters.
   * @param tower Pointer to the tower performing the selection.
   * @return Pointer to the selected critter, or nullptr if none are in range.
   */
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // STRONGEST_TARGET_STRATEGY_H
