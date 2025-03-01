// TowerObserver.h
#ifndef TOWEROBSERVER_H
#define TOWEROBSERVER_H

#include "../Towers/Tower.h" // so we can reference Tower, BombTower, etc.
#include "Observer.h"        // base observer interface
#include "Subject.h"

class TowerObserver : public Observer {
public:
  TowerObserver() {}
  void Update(Subject *subject) override;
};

#endif
