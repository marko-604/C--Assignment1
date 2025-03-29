#ifndef MAP_OBSERVER_H
#define MAP_OBSERVER_H

#include "Observer.h"
#include <string>
#include <vector>

/**
 * @brief Concrete observer that listens to updates from a Subject (typically a
 * Map).
 *
 * It logs update messages to a shared vector of strings.
 */
class MapObserver : public Observer {
public:
  /// @brief Pointer to a shared vector where messages will be stored.
  std::vector<std::string> *messages;

  /**
   * @brief Construct a new MapObserver object.
   *
   * @param m Pointer to a shared vector of messages used for logging.
   */
  MapObserver(std::vector<std::string> *m) : messages(m) {}

  /**
   * @brief Handles updates from the subject.
   *
   * This method is called whenever the observed subject changes.
   *
   * @param subject Pointer to the subject that triggered the update.
   */
  void Update(Subject *subject) override;
};

#endif // MAP_OBSERVER_H
