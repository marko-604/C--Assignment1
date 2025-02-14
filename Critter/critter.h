#ifndef CRITTER_H
#define CRITTER_H

#include <string>
#include <iostream>
#include <vector>
using namespace std;


namespace crits {

    class Critter {


    public :
    Critter(string reward, double hitpoints, double strength, double speed, int level, int startX, int startY, int endX, int endY);
    virtual void move();
    virtual void takeDamage(double damage);
    bool isDead();
    };
    class FastCritter: public Critter {
        public :
        FastCritter(int startX, int startY, int endX, int endY) ;


};
class SolidCritter : public Critter {
        public :
        SolidCritter(int startX, int startY, int endX, int endY) ;


};

class FinalBossCritter : public Critter {
        public :
        FinalBossCritter(int startX, int startY, int endX, int endY) ;


};

class CritterGenerator {
   public : 
   void generateWave(int startX, int startY, int endX, int endY);
   void moveCritters();
   void removeDeadCritters();



};
}











#endif

