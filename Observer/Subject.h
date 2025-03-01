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
  Subject() {}
  virtual ~Subject() {}
  void Attach(Observer *obs);
  void Detach(Observer *obs);
  void DetachAll();
  void Notify();
};

#endif
