#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h" // for VIEW_HEIGHT, soundIDs, imageIDs
#include <cmath>


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir, int depth, double size)
: GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_alive = true;
    m_game = sWorld;
}

Actor::~Actor()
{
    delete m_game;
}

bool Actor::exhausted() const
{
    return false;
}

void Actor::doSomething()
{
}

void Actor::playSound(int soundID)
{
    m_game->playSound(soundID);
}

bool Actor::getLifeStatus() const
{
    return m_alive;
}

void Actor::setLifeStatus(bool status)
{
    m_alive = status;
}

StudentWorld* Actor::sWorldPointer()
{
    return m_game;
}

Socrates::Socrates(StudentWorld* sWorld, int flameThrowerCharges, int disinfectant, int hp)
: Actor(sWorld, IID_PLAYER, 0, VIEW_HEIGHT/2)
{
//    m_sWorld = sWorld;
    m_positionalAngle = 180;
//    m_totalLives = totalLives;
    m_flameThrowerCharges = flameThrowerCharges;
    m_disinfectant = disinfectant;
    m_hp = hp;
    m_replenish = true;
}


void Socrates::doSomething()
{
    if (m_hp <= 0)
    {
        setLifeStatus(false);
        playSound(SOUND_PLAYER_DIE);
        return;
    }
    int ch;
    if (sWorldPointer()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_SPACE:
            {
                if (m_disinfectant >= 1)
                {
                    double dx, dy;
                    getPositionInThisDirection(getDirection(), 2*SPRITE_RADIUS, dx, dy);
                    sWorldPointer()->addActor(new Disinfectant(sWorldPointer(), dx, dy, getDirection()));
                    m_disinfectant--;
                    playSound(SOUND_PLAYER_SPRAY);
                    m_replenish = false;
                }
                break;
            }
            case KEY_PRESS_ENTER:
            {
                for (int i=0; i<16; i++)
                {
                    double dx, dy;
                    getPositionInThisDirection(22*i, 2*SPRITE_RADIUS, dx, dy);
                    sWorldPointer()->addActor(new Flame(sWorldPointer(), dx, dy, 22*i));
                }
                m_flameThrowerCharges--;
                playSound(SOUND_PLAYER_FIRE);
                m_replenish = true;
                break;
            }
            case KEY_PRESS_LEFT:
            {
                m_positionalAngle += 5;
                const double PI = 4 * atan(1);
                double x = VIEW_RADIUS + VIEW_RADIUS*cos(m_positionalAngle/180*PI);
                double y = VIEW_RADIUS + VIEW_RADIUS*sin(m_positionalAngle/180*PI);
                moveTo(x, y);
                setDirection(m_positionalAngle + 180);
                m_replenish = true;
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                m_positionalAngle -= 5;
                const double PI = 4 * atan(1);
                double x = VIEW_RADIUS + VIEW_RADIUS*cos(m_positionalAngle/180*PI);
                double y = VIEW_RADIUS + VIEW_RADIUS*sin(m_positionalAngle/180*PI);
                moveTo(x, y);
                setDirection(m_positionalAngle + 180);
                m_replenish = true;
                break;
            }
            
        }
    }
    else
    {
        if (m_replenish && m_disinfectant < 20)
            m_disinfectant++;
        m_replenish = true;
    }
}

int Socrates::getHP()
{
    return m_hp;
}

void Socrates::setHP(int dif)
{
    m_hp += dif;
    if (dif < 0 && m_hp > 0)
        playSound(SOUND_PLAYER_HURT);
}

Disinfectant::Disinfectant(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth)
: Actor(sWorld, IID_SPRAY, dx, dy, direction, depth)
{
    
}

void Disinfectant::doSomething()
{
    
}

Flame::Flame(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth)
: Actor(sWorld, IID_FLAME, dx, dy, direction, depth)
{
    
}

void Flame::doSomething()
{
    
}

DirtPile::DirtPile(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth)
: Actor(sWorld, IID_DIRT, dx, dy, direction, depth)
{
}

Pit::Pit(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth)
: Actor(sWorld, IID_PIT, dx, dy, direction, depth)
{
    m_nRSalmonella = 5;
    m_nASalmonella = 3;
    m_nEColi = 2;
}

bool Pit::exhausted()
{
    if (m_nASalmonella == 0 && m_nRSalmonella == 0 && m_nEColi == 0)
        return true;
    return false;
}

Food::Food(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth)
: Actor(sWorld, IID_FOOD, dx, dy, direction, depth)
{
    
}
