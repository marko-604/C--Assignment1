#ifndef OBSERVER_H
#define OBSERVER_H
#include "../Towers/Tower.h"
#include "Subject.h"
#include <iostream>

class Observer {
public:
  virtual ~Observer();

  virtual void Update(Subject *subject) = 0;
};

class TowerObserver : public Observer {
public:
  void Update(Subject *subject) override {
    Tower *tower = dynamic_cast<Tower *>(subject);
    std::cout << "[TowerObserver] Tower ID = " << tower->getTid() << std::endl
              << " Position = (" << tower->getX() << ", " << tower->getY()
              << ")" << std::endl
              << "Level = " << tower->getLevel() << std::endl
              << "Damage = " << tower->getDamage() << std::endl
              << "Fire Rate = " << tower->getAttaRate() << std::endl
              << "Hit Rate = " << tower->getHitRate() << std::endl
              << "Cost = " << tower->getCost()
              << "Resale = " << tower->getResale() << std::endl
              << "Range = " << tower->getRange() << std::endl;

    if (tower->getType() == FREEZING) {
      std::cout << "Type = FREEZING" << std::endl;
      FreezingTower *t = dynamic_cast<FreezingTower *>(tower);
      std::cout << "Slow Rate = " << t->getSlowRate() << std::endl;

    } else if (tower->getType() == BOMB) {
      BombTower *t = dynamic_cast<BombTower *>(tower);
      std::cout << "Type = BOMB" << std::endl;
      std::cout << "Splash Area = " << t->getSplash() << std::endl;
    } else if (tower->getType() == SNIPER) {
      std::cout << "Type = SNIPER" << std::endl;
    } else {
      std::cout << "Type = REGULAR" << std::endl;
    }
  }
};

#endif // #ifndef OBSERVER_H
