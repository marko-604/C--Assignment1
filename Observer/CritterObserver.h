#ifndef CRITTER_OBSERVER_H
#define CRITTER_OBSERVER_H

#include "Observer.h"
#include "Subject.h"

class CritterObserver : public Observer {
public:
  void Update(Subject *subject) override;
};

#endif // !CRITTER_OBSERVER_H
