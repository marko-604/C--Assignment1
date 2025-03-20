#ifndef STRONGEST_TARGET_STRATEGY_H
#define STRONGEST_TARGET_STRATEGY_H

#include "SelectionStrategy.h"

class StrongestTargetStrategy : public TargetSelectionStrategy {
public:
  virtual Critter *selectTarget(const std::vector<Critter *> &critters,
                                Tower *tower) override;
};

#endif // STRONGEST_TARGET_STRATEGY_H
