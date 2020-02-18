#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0);
    virtual ~Actor();
    virtual bool exhausted() const;
    virtual bool isDirtPile();
    virtual void doSomething();
    virtual void playSound(int soundID);
    bool getLifeStatus() const;
    void setLifeStatus(bool status);
    StudentWorld* sWorldPointer();
private:
    bool m_alive;
    StudentWorld* m_game;
};

class Socrates : public Actor
{
public:
    Socrates(StudentWorld* sWorld, int flameThrowerCharges = 5, int disinfectant = 20, int hp = 100);
    virtual void doSomething();
    int getHP();
    void setHP(int dif);
private:
    Direction m_positionalAngle;
//    int m_totalLives;
    int m_flameThrowerCharges;
    int m_disinfectant;
    int m_hp;
//    GameWorld* m_sWorld;
    bool m_replenish;
};

class Disinfectant : public Actor
{
public:
    Disinfectant(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth = 1);
    virtual void doSomething();
private:
    
};

class Flame : public Actor
{
public:
    Flame(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth = 1);
    virtual void doSomething();
private:
    
};

class DirtPile : public Actor
{
public:
    DirtPile(StudentWorld* sWorld, double dx, double dy, Direction direction = 0, int depth = 1);
    virtual bool isDirtPile();
private:
    
};

class Pit : public Actor
{
public:
    Pit(StudentWorld* sWorld, double dx, double dy, Direction direction = 0, int depth = 1);
    virtual bool exhausted();
private:
    int m_nRSalmonella;
    int m_nASalmonella;
    int m_nEColi;
};

class Food : public Actor
{
public:
    Food(StudentWorld* sWorld, double dx, double dy, Direction direction = 90, int depth = 1);
private:
};

#endif // ACTOR_H_
