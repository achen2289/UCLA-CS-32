#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h" // base class
#include "GraphObject.h" // for "Direction"
#include <string> // strings
#include <list> // for actor container

class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld(); // calls cleanUp()
    void incrementAddedBac(); // increment num of added bacteria
    void addActor(Actor* actor); // add actor to world
    double getDistanceBetween(double dx, double tx, double dy, double ty); // get distance between two points
    double distanceFromSocrates(double dx, double dy, double& tx, double& ty); // get socrates position and return distance from socrates
    Direction getClosestFood(Direction dx, Direction dy); // get direction to closest food
    bool checkForOverlap(Direction dx, Direction dy, int overlap, int hpChange, bool withSocrates, bool withDirtPile, bool withFood, bool isFlamethower); // check for overlap with objects based on passed parameters
    virtual int init(); // initialize socrates and all pits, food, and dirt piles
    virtual int move(); // calls doSomething() for each actor
    virtual void cleanUp(); // cleans up all dynamically allocated data and resets member variables
    
private:
    int m_numBacteriaKilled;
    int m_numAdditionalBacteria;
    Actor* soc;
    std::list<Actor*> actors;
    void spawnObj(int chance, bool fungus = false);
};

#endif // STUDENTWORLD_H_
