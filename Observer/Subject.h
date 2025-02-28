#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <algorithm>
#include <vector>

class Subject {
private:
  std::vector<Observer *> observers;

public:
  virtual ~Subject() {}

  void attach(Observer *obs) { observers.push_back(obs); };

  void Detach(Observer *obs) {
    std::remove(observers.begin(), observers.end(), obs);
    observers.end();
  }
};

#endif
