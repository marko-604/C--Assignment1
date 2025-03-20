// TargetSelectionStrategy.h
#ifndef SELECTION_STRATEGY_H
#define SELECTION_STRATEGY_H

#include <vector>

class Tower;
class Critter;
class TargetSelectionStrategy {
public:
  // Given the available critters and the tower, select one target (or nullptr
  // if none)
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) = 0;
  virtual ~TargetSelectionStrategy() {}
};

#endif // TARGET_SELECTION_STRATEGY_H
