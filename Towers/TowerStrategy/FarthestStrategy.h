#ifndef FARTHEST_STRATEGY_H
#define FARTHEST_STRATEGY_H

#include "SelectionStrategy.h"

/**
 * @brief Strategy for selecting the farthest critter from the tower.
 *
 * Implements the TargetSelectionStrategy interface to choose the critter
 * that is farthest along the path (or furthest in terms of distance).
 */
class FarthestTargetStrategy : public TargetSelectionStrategy {
public:
  /**
   * @brief Selects the farthest critter from the given tower.
   *
   * @param critters Vector of available critters.
   * @param tower Pointer to the tower performing the selection.
   * @return Pointer to the selected critter, or nullptr if none are in range.
   */
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // FARTHEST_TARGET_STRATEGY_H
