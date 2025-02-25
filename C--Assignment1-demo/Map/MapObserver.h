//
// Created by m_mat on 2025-02-25.
//

#ifndef C_ASSIGNMENT1_DEMO_MAPOBSERVER_H
#define C_ASSIGNMENT1_DEMO_MAPOBSERVER_H
#include "../Observer.h"
#include "Map.h"

class MapObserver : public Observer {
private:
    Map* subject;  // The map being observed

public:
    MapObserver(Map* map);
    void update() override; // Called when the map changes
};
#endif //C_ASSIGNMENT1_DEMO_MAPOBSERVER_H
