#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h" // base class
#include <set> // for pit
#include <string> // for string

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir, int depth = 1, double size = 1.0);
    virtual ~Actor() {}
    virtual int getHP(); // get HP of actor, default is 0
    virtual void changeHP(int change, int noiseDie = SOUND_NONE, int noiseHurt = SOUND_NONE); // all actors must have, does nothing by  default
    bool getLifeStatus() const; // return if alive or not
    void setLifeStatus(bool status); // set to alive or dead
    virtual bool isDamageable(); // true for damageable actors except for food (all actors that socrates can damage)
    virtual bool emittedByPit(); // true for bacteria
    virtual bool exhausted() const; // determine if pit emitted all bacteria
    virtual bool edibleByBacteria(); // true for food
    virtual bool blocksBacterialMovement(); // true for dirt piles
    virtual int getDisinfectant(); // socrates spray charges
    virtual int getFlameCharges(); // socrates flame charges
    virtual void addFlameThrowerCharges(int num); // add flame charges for socrates
    virtual void playSound(int soundID); // play specified sound
    virtual void doSomething(); // does nothing by default
    StudentWorld* sWorldPointer(); // return StudentWorld pointer
private:
    bool m_alive;
    StudentWorld* m_game;
};

// cannot be killed
class Disinfectant : public Actor
{
public:
    Disinfectant(StudentWorld* sWorld, double dx, double dy, Direction direction);
    virtual ~Disinfectant() {}
    virtual void doSomething();
private:
    int m_pixelsTraveled;
};

// cannot be killed
class Flame : public Actor
{
public:
    Flame(StudentWorld* sWorld, double dx, double dy, Direction direction);
    virtual ~Flame() {}
    virtual void doSomething();
private:
    int m_pixelsTraveled;
};

// cannot be killed
class Pit : public Actor
{
public:
    Pit(StudentWorld* sWorld, double dx, double dy, Direction direction = 0);
    virtual ~Pit() {}
    virtual bool exhausted(); // return true if all bacteria emitted
    virtual void doSomething();
private:
    int m_nRSalmonella;
    int m_nASalmonella;
    int m_nEColi;
    std::set<int> bac;
};

// DirtPile, Food
// all (except food) can be damaged by socrates/spray/flames
class DamageableActor : public Actor
{
public:
    DamageableActor(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir, int depth);
    virtual ~DamageableActor() {}
    virtual bool isDamageable();
    virtual void changeHP(int change, int noiseDie = SOUND_NONE, int noiseHurt = SOUND_NONE); // will kill dirtpile & actors with lifetime immediately, play no noise
};

class DirtPile : public DamageableActor
{
public:
    DirtPile(StudentWorld* sWorld, double dx, double dy, Direction direction = 0, int depth = 1);
    virtual ~DirtPile() {}
    virtual bool blocksBacterialMovement(); // return true
};

class Food : public DamageableActor
{
public:
    Food(StudentWorld* sWorld, double dx, double dy, Direction direction = 90, int depth = 1);
    virtual ~Food() {}
    virtual bool edibleByBacteria(); // return true
    virtual bool isDamageable(); // false to prevent spray / fire from killing
};

// 3 goodies and fungus
// all have lifetime and ability to change score
class ActorsWithLifetime : public DamageableActor
{
public:
    ActorsWithLifetime(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir = 0, int depth = 1);
    virtual ~ActorsWithLifetime() {}
    int getLifetime(); // get lifetime of actor
    void decLifetime(); // decrease lifetime of actor
    void changeScore(int change); // change score of actor
private:
    int m_lifetime;
};

class RestoreHealthGoodie : public ActorsWithLifetime
{
public:
    RestoreHealthGoodie(StudentWorld* sWorld, double startX, double startY, int imageID = IID_RESTORE_HEALTH_GOODIE);
    virtual ~RestoreHealthGoodie() {}
    virtual void doSomething();
};

class FlamethrowerGoodie : public ActorsWithLifetime
{
public:
    FlamethrowerGoodie(StudentWorld* sWorld, double startX, double startY, int imageID = IID_FLAME_THROWER_GOODIE);
    virtual ~FlamethrowerGoodie() {}
    virtual void doSomething();
};

class ExtraLifeGoodie : public ActorsWithLifetime
{
public:
    ExtraLifeGoodie(StudentWorld* sWorld, double startX, double startY, int imageID = IID_EXTRA_LIFE_GOODIE);
    virtual ~ExtraLifeGoodie() {}
    virtual void doSomething();
};

class Fungus : public ActorsWithLifetime
{
public:
    Fungus(StudentWorld* sWorld, double startX, double startY, int imageID = IID_FUNGUS);
    virtual ~Fungus() {}
    virtual void doSomething();
};

// Socrates, Bacteria (3 types)
// all have HP
class ActorWithHP : public DamageableActor
{
public:
    ActorWithHP(StudentWorld* sWorld, int hp, int imageID, double startX, double startY, Direction dir = 0, int depth = 0);
    virtual ~ActorWithHP() {}
    virtual int getHP(); // get HP of actor
    // will change hp by change, instead of killing immediately
    // will play specified noise, based on calling object, only if object is hurt or dead (does not play noises if socrates gains health)
    virtual void changeHP(int change, int noiseDie, int noiseHurt);
private:
    int m_hp;
};

class Socrates : public ActorWithHP
{
public:
    Socrates(StudentWorld* sWorld, int hp = 100, int flameThrowerCharges = 5, int disinfectant = 20);
    virtual ~Socrates() {}
    virtual int getFlameCharges(); // get flamethrower charges
    virtual void addFlameThrowerCharges(int num); // add flamethrower charges
    virtual int getDisinfectant(); // get num of disinfectant
    // calls ActorWithHP change, but if change is 100, adjusts change to restore Soc to full health
    virtual void changeHP(int change, int noiseDie = SOUND_PLAYER_DIE, int noiseHurt = SOUND_PLAYER_HURT);
    virtual void doSomething();
private:
    Direction m_positionalAngle;
    int m_flameThrowerCharges;
    int m_disinfectant;
};

// regular and aggressive salmonella, ecoli
class Bacteria : public ActorWithHP
{
public:
    Bacteria(StudentWorld* sWorld, int hp, int imageID, double startX, double startY, Direction dir = 90, int depth = 0);
    virtual ~Bacteria() {}
    virtual bool emittedByPit(); // true for bacteria
    void eatFood(); // increment food eaten
    int getFoodEaten(); // return num of food eaten
    void resetFood(); // reset food eaten to 0
    void turnIntoFood(); // allow bacteria 50% chance of turning into food

    void completeMovementPlan(); // utilizes private functions to create movement plan
    bool validBacterialMovement(bool isAggressive = false, bool isEColi = false); // tests if forward movement is allowable
    void addNewBacteria(std::string bacteriaType); // add new bacteria to world
    
    // calls ActorWithHP changeHP, with noises specified by derived classes
    // bacteria type call this changeHP, but pass in own default parameters
    // appropriately deals with score and turning into food
    virtual void changeHP(int change, int noiseDie, int noiseHurt);
private:
    int m_foodEaten;
    int m_movementPlanDistance;
    
    // all movement plan functions encapsulated within completeMovementPlan()
    int getMovementPlan();
    void decrementMovementPlan();
    void resetMovementPlan();
    void carryOutMovementPlan();
    void createMovementPlanTowardsFood();
};

class RegularSalmonella : public Bacteria
{
public:
    RegularSalmonella(StudentWorld* sWorld, double startX, double startY, int imageID = IID_SALMONELLA, int hp = 4);
    virtual ~RegularSalmonella() {}
    virtual void changeHP(int change, int noiseDie = SOUND_SALMONELLA_DIE, int noiseHurt = SOUND_SALMONELLA_HURT);
    virtual void doSomething();
};

class AggressiveSalmonella : public Bacteria
{
public:
    AggressiveSalmonella(StudentWorld* sWorld, double startX, double startY, int imageID = IID_SALMONELLA, int hp = 10);
    virtual ~AggressiveSalmonella() {}
    virtual void changeHP(int change, int noiseDie = SOUND_SALMONELLA_DIE, int noiseHurt = SOUND_SALMONELLA_HURT);
    virtual void doSomething();
};

class EColi : public Bacteria
{
public:
    EColi(StudentWorld* sWorld, double startX, double startY, int imageID = IID_ECOLI, int hp = 5);
    virtual ~EColi() {}
    void EColiMovementPlan();
    virtual void changeHP(int change, int noiseDie = SOUND_ECOLI_DIE, int noiseHurt = SOUND_ECOLI_HURT);
    virtual void doSomething();
};

#endif // ACTOR_H_
