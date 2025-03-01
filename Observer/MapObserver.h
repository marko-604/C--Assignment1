#ifndef MAP_OBSERVER_H
#define MAP_OBSERVER_H

#include "Observer.h"

class MapObserver : public Observer {
public:
  void Update(Subject *subject) override;
};

#endif
