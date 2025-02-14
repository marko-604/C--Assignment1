#include "critter.h"
#include <iostream>


#include <string>
using namespace std;


namespace crits {

    class Critter {

      protected :
      string reward;
      double hitpoints;
      double strength;
      double speed;
      int level;
      int x,y;
      int exitX, exitY;

    public :
   Critter(string reward, double hitpoints, double strength, double speed, int level, int x, int y, int exitX, int exitY) {
        this->hitpoints =hitpoints;
        this->reward = reward;
        this->strength = strength; 
        this-> speed = speed;
        this->level = level;
        this->x=x;
        this->y=y;
        this->exitX =exitX;
        this->exitY =exitY;

    }

    virtual ~Critter() {}
    virtual void move()  {
        
        if (x < exitX) x++;
        else if (x > exitX) x--;
        else if (y > exitY) y--;
        else if (y < exitY) y++;
        cout << "Critter moved to ("<< x <<","<< y <<")" <<endl;
    }
    
    virtual void takeDamage(double damage) {
        hitpoints -= damage;
    }
    bool isDead() {
        return hitpoints <= 0;
    }

    bool reachedexit() {
        return (x == exitX && y == exitY);
    }
    };

    class FastCritter : public Critter {
    public :
    FastCritter(int startX, int startY, int endX, int endY) :Critter ("Fast",50,1,2,1,startX,startY,endX,endY){

    }

    void move () override {
        for (int i=0; i<2; i++){
        if (x < exitX) x++;
        else if (x > exitX) x--;
        else if (y > exitY) y--;
        else if (y < exitY) y++;
        }
        cout << "Critter moved to ("<< x <<","<< y <<")" <<endl;
    }
    



};

class SolidCritter : public Critter {
    public :
    SolidCritter(int startX, int startY, int endX, int endY) : Critter ("Durable", 150, 4,1, 2, startX,startY, endX, endY) {

    }
};

class FinalBossCritter : public Critter {
    public : 
    FinalBossCritter(int startX, int startY, int endX, int endY) : Critter ("Unstoppable",400, 7, 4,4, startX,startY, endX, endY) {
    }
     void move () override {
        for (int i=0; i<4; i++){
        if (x < exitX) x++;
        else if (x > exitX) x--;
        else if (y > exitY) y--;
        else if (y < exitY) y++;
        }
        cout << "Critter moved to ("<< x <<","<< y <<")" <<endl;
    }
};

class CritterGenerator {
     private : 
        std::vector<std::unique_ptr<Critter>> critters;
        int waveNumber;

public : 
    CritterGenerator() : waveNumber(1) {};
    void generateWave (int startX, int startY, int endX, int endY) {
        std::cout << "Generating wave " << waveNumber << "...\n";

        for (int i=0; i< waveNumber * 2; i++) {
            if (i % 3 == 0) {
                critters.push_back(std::make_unique<FastCritter> (startX,startY,endX, endY));
            }
            else if ( i % 3 ==1) {
             critters.push_back(std::make_unique<SolidCritter> (startX, startY, endX, endY));

                    
            }
            else {
                critters.push_back(std::make_unique<FinalBossCritter> (startX, startY, endX, endY));

            }
            waveNumber++;
        }
    }


    void moveCritters() {
        for (auto& critter : critters ) {
            critter->move();
        }
    }

    void removeDeadCritters() {
    critters.erase(std::remove_if(critters.begin(), critters.end(),
        [](const std::unique_ptr<Critter>& critter) { return critter->isDead(); }),
        critters.end());
}
};

}

