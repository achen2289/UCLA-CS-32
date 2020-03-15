//
//  main.cpp
//  hw3
//
//  Created by Alex Chen on 2/6/20.
//  Copyright © 2020 cs. All rights reserved.
//

//#include <iostream>
//#include <string>
//using namespace std;

class Event
{
public:
    Event(string name);
    virtual ~Event();
    string name() const;
    virtual string need() const = 0;
    virtual bool isSport() const;
private:
    string m_name;
};

class BasketballGame: public Event
{
public:
    BasketballGame(string eventName);
    virtual ~BasketballGame();
    virtual string need() const;
};

class Concert: public Event
{
public:
    Concert(string eventName, string genre);
    virtual ~Concert();
    virtual string need() const;
    virtual bool isSport() const;
private:
    string m_genre;
};

class HockeyGame: public Event
{
public:
    HockeyGame(string eventName);
    virtual ~HockeyGame();
    virtual string need() const;
};



Event::Event(string name)
{
    m_name = name;
}

Event::~Event()
{
}

string Event::name() const
{
    return m_name;
}

bool Event::isSport() const
{
    return true;
}



BasketballGame::BasketballGame(string eventName)
    :Event(eventName)
{
}

BasketballGame::~BasketballGame()
{
    cout << "Destroying the " << name() << " basketball game" << endl;
}

string BasketballGame::need() const
{
    return "hoops";
}



Concert::Concert(string eventName, string genre)
    :Event(eventName)
{
    m_genre = genre;
}

Concert::~Concert()
{
    cout << "Destroying the " << name() << " " << m_genre << " concert" << endl;
}

string Concert::need() const
{
    return "a stage";
}

bool Concert::isSport() const
{
    return false;
}



HockeyGame::HockeyGame(string eventName)
    :Event(eventName)
{
}

HockeyGame::~HockeyGame()
{
    cout << "Destroying the " << name() << " hockey game" << endl;
}

string HockeyGame::need() const
{
    return "ice";
}



//void display(const Event* e)
//{
//    cout << e->name() << ": ";
//    if (e->isSport())
//    cout << "(sport) ";
//    cout << "needs " << e->need() << endl;
//}

//int main()
//{
//    Event* events[4];
//    events[0] = new BasketballGame("Lakers vs. Suns");
//      // Concerts have a name and a genre.
//    events[1] = new Concert("Banda MS", "banda");
//    events[2] = new Concert("KISS", "hard rock");
//    events[3] = new HockeyGame("Kings vs. Flames");
//
//    cout << "Here are the events." << endl;
//    for (int k = 0; k < 4; k++)
//    display(events[k]);
//
//      // Clean up the events before exiting
//    cout << "Cleaning up." << endl;
//    for (int k = 0; k < 4; k++)
//    delete events[k];
//}
