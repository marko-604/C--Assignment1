#ifndef CRITTER_OBSERVER_H
#define CRITTER_OBSERVER_H

#include "Observer.h"
#include "Subject.h"

/**
 * @brief Concrete observer for monitoring updates to Critter subjects.
 *
 * This observer reacts when a critter (or any Subject) changes state.
 */
class CritterObserver : public Observer {
public:
  /**
   * @brief Respond to updates from the observed subject.
   *
   * This method is called whenever the Subject notifies its observers of a
   * change.
   *
   * @param subject Pointer to the subject that triggered the update.
   */
  void Update(Subject *subject) override;
};

#endif // !CRITTER_OBSERVER_H
