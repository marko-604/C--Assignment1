#include "FreezingDecorator.h"
#include "../../Critters/Critter.h"
#include "../../Maps/Map.h"
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
    int towerX = baseTower->getX();
    int towerY = baseTower->getY();
    std::cout << "FreezingDecorator: Attack executed. Applying freezing effect."
              << std::endl;

    // Find the first critter within the base tower's range and apply the
    // freezing effect.
    for (Critter *critter : critters) {
      int row = critter->getRow();
      int col = critter->getCol();
      int dx = towerX - row;
      int dy = towerY - col;
      int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
      if (distance <= baseTower->getRange()) {
        // Instead of calling applySlow(), we increment the critter's speed.
        // We assume a higher speed value means the critter moves slower.
        float currentSpeed = critter->getSpeed();
        critter->setSpeed(currentSpeed + slowRate);
        std::cout << "FreezingDecorator: Critter at (" << row << ", " << col
                  << ") slowed by adding " << slowRate << " to its speed."
                  << std::endl;
        break; // Apply the freezing effect to only one critter.
      }
    }
  }
  return attacked;
}
