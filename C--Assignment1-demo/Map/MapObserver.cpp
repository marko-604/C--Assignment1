#include "MapObserver.h"
#include <iostream>

MapObserver::MapObserver(Map* map) {
    subject = map;
    subject->attach(this);  // Attach observer to map
}

void MapObserver::update() {
    std::cout << "Map has been updated:\n";
    subject->displayMap();
}
