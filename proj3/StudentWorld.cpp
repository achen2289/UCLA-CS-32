#include "StudentWorld.h"
#include "Actor.h" // actor functions
#include "GameConstants.h" // for "Direction"
#include <cmath> // trig functions, powers
#include <string> // strings
#include <list> // actor container
#include <sstream> // output stringstream
#include <iomanip> // output stringstream
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    soc = nullptr;
    m_numBacteriaKilled = 0;
    m_numAdditionalBacteria = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::incrementAddedBac()
{
    m_numAdditionalBacteria++;
}

void StudentWorld::addActor(Actor* actor)
{
    actors.push_back(actor);
}

double StudentWorld::getDistanceBetween(double dx, double tx, double dy, double ty)
{
    return pow(pow(dx - tx, 2.0) + pow(dy - ty, 2.0), 0.5);
}

double StudentWorld::distanceFromSocrates(double dx, double dy, double& tx, double& ty)
{
    tx = soc->getX();
    ty = soc->getY();
    return getDistanceBetween(dx, tx, dy, ty);
}

// get direction of closest food
// if closest food is not within 128 pixels then return -1
Direction StudentWorld::getClosestFood(Direction dx, Direction dy)
{
    const double PI = 4 * atan(1);
    for (list<Actor*>::iterator itr = actors.begin(); itr != actors.end(); itr++)
    {
        if ((*itr)->edibleByBacteria())
        {
            double tx = (*itr)->getX();
            double ty = (*itr)->getY();
            if (getDistanceBetween(dx, tx, dy, ty) <= 128)
            {
                double opp = ty - dy;
                double adj = tx - dx;
                return atan2(opp, adj)*1.0*180/PI;
            }
        }
    }
    return -1;
}

// check for overlap with Socrates, dirtPile, Food, or Flamethrower goodie
bool StudentWorld::checkForOverlap(Direction dx, Direction dy, int overlap, int hpChange, bool withSocrates, bool withDirtPile, bool withFood, bool isFlamethrower)
{
    if (withSocrates)
    {
        double tx = soc->getX();
        double ty = soc->getY();
        if (getDistanceBetween(dx, tx, dy, ty) <= overlap)
        {
            // if object is a flamethrower goodie, add 5 charges
            if (isFlamethrower)
                soc->addFlameThrowerCharges(5);
            soc->changeHP(hpChange);
            return true;
        }
        return false;
    }
    for (list<Actor*>::iterator itr = actors.begin(); itr != actors.end(); itr++)
    {
        if (withDirtPile)
        {
            if ((*itr)->blocksBacterialMovement())
            {
                double tx = (*itr)->getX();
                double ty = (*itr)->getY();
                if (getDistanceBetween(dx, tx, dy, ty) <= overlap)
                {
                    return true;
                }
            }
        }
        else if (withFood)
        {
            if ((*itr)->edibleByBacteria() && (*itr)->getLifeStatus())
            {
                double tx = (*itr)->getX();
                double ty = (*itr)->getY();
                if (getDistanceBetween(dx, tx, dy, ty) <= overlap)
                {
                    // kill food if overlapping
                    (*itr)->changeHP(hpChange);
                    return true;
                }
            }
        }
        else if ((*itr)->isDamageable())
        {
            double tx = (*itr)->getX();
            double ty = (*itr)->getY();
            if (getDistanceBetween(dx, tx, dy, ty) <= overlap)
            {
                // damage actor if damageable
                (*itr)->changeHP(hpChange);
//                if ((*itr)->emittedByPit() && !(*itr)->getLifeStatus())
//                    m_numBacteriaKilled++;
                return true;
            }
        }
    }
    return false;
}

// add randomly located pits, food, and dirt
// food and pits cannot overlap with anything
// dirt piles can overlap with themselves
int StudentWorld::init()
{
    m_numBacteriaKilled = 0;
    m_numAdditionalBacteria = 0;
    const double PI = 4 * atan(1);
    soc = new Socrates(this);
    int level = getLevel();
    int nPits = level;
    for (int i=0; i<nPits; i++)
    {
        int angle = randInt(0, 360);
        int radius = randInt(0, 120);
        double dx = radius * cos(angle*1.0/180*PI) + VIEW_RADIUS;
        double dy = radius * sin(angle*1.0/180*PI) + VIEW_RADIUS;
        
        // iterate through all pits to ensure no overlap
        list<Actor*>::iterator itr = actors.begin();
        int j = i-1;
        for ( ; j>=0; j--, itr++)
        {
            Actor* currentActor = *itr;
            double tx = currentActor->getX();
            double ty = currentActor->getY();
            while (getDistanceBetween(dx, tx, dy, ty) <= SPRITE_WIDTH)
            {
                angle = randInt(0, 360);
                radius = randInt(0, 120);
                dx = radius * cos(angle*1.0/180*PI) + VIEW_RADIUS;
                dy = radius * sin(angle*1.0/180*PI) + VIEW_RADIUS;
                
                // restart inner for loop if overlapping
                itr = actors.begin();
                j = i-1;
            }
        }
        actors.push_back(new Pit(this, dx, dy));
    }
    int nFood = min(5*level, 25);
    for (int i=0; i<nFood; i++)
    {
        int angle = randInt(0, 360);
        int radius = randInt(0, 120);
        double dx = radius * cos(angle*1.0/180*PI) + VIEW_RADIUS;
        double dy = radius * sin(angle*1.0/180*PI) + VIEW_RADIUS;
        
        // iterate through all pits and food to ensure no overlap
        list<Actor*>::iterator itr = actors.begin();
        int j = i+nPits-1;
        for ( ; j>=0; j--, itr++)
        {
            Actor* currentActor = *itr;
            double tx = currentActor->getX();
            double ty = currentActor->getY();
            while (getDistanceBetween(dx, tx, dy, ty) <= SPRITE_WIDTH)
            {
                angle = randInt(0, 360);
                radius = randInt(0, 120);
                dx = radius * cos(angle*1.0/180*PI) + VIEW_RADIUS;
                dy = radius * sin(angle*1.0/180*PI) + VIEW_RADIUS;
                
                // restart inner for loop if overlapping
                itr = actors.begin();
                j = i+nPits-1;
            }
        }
        actors.push_back(new Food(this, dx, dy));
    }
    int nDirt = max(180 - 20*level, 20);
    for (int i=0; i<nDirt; i++)
    {
        int angle = randInt(0, 360);
        int radius = randInt(0, 120);
        double dx = radius * cos(angle*1.0/180*PI) + VIEW_RADIUS;
        double dy = radius * sin(angle*1.0/180*PI) + VIEW_RADIUS;
        
        // iterate through all pits and food to ensure no overlap
        // does not iterate through dirt piles
        list<Actor*>::iterator itr = actors.begin();
        int j = i+nPits+nFood-1;
        for ( ; j>=i; j--, itr++)
        {
            Actor* currentActor = *itr;
            double tx = currentActor->getX();
            double ty = currentActor->getY();
            while (getDistanceBetween(dx, tx, dy, ty) <= SPRITE_WIDTH)
            {
                angle = randInt(0, 360);
                radius = randInt(0, 120);
                dx = radius * cos(angle*1.0/180*PI) + VIEW_RADIUS;
                dy = radius * sin(angle*1.0/180*PI) + VIEW_RADIUS;
                
                // restart inner for loop if overlapping
                itr = actors.begin();
                j = i+nPits+nFood-1;
            }
        }
        actors.push_back(new DirtPile(this, dx, dy));
    }
    return GWSTATUS_CONTINUE_GAME;
}

// give all actors a chance to doSomething()
// randomly spawn goodies and fungi along petri dish
// remove all dead actors
// check if game has ended
// update game stat  text
int StudentWorld::move()
{
    if (!soc->getLifeStatus())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    soc->doSomething();
    
    list<Actor*>::iterator itr = actors.begin();
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
//            if ((*itr)->exhausted())
//            {
//                advanceToNextLevel();
//                return GWSTATUS_FINISHED_LEVEL;
//            }
        }
        itr++;
    }
    
    // chance to spawn goodies and fungus
    int level = getLevel();
    int chanceSpawn = max(510 - level*10, 200);
    spawnObj(chanceSpawn, true);
    chanceSpawn = max(510 - level*10, 250);
    spawnObj(chanceSpawn);
    
    // remove dead game objects
    for (list<Actor*>::iterator itr = actors.begin(); itr != actors.end(); )
    {
        if (!(*itr)->getLifeStatus())
        {
            if ((*itr)->emittedByPit())
                m_numBacteriaKilled++;
            delete *itr;
            itr = actors.erase(itr);
        }
        else
            itr++;
    }
    
    // check if all bacteria are killed
    if (m_numBacteriaKilled == (getLevel()*10 + m_numAdditionalBacteria))
        return GWSTATUS_FINISHED_LEVEL;
    
    // update game stat text
    ostringstream oss;
    oss.fill('0');
    if (getScore() < 0)
        oss << "Score: " << '-' << setw(5) << abs(getScore());
    else
        oss << "Score: " << setw(6) << abs(getScore());
    oss.fill(' ');
    oss << setw(9) << "Level: " << getLevel();
    oss << setw(9) << "Lives: " << getLives();
    oss << setw(10) << "Health: " << soc->getHP();
    oss << setw(10) << "Sprays: " << soc->getDisinfectant();
    oss << setw(10) << "Flames: " << soc->getFlameCharges();
    setGameStatText(oss.str());
    
    // continue game
    return GWSTATUS_CONTINUE_GAME;
}

// reset number of bacteria killed and additional bacteria created
// delete all actors
// is called in destructor
void StudentWorld::cleanUp()
{
    for (list<Actor*>::iterator itr = actors.begin(); itr != actors.end(); )
    {
        delete *itr;
        itr = actors.erase(itr);
    }
    delete soc;
    soc = nullptr;
}

// PRIVATE FUNCTIONS

// give opportunities to randomly spawn fungi or goodies along petri dish
void StudentWorld::spawnObj(int chance, bool fungus)
{
    const double PI = 4 * atan(1);
    int angle = randInt(0, 360);
    double newX = VIEW_RADIUS + VIEW_RADIUS * cos(angle*1.0*PI/180);
    double newY = VIEW_RADIUS + VIEW_RADIUS * sin(angle*1.0*PI/180);
    
    if (randInt(0, chance) == 0)
    {
        if (fungus)
        {
            addActor(new Fungus(this, newX, newY));
            return;
        }
        int rand = randInt(0, 9);
        if (rand < 1)
            addActor(new ExtraLifeGoodie(this, newX, newY));
        else if (rand < 4)
            addActor(new FlamethrowerGoodie(this, newX, newY));
        else
            addActor(new RestoreHealthGoodie(this, newX, newY));
    }
}
