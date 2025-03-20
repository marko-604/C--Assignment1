
#include "FreezingDecorator.h"
#include "../../Critters/Critter.h"
#include "../../Maps/Map.h"
#include "../TowerStrategy/Strategies.h" // Make sure this is the correct path.
#include <cmath>
#include <iostream>

FreezingDecorator::FreezingDecorator(Tower *tower, float slowRateVal)
    : TowerDecorator(tower), slowRate(slowRateVal) {}

bool FreezingDecorator::attack(std::vector<Critter *> &critters, int tick_count,
                               int *player_points, Map &gameMap) {
  // Call the underlying tower's attack.
  bool attacked =
      baseTower->attack(critters, tick_count, player_points, gameMap);
  if (attacked) {
    std::cout << "FreezingDecorator: Attack executed. Applying freezing effect."
              << std::endl;

    // Attempt to select a target using the tower's strategy.
    Critter *target = nullptr;
    if (baseTower->getStrategy() != nullptr) {
      target = baseTower->getStrategy()->selectTarget(critters, baseTower);
    }

    // Fallback: if no target was selected by the strategy, find the first
    // critter in range.
    if (target == nullptr) {
      int towerX = baseTower->getX();
      int towerY = baseTower->getY();
      for (Critter *critter : critters) {
        int row = critter->getRow();
        int col = critter->getCol();
        int dx = towerX - row;
        int dy = towerY - col;
        int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
        if (distance <= baseTower->getRange()) {
          target = critter;
          break;
        }
      }
    }

    // If a target is found, apply the freezing effect.
    if (target != nullptr) {
      float currentSpeed = target->getSpeed();
      target->setSpeed(currentSpeed + slowRate);
      std::cout << "FreezingDecorator: Critter at (" << target->getRow() << ", "
                << target->getCol() << ") slowed by adding " << slowRate
                << " to its speed." << std::endl;
    }
  }
  return attacked;
}
