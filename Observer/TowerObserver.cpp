#include "TowerObserver.h"

// The definition of the Update function for the tower observer class
void TowerObserver::Update(Subject *subject) {
  // cast subject to Tower
  Tower *tower = dynamic_cast<Tower *>(subject);
  if (!tower)
    return; // safety check

  // Now we can see tower->getType(), tower->getDamage(), etc.
  std::cout << "[TowerObserver] Tower ID=" << tower->getTid() << "\n"
            << " Position=(" << tower->getX() << "," << tower->getY() << ")\n"
            << " Damage=" << tower->getDamage() << "\n"
            << " Attack Rate=" << tower->getAttaRate() << "\n"
            << " Range=" << tower->getRange() << "\n"
            << " Hit Rate = " << tower->getHitRate() << std::endl;
  std::cout << " Level = " << tower->getLevel() << std::endl;

  // If we need BombTower, FreezingTower, etc. we cast again

  std::cout << std::endl;
}
