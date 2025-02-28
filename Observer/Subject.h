// Subject.h
#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h" // This is okay if Observer doesn't also include Subject
#include <algorithm>
#include <vector>

class Subject {
protected:
  std::vector<Observer *> observers;

public:
  virtual ~Subject() {}

  void Attach(Observer *obs) { observers.push_back(obs); }

  void Detach(Observer *obs) {
    // properly remove using erase–remove
    observers.erase(std::remove(observers.begin(), observers.end(), obs),
                    observers.end());
  }

  void Notify() {
    for (Observer *obs : observers) {
      obs->Update(this);
    }
  }
};

#endif
