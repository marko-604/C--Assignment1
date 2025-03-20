
#ifndef HIGHEST_HEALTH_TARGET_STRATEGY_H
#define HIGHEST_HEALTH_TARGET_STRATEGY_H

#include "../../Critters/Critter.h"
#include "SelectionStrategy.h"

class HighestHealthTargetStrategy : public TargetSelectionStrategy {
public:
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // HIGHEST_HEALTH_TARGET_STRATEGY_H
