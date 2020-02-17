#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::addActor(Actor* actor)
{
    actors.push_back(actor);
}

int StudentWorld::init()
{
    const double PI = 4 * atan(1);
    soc = new Socrates(this);
    int level = getLevel();
    int nPits = level;
    for (int i=0; i<nPits; i++)
    {
//        int dx = randInt(0, VIEW_WIDTH);
//        int dy = randInt(0, VIEW_HEIGHT);
//        for (int j=i-1; j>=0; j--)
//        {
//            Actor* currentActor = actors[actors.size()-j];
//            while ( ( ((dx-VIEW_RADIUS)^2) + ((dy-VIEW_RADIUS)^2) ) > 14400 || ( (abs(dx - currentActor->getX()) < 2*SPRITE_RADIUS) && abs(dy - currentActor->getY()) < 2*SPRITE_RADIUS))
//            {
//                dx = randInt(0, VIEW_WIDTH);
//                dy = randInt(0, VIEW_HEIGHT);
//            }
//        }
        int angle = randInt(0, 360);
        int radius = randInt(0, 120);
        double dx = radius * cos(angle/180*PI) + VIEW_RADIUS;
        double dy = radius * sin(angle/180*PI) + VIEW_RADIUS;
        for (int j=i-1; j>=0; j--)
            {
                Actor* currentActor = actors[actors.size()-j];
                int tx = currentActor->getX();
                int ty = currentActor->getY();
                while ( (((((int)dx-tx)^2) + (((int)dy-ty)^2))^(1/2)) < 2*SPRITE_RADIUS )
                {
                    angle = randInt(0, 360);
                    radius = randInt(0, 120);
                    dx = radius * cos(angle/180*PI) + VIEW_RADIUS;
                    dy = radius * sin(angle/180*PI) + VIEW_RADIUS;
                }
            }
        actors.push_back(new Pit(this, dx, dy));
    }
    int nFood = min(5*level, 25);
    for (int i=0; i<nFood; i++)
    {
        int angle = randInt(0, 360);
        int radius = randInt(0, 120);
        double dx = radius * cos(angle/180*PI) + VIEW_RADIUS;
        double dy = radius * sin(angle/180*PI) + VIEW_RADIUS;
        for (int j=i+nPits-1; j>=0; j--)
            {
                Actor* currentActor = actors[actors.size()-j-1];
                int tx = currentActor->getX();
                int ty = currentActor->getY();
                while ( (((((int)dx-tx)^2) + (((int)dy-ty)^2))^(1/2)) < 2*SPRITE_RADIUS )
                {
                    angle = randInt(0, 360);
                    radius = randInt(0, 120);
                    dx = radius * cos(angle/180*PI) + VIEW_RADIUS;
                    dy = radius * sin(angle/180*PI) + VIEW_RADIUS;
                }
            }
        actors.push_back(new Food(this, dx, dy));
    }
    int nDirt = max(180 - 20*level, 20);
    for (int i=0; i<nDirt; i++)
    {
        int angle = randInt(0, 360);
        int radius = randInt(0, 120);
        double dx = radius * cos(angle/180*PI) + VIEW_RADIUS;
        double dy = radius * sin(angle/180*PI) + VIEW_RADIUS;
        for (int j=i+nPits+nFood-1; j>=i; j--)
            {
                Actor* currentActor = actors[actors.size()-j-1];
                int tx = currentActor->getX();
                int ty = currentActor->getY();
                while ( (((((int)dx-tx)^2) + (((int)dy-ty)^2))^(1/2)) < 2*SPRITE_RADIUS )
                {
                    angle = randInt(0, 360);
                    radius = randInt(0, 120);
                    dx = radius * cos(angle/180*PI) + VIEW_RADIUS;
                    dy = radius * sin(angle/180*PI) + VIEW_RADIUS;
                }
            }
        actors.push_back(new DirtPile(this, dx, dy));
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    vector<Actor*>::iterator itr = actors.begin();
    soc->doSomething();
    if (!soc->getLifeStatus())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    while (itr != actors.end())
    {
        if ((*itr)->getLifeStatus())
        {
            (*itr)->doSomething();
            if (!soc->getLifeStatus())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if ((*itr)->exhausted())
            {
                advanceToNextLevel();
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    
    // remove dead game objects
    vector<Actor*> temp;
    for (int i=0; i<actors.size(); i++)
    {
        if (actors[i]->getLifeStatus())
        {
            temp.push_back(actors[i]);
        }
        else
        {
            delete actors[i];
        }
    }
    actors = temp;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (vector<Actor*>::iterator itr = actors.begin(); itr != actors.end(); )
    {
        delete *itr;
        itr = actors.erase(itr);
    }
    delete soc;
}
