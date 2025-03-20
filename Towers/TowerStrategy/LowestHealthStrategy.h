
#ifndef LOWEST_HEALTH_TARGET_STRATEGY_H
#define LOWEST_HEALTH_TARGET_STRATEGY_H

#include "SelectionStrategy.h"

class LowestHealthTargetStrategy : public TargetSelectionStrategy {
public:
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // LOWEST_HEALTH_TARGET_STRATEGY_H
