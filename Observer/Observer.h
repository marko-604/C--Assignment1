// Observer.h
#ifndef OBSERVER_H
#define OBSERVER_H

/// @brief Forward declaration of Subject to avoid circular dependency.
class Subject;

/**
 * @brief Interface for all observers in the Observer pattern.
 *
 * Classes that inherit from Observer can be notified when a Subject changes.
 */
class Observer {
public:
  /// @brief Virtual destructor for safe polymorphic destruction.
  virtual ~Observer() {}

  /**
   * @brief Method called by the Subject when its state changes.
   *
   * @param subject Pointer to the Subject that triggered the update.
   */
  virtual void Update(Subject *subject) = 0;
};

#endif // OBSERVER_H
