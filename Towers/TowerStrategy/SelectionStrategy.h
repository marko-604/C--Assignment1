
// TargetSelectionStrategy.h
#ifndef SELECTION_STRATEGY_H
#define SELECTION_STRATEGY_H

#include <vector>

class Tower;
class Critter;

/**
 * @brief Abstract base class for tower target selection strategies.
 *
 * Provides an interface for selecting a single target from a list of critters
 * based on a specific strategy (e.g., weakest, closest, farthest).
 */
class TargetSelectionStrategy {
public:
  /**
   * @brief Select a target from the list of critters based on the strategy.
   *
   * @param critters Vector of available critters.
   * @param tower Pointer to the tower performing the selection.
   * @return Pointer to the selected critter, or nullptr if no valid target.
   */
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) = 0;

  /// @brief Virtual destructor for proper cleanup of derived classes.
  virtual ~TargetSelectionStrategy() {}
};

#endif // TARGET_SELECTION_STRATEGY_H
