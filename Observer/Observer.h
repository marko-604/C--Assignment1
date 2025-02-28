#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
public:
  virtual ~Observer();

  virtual void Update(Subject *subject) = 0;
};

#endif // #ifndef OBSERVER_H
