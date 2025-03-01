// Observer.h
#ifndef OBSERVER_H
#define OBSERVER_H

// Forward-declare Subject to avoid circular includes
class Subject;
/**
 * @brief The Observer interface.
 */
class Observer {
public:
  virtual ~Observer() {}
  /**
   * @brief Called by Subject whenever the subject changes.
   * @param subject The subject that changed
   */
  virtual void Update(Subject *subject) = 0;
};

#endif
