
#ifndef FARTHEST_STRATEGY_H
#define FARTHEST_STRATEGY_H

#include "SelectionStrategy.h"

class FarthestTargetStrategy : public TargetSelectionStrategy {
public:
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // FARTHEST_TARGET_STRATEGY_H
