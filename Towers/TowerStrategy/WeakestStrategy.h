
#ifndef WEAKEST_TARGET_STRATEGY_H
#define WEAKEST_TARGET_STRATEGY_H

#include "SelectionStrategy.h"

class WeakestTargetStrategy : public TargetSelectionStrategy {
public:
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // WEAKEST_TARGET_STRATEGY_H
