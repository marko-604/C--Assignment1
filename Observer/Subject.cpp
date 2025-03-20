#include "Subject.h"
#include "Observer.h"
#include <iostream>

void Subject::Attach(Observer *obs) { observers.push_back(obs); }

void Subject::Detach(Observer *obs) {
  // properly remove using erase–remove
  observers.erase(std::remove(observers.begin(), observers.end(), obs),
                  observers.end());
}

void Subject::DetachAll() {
  while (!observers.empty()) {
    observers.pop_back();
  }
}

void Subject::Notify() {
  if (observers.empty())
    return;
  for (Observer *obs : observers) {
    obs->Update(this);
  }
}
