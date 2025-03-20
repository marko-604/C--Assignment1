
#include "WeakestStrategy.h"
#include "../../Critters/Critter.h"
#include "../Tower.h"
#include <cmath>
#include <limits>
Critter *
WeakestTargetStrategy::selectTarget(const std::vector<Critter *> &critters,
                                    Tower *tower) {
  int towerX = tower->getX();
  int towerY = tower->getY();
  Critter *selected = nullptr;
  int range = tower->getRange();
  int minStrength = std::numeric_limits<int>::max();

  for (Critter *critter : critters) {
    int row = critter->getRow();
    int col = critter->getCol();
    int dx = towerX - row;
    int dy = towerY - col;
    int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
    if (distance <= range) {
      int strength = critter->getStr();
      if (strength < minStrength) {
        minStrength = strength;
        selected = critter;
      }
    }
  }
  return selected;
}
