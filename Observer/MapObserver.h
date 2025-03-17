#ifndef MAP_OBSERVER_H
#define MAP_OBSERVER_H

#include "Observer.h"
#include <string>
#include <vector>

class MapObserver : public Observer {
public:
  // Store a pointer to the shared messages vector
  std::vector<std::string> *messages;

  // Modify constructor to accept a pointer to the vector
  MapObserver(std::vector<std::string> *m) : messages(m) {}
  void Update(Subject *subject) override;
};

#endif
