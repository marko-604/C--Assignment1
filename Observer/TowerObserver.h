// TowerObserver.h
#ifndef TOWEROBSERVER_H
#define TOWEROBSERVER_H

#include "../Towers/Tower.h" // so we can reference Tower, BombTower, etc.
#include "Observer.h"        // base observer interface

class TowerObserver : public Observer {
public:
  void Update(Subject *subject) override {
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
              << "Hit Rate = " << tower->getHitRate() << std::endl;

    // If we need BombTower, FreezingTower, etc. we cast again
    switch (tower->getType()) {
    case FREEZING: {
      auto freeze = dynamic_cast<FreezingTower *>(tower);
      if (freeze) {
        std::cout << "Type=FREEZING, SlowRate=" << freeze->getSlowRate()
                  << "\n";
      }
      break;
    }
    case BOMB: {
      auto bomb = dynamic_cast<BombTower *>(tower);
      if (bomb) {
        std::cout << "Type=BOMB, SplashArea=" << bomb->getSplash() << "\n";
      }
      break;
    }
    case SNIPER:
      std::cout << "Type=SNIPER\n";
      break;
    default:
      std::cout << "Type=REGULAR\n";
      break;
    }
    std::cout << std::endl;
  }
};

#endif
