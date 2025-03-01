#include "CritterObserver.h"
#include "../Critters/Critter.h"
#include <iostream>
void CritterObserver::Update(Subject *subject) {
  Critter *critter = dynamic_cast<Critter *>(subject);

  std::cout << "Critter CID = " << critter->getCID() << "\n"
            << "Life = " << critter->getHealth()
            << "\nSpeed = " << critter->getSpeed()
            << "\nStrength = " << critter->getStr() << "\nPosition = ("
            << critter->getRow() << ", " << critter->getCol() << ")\n";

  if (critter->getType() == SQUIRREL) {
    std::cout << "Type = SQUIRREL" << std::endl;
  } else if (critter->getType() == WOLF) {
    std::cout << "Type = WOLF" << std::endl;
  } else {
    std::cout << "Type = BEAR" << std::endl;
  }
}
