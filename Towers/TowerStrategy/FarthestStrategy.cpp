
#include "FarthestStrategy.h"
#include "../../Critters/Critter.h"
#include "../Tower.h"
#include <cmath>

Critter *
FarthestTargetStrategy::selectTarget(const std::vector<Critter *> &critters,
                                     Tower *tower) {
  int towerX = tower->getX();
  int towerY = tower->getY();
  Critter *selected = nullptr;
  int maxDistance = -1;
  for (Critter *critter : critters) {
    int row = critter->getRow();
    int col = critter->getCol();
    int dx = towerX - row;
    int dy = towerY - col;
    int distance = static_cast<int>(std::sqrt(dx * dx + dy * dy));
    if (distance <= tower->getRange() && distance > maxDistance) {
      maxDistance = distance;
      selected = critter;
    }
  }
  return selected;
}
