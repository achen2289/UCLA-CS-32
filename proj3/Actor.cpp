#include "Actor.h"
#include "StudentWorld.h" // sWorldPointer()
#include "GameConstants.h" // for size constants, soundIDs and imageIDs
#include <cmath> // trig and power functions
#include <set> // for pit set
#include <string> // for string
using namespace std;

Actor::Actor(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir, int depth, double size)
: GraphObject(imageID, startX, startY, dir, depth, size)
{
    m_alive = true;
    m_game = sWorld;
}

int Actor::getHP()
{
    return 0;
}

void Actor::changeHP(int change, int noiseDie, int noiseHurt)
{
    return;
}

bool Actor::getLifeStatus() const
{
    return m_alive;
}

void Actor::setLifeStatus(bool status)
{
    m_alive = status;
}

bool Actor::isDamageable()
{
    return false;
}

bool Actor::emittedByPit()
{
    return false;
}

bool Actor::exhausted() const
{
    return false;
}

bool Actor::edibleByBacteria()
{
    return false;
}

bool Actor::blocksBacterialMovement()
{
    return false;
}

int Actor::getDisinfectant()
{
    return 0;
}

int Actor::getFlameCharges()
{
    return 0;
}

void Actor::addFlameThrowerCharges(int num)
{
    return;
}

void Actor::playSound(int soundID)
{
    m_game->playSound(soundID);
}

void Actor::doSomething()
{
    return;
}

StudentWorld* Actor::sWorldPointer()
{
    return m_game;
}

Disinfectant::Disinfectant(StudentWorld* sWorld, double dx, double dy, Direction direction)
: Actor(sWorld, IID_SPRAY, dx, dy, direction)
{
    m_pixelsTraveled = 0;
}

void Disinfectant::doSomething()
{
    if (!getLifeStatus())
        return;
    
    // check for overlap with anything it can kill
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, -2, false, false, false, false))
    {
        setLifeStatus(false);
        return;
    }
    
    // moves SPRITE_WIDTH pixels at a time until reaching 112 pixels
    moveAngle(getDirection(), SPRITE_WIDTH);
    m_pixelsTraveled += SPRITE_WIDTH;
    if (m_pixelsTraveled >= 112)
    {
        setLifeStatus(false);
    }
    return;
}

Flame::Flame(StudentWorld* sWorld, double dx, double dy, Direction direction)
: Actor(sWorld, IID_FLAME, dx, dy, direction)
{
    m_pixelsTraveled = 0;
}

void Flame::doSomething()
{
    if (!getLifeStatus())
        return;
    
    // checks for overlap with anything that can be killed by it
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, -5, false, false, false, false))
    {
        setLifeStatus(false);
        return;
    }
    
    // radiates in a circle around socrates until ovelap or traveling 32 pixels
    else
    {
        moveAngle(getDirection(), SPRITE_WIDTH);
        m_pixelsTraveled += SPRITE_WIDTH;
        if (m_pixelsTraveled >= 32)
        {
            setLifeStatus(false);
        }
        return;
    }
}

Pit::Pit(StudentWorld* sWorld, double dx, double dy, Direction direction)
: Actor(sWorld, IID_PIT, dx, dy, direction)
{
    m_nRSalmonella = 5;
    m_nASalmonella = 3;
    m_nEColi = 2;
    bac.insert(1); // regular salmonella
    bac.insert(2); // aggressive salmonella
    bac.insert(3); // ecoli
}

// checks if pit has finished emission
bool Pit::exhausted()
{
    if (m_nASalmonella == 0 && m_nRSalmonella == 0 && m_nEColi == 0)
    {
        setLifeStatus(false);
        return true;
    }
    return false;
}

void Pit::doSomething()
{
    if (exhausted())
    {
        setLifeStatus(false);
        return;
    }
    if (m_nRSalmonella == 0)
        bac.erase(1);
    if (m_nASalmonella == 0)
        bac.erase(2);
    if (m_nEColi == 0)
        bac.erase(3);
    
    if (randInt(1, 50) == 1)
    {
        // uses set to randomly select one of remaining types of bacteria to create
        int random = randInt(0, (int)bac.size()-1);
        set<int>::iterator itr = bac.begin();
        for (int i=0; i<random; i++)
            itr++;
        switch (*itr)
        {
            case 1:
            {
                sWorldPointer()->addActor(new RegularSalmonella(sWorldPointer(), getX(), getY()));
                m_nRSalmonella--;
                break;
            }
            case 2:
            {
                sWorldPointer()->addActor(new AggressiveSalmonella(sWorldPointer(), getX(), getY()));
                m_nASalmonella--;
                break;
            }
            case 3:
            {
                sWorldPointer()->addActor(new EColi(sWorldPointer(), getX(), getY()));
                m_nEColi--;
                break;
            }
        }
        playSound(SOUND_BACTERIUM_BORN);
    }
}

DamageableActor::DamageableActor(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir, int depth)
: Actor(sWorld, imageID, startX, startY, dir, depth)
{
}

bool DamageableActor::isDamageable()
{
    return true;
}

void DamageableActor::changeHP(int change, int noiseDie, int noiseHurt)
{
    setLifeStatus(false);
    playSound(noiseDie);
}

DirtPile::DirtPile(StudentWorld* sWorld, double dx, double dy, Direction dir, int depth)
: DamageableActor(sWorld, IID_DIRT, dx, dy, dir, depth)
{
}

bool DirtPile::blocksBacterialMovement()
{
    return true;
}

Food::Food(StudentWorld* sWorld, double dx, double dy, Direction direction, int depth)
: DamageableActor(sWorld, IID_FOOD, dx, dy, direction, depth)
{
}

bool Food::edibleByBacteria()
{
    return true;
}

bool Food::isDamageable()
{
    return false;
}

ActorsWithLifetime::ActorsWithLifetime(StudentWorld* sWorld, int imageID, double startX, double startY, Direction dir, int depth)
: DamageableActor(sWorld, imageID, startX, startY, dir, depth)
{
    m_lifetime = max(randInt(0, 300 - 10*sWorldPointer()->getLevel() - 1), 50);
}

int ActorsWithLifetime::getLifetime()
{
    return m_lifetime;
}

void ActorsWithLifetime::decLifetime()
{
    m_lifetime--;
}

void ActorsWithLifetime::changeScore(int change)
{
    sWorldPointer()->increaseScore(change);
}

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* sWorld, double startX, double startY, int imageID)
: ActorsWithLifetime(sWorld, imageID, startX, startY)
{
}

void RestoreHealthGoodie::doSomething()
{
    if (!getLifeStatus())
        return;
    decLifetime();
    
    // check for overlap with socrates
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, 100, true, false, false, false))
    {
        // increase score
        changeScore(250);
        setLifeStatus(false);
        playSound(SOUND_GOT_GOODIE);
    }
    
    // check if lifetime has elapsed
    if (getLifetime() == 0)
        setLifeStatus(false);
}

FlamethrowerGoodie::FlamethrowerGoodie(StudentWorld* sWorld, double startX, double startY, int imageID)
: ActorsWithLifetime(sWorld, imageID, startX, startY)
{
}

void FlamethrowerGoodie::doSomething()
{
    if (!getLifeStatus())
        return;
    decLifetime();
    
    // check for overlap with socrates
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, 0, true, false, false, true))
    {
        // increase score
        changeScore(300);
        setLifeStatus(false);
        playSound(SOUND_GOT_GOODIE);
    }
    
    // check if lifetime has elapsed
    if (getLifetime() == 0)
        setLifeStatus(false);
}

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* sWorld, double startX, double startY, int imageID)
: ActorsWithLifetime(sWorld, imageID, startX, startY)
{
}

void ExtraLifeGoodie::doSomething()
{
    if (!getLifeStatus())
        return;
    decLifetime();
    
    // check for overlap with socrates
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, 0, true, false, false, false))
    {
        // increse score, increase lives
        changeScore(500);
        setLifeStatus(false);
        playSound(SOUND_GOT_GOODIE);
        sWorldPointer()->incLives();
    }
    
    // check if lifetime has elapsed
    if (getLifetime() == 0)
        setLifeStatus(false);
}

Fungus::Fungus(StudentWorld* sWorld, double startX, double startY, int imageID)
: ActorsWithLifetime(sWorld, imageID, startX, startY)
{
}

void Fungus::doSomething()
{
    if (!getLifeStatus())
        return;
    decLifetime();
    
    // check if overlapping with socrates
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, -20, true, false, false, false))
    {
        // decrease score
        changeScore(-50);
        setLifeStatus(false);
    }
    
    // check if lifetime has elapsed
    if (getLifetime() == 0)
        setLifeStatus(false);
}

ActorWithHP::ActorWithHP(StudentWorld* sWorld, int hp, int imageID, double startX, double startY, Direction dir, int depth)
: DamageableActor(sWorld, imageID, startX, startY, dir, depth)
{
    m_hp = hp;
}

int ActorWithHP::getHP()
{
    return m_hp;
}

void ActorWithHP::changeHP(int change, int noiseDie, int noiseHurt)
{
    m_hp += change;
    
    // if change is negative, sounds may be played
    if (change < 0)
    {
        if (getHP() > 0)
        {
            playSound(noiseHurt);
            return;
        }
        playSound(noiseDie);
        setLifeStatus(false);
    }
}

Socrates::Socrates(StudentWorld* sWorld, int hp, int flameThrowerCharges, int disinfectant)
: ActorWithHP(sWorld, hp, IID_PLAYER, 0, VIEW_HEIGHT/2)
{
    m_positionalAngle = 180;
    m_flameThrowerCharges = flameThrowerCharges;
    m_disinfectant = disinfectant;
}

int Socrates::getFlameCharges()
{
    return m_flameThrowerCharges;
}

void Socrates::addFlameThrowerCharges(int num)
{
    m_flameThrowerCharges += num;
}

int Socrates::getDisinfectant()
{
    return m_disinfectant;
}

void Socrates::changeHP(int change, int noiseDie, int noiseHurt)
{
    // if change is equal to 100, then restore socrates to full life
    int actualChange = change;
    if (change == 100)
        actualChange = 100 - getHP();
    ActorWithHP::changeHP(actualChange, noiseDie, noiseHurt);
}

void Socrates::doSomething()
{
    if (!getLifeStatus())
    {
        return;
    }
    
    // deals with live user input
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
                    getPositionInThisDirection(getDirection(), SPRITE_WIDTH, dx, dy);
                    sWorldPointer()->addActor(new Disinfectant(sWorldPointer(), dx, dy, getDirection()));
                    m_disinfectant--;
                    playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            }
            case KEY_PRESS_ENTER:
            {
                if (m_flameThrowerCharges > 0)
                {
                    for (int i=0; i<16; i++)
                    {
                        double dx, dy;
                        getPositionInThisDirection(22*i, SPRITE_WIDTH, dx, dy);
                        sWorldPointer()->addActor(new Flame(sWorldPointer(), dx, dy, 22*i));
                    }
                    m_flameThrowerCharges--;
                    playSound(SOUND_PLAYER_FIRE);
                }
                break;
            }
            case KEY_PRESS_LEFT:
            {
                m_positionalAngle += 5;
                const double PI = 4 * atan(1);
                double x = VIEW_RADIUS + VIEW_RADIUS*cos(m_positionalAngle*1.0/180*PI);
                double y = VIEW_RADIUS + VIEW_RADIUS*sin(m_positionalAngle*1.0/180*PI);
                moveTo(x, y);
                setDirection(m_positionalAngle + 180);
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                m_positionalAngle -= 5;
                const double PI = 4 * atan(1);
                double x = VIEW_RADIUS + VIEW_RADIUS*cos(m_positionalAngle*1.0/180*PI);
                double y = VIEW_RADIUS + VIEW_RADIUS*sin(m_positionalAngle*1.0/180*PI);
                moveTo(x, y);
                setDirection(m_positionalAngle + 180);
                break;
            }
        }
    }
    
    // if no key is pressed, increment disinfectant num
    else
    {
        if (m_disinfectant < 20)
            m_disinfectant++;
    }
}

Bacteria::Bacteria(StudentWorld* sWorld, int hp, int imageID, double startX, double startY, Direction dir, int depth)
: ActorWithHP(sWorld, hp, imageID, startX, startY, dir, depth)
{
    m_movementPlanDistance = 0;
    m_foodEaten = 0;
}

bool Bacteria::emittedByPit()
{
    return true;
}

void Bacteria::eatFood()
{
    m_foodEaten++;
}

int Bacteria::getFoodEaten()
{
    return m_foodEaten;
}

void Bacteria::resetFood()
{
    m_foodEaten = 0;
}

void Bacteria::turnIntoFood()
{
    // 50% chance of bacteria turning into food
    if (randInt(0,1) == 0)
    {
        sWorldPointer()->addActor(new Food(sWorldPointer(), getX(), getY()));
    }
}

// calls private member functions
void Bacteria::completeMovementPlan()
{
    if (getMovementPlan() > 0)
    {
        carryOutMovementPlan();
        return;
    }
    createMovementPlanTowardsFood();
    return;
}

bool Bacteria::validBacterialMovement(bool isAggressive, bool isEColi)
{
    double dx, dy;
    if (isEColi)
    {
        for (int i=1; i<=2; i++)
        {
            // checks two pixels in front of bacteria for valid movement
            getPositionInThisDirection(getDirection(), i, dx, dy);
            if (sWorldPointer()->checkForOverlap(dx, dy, SPRITE_WIDTH/2, 0, false, true, false, false))
            {
                return false;
            }
            if (sWorldPointer()->getDistanceBetween(dx, VIEW_RADIUS, dy, VIEW_RADIUS) >= VIEW_RADIUS)
            {
                return false;
            }
        }
        return true;
    }
    for (int i=1; i<=3; i++)
    {
        // checks three pixels in front of bacteria for valid movement
        getPositionInThisDirection(getDirection(), i, dx, dy);
        if (sWorldPointer()->checkForOverlap(dx, dy, SPRITE_WIDTH/2, 0, false, true, false, false))
        {
            if (isAggressive)
                return false;
            setDirection(randInt(0, 359));
            resetMovementPlan();
            return false;
        }
        if (sWorldPointer()->getDistanceBetween(dx, VIEW_RADIUS, dy, VIEW_RADIUS) >= VIEW_RADIUS)
        {
            if (isAggressive)
                return false;
            setDirection(randInt(0, 359));
            resetMovementPlan();
            return false;
        }
    }
    return true;
}

void Bacteria::addNewBacteria(std::string bacteriaType)
{
    // adds new bacteria to location based on current location
    double newX = getX();
    if (newX < VIEW_WIDTH/2)
        newX += SPRITE_WIDTH/2;
    else if (newX > VIEW_WIDTH/2)
        newX -= SPRITE_WIDTH/2;
    double newY = getY();
    if (newY < VIEW_HEIGHT/2)
        newY += SPRITE_WIDTH/2;
    else if (newY > VIEW_HEIGHT/2)
        newY -= SPRITE_WIDTH/2;
    if (bacteriaType == "regular salmonella")
    {
        sWorldPointer()->addActor(new RegularSalmonella(sWorldPointer(), newX, newY));
    }
    else if (bacteriaType == "aggressive salmonella")
    {
        sWorldPointer()->addActor(new AggressiveSalmonella(sWorldPointer(), newX, newY));
    }
    else if (bacteriaType == "ecoli")
    {
        sWorldPointer()->addActor(new EColi(sWorldPointer(), newX, newY));
    }
    
    // increment num of bacteria added into world
    sWorldPointer()->incrementAddedBac();
}

void Bacteria::changeHP(int change, int noiseDie, int noiseHurt)
{
    ActorWithHP::changeHP(change, noiseDie, noiseHurt);
    
    // if bacteria dies after HP change, increase score
    if (!getLifeStatus())
    {
        sWorldPointer()->increaseScore(100);
        turnIntoFood();
    }
}

// PRIVATE BACTERIA FUNCTIONS

int Bacteria::getMovementPlan()
{
    return m_movementPlanDistance;
}

void Bacteria::decrementMovementPlan()
{
    m_movementPlanDistance--;
}

void Bacteria::resetMovementPlan()
{
    m_movementPlanDistance = 10;
}

void Bacteria::carryOutMovementPlan()
{
    decrementMovementPlan();
    if (!validBacterialMovement())
    {
        setDirection(randInt(0, 359));
        resetMovementPlan();
        return;
    }
    moveAngle(getDirection(), 3);
    return;
}

void Bacteria::createMovementPlanTowardsFood()
{
    Direction dirFood = sWorldPointer()->getClosestFood(getX(), getY());
    if (dirFood == -1)
    {
        setDirection(randInt(0, 359));
        resetMovementPlan();
        return;
    }
    setDirection(dirFood);
    
    // SHOULD BE INCLUDED, B/C MOVEMENTPLAN WILL STILL BE 0 FOR EVERY ITERATION??
    if (!validBacterialMovement())
    {
        setDirection(randInt(0, 359));
        resetMovementPlan();
        return;
    }
    moveAngle(getDirection(), 3);
}

RegularSalmonella::RegularSalmonella(StudentWorld* sWorld, double startX, double startY, int imageID, int hp)
: Bacteria(sWorld, hp, imageID, startX, startY)
{
}

void RegularSalmonella::changeHP(int change, int noiseDie, int noiseHurt)
{
    Bacteria::changeHP(change, noiseDie, noiseHurt);
}

void RegularSalmonella::doSomething()
{
    if (!getLifeStatus())
        return;
    
    // checks for overlap with socrates
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, -1, true, false, false, false))
    {
        completeMovementPlan();
        return;
    }
    
    // divide
    if (getFoodEaten() == 3)
    {
        resetFood();
        addNewBacteria("regular salmonella");
        completeMovementPlan();
        return;
    }
    
    // check for overlap with food
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, 0, false, false, true, false))
    {
        eatFood();
    }
    completeMovementPlan();
    return;
}

AggressiveSalmonella::AggressiveSalmonella(StudentWorld* sWorld, double startX, double startY, int imageID, int hp)
: Bacteria(sWorld, hp, imageID, startX, startY)
{
}

void AggressiveSalmonella::changeHP(int change, int noiseDie, int noiseHurt)
{
    Bacteria::changeHP(change, noiseDie, noiseHurt);
}

void AggressiveSalmonella::doSomething()
{
    if (!getLifeStatus())
        return;
    
    // attempts to chase socrates if located within 72 pixels
    double socX, socY;
    if (sWorldPointer()->distanceFromSocrates(getX(), getY(), socX, socY) <= 72)
    {
        const double PI = 4 * atan(1);
        setDirection(atan2(socY-getY(), socX-getX())*1.0*180/PI);
        if (validBacterialMovement(true))
            moveAngle(getDirection(), 3);
        
        // checks for overlap with socrates on the way to socrates
        if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, -2, true, false, false, false))
            return;
        
        // checks if needs to divide on the way to socrates
        if (getFoodEaten() == 3)
        {
            resetFood();
            addNewBacteria("aggressive salmonella");
            return;
        }
        
        // checks if overlap with food on the way
        if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, 0, false, false, true, false))
        {
            eatFood();
        }
        return;
    }
    
    // checks for overlap with socrates
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, -2, true, false, false, false))
    {
        completeMovementPlan();
        return;
    }
    
    // checks if necessary to divide
    if (getFoodEaten() == 3)
    {
        resetFood();
        addNewBacteria("aggressive salmonella");
        completeMovementPlan();
        return;
    }
    
    // checks if overlap with food
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, 0, false, false, true, false))
    {
        eatFood();
    }
    
    // if socrates is not being chased after, then go for closest food
    // if blocked, then randomize direction
    completeMovementPlan();
}

EColi::EColi(StudentWorld* sWorld, double startX, double startY, int imageID, int hp)
: Bacteria(sWorld, hp, imageID, startX, startY)
{
}

void EColi::EColiMovementPlan()
{
    // chases socrates
    double socX, socY;
    if (sWorldPointer()->distanceFromSocrates(getX(), getY(), socX, socY) <= VIEW_WIDTH)
    {
        const double PI = 4 * atan(1);
        setDirection(atan2(socY-getY(), socX-getX())*1.0*180/PI);
        
        // if movement is blocked, then rotate 10 times max until not blocked
        for (int i=0; i<10; i++)
        {
            if (validBacterialMovement(false, true))
            {
                moveAngle(getDirection(), 2);
                return;
            }
            setDirection(getDirection()+10);
        }
    }
    return;
}

void EColi::changeHP(int change, int noiseDie, int noiseHurt)
{
    Bacteria::changeHP(change, noiseDie, noiseHurt);
}

void EColi::doSomething()
{
    if (!getLifeStatus())
        return;
    
    // checks for overlap with socrates
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, -4, true, false, false, false))
    {
        // if so, continue to chase socrates
        EColiMovementPlan();
        return;
    }
    
    // tries to divide
    if (getFoodEaten() == 3)
    {
        resetFood();
        addNewBacteria("ecoli");
        
        // continue to chase socrates
        EColiMovementPlan();
        return;
    }
    
    // checks if overlapping with food
    if (sWorldPointer()->checkForOverlap(getX(), getY(), SPRITE_WIDTH, 0, false, false, true, false))
    {
        eatFood();
    }
    
    // chase socrates
    EColiMovementPlan();
}
