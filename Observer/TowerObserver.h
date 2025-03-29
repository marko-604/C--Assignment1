// TowerObserver.h
#ifndef TOWEROBSERVER_H
#define TOWEROBSERVER_H

#include "../Towers/Tower.h" ///< Includes definitions for Tower and its derived types.
#include "Observer.h"        ///< Base Observer interface.
#include "Subject.h"         ///< Forwarded notifications come from Subject.

/**
 * @brief Concrete observer for monitoring Tower-related changes.
 *
 * This observer reacts to updates from any Tower (or Subject-derived) instance.
 */
class TowerObserver : public Observer {
public:
  /// @brief Default constructor.
  TowerObserver() {}

  /**
   * @brief Respond to updates from the observed subject (typically a Tower).
   *
   * @param subject Pointer to the subject that triggered the update.
   */
  void Update(Subject *subject) override;
};

#endif // TOWEROBSERVER_H
