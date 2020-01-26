//
//  Player.hpp
//  proj1
//
//  Created by Alex Chen on 1/11/20.
//  Copyright © 2020 cs. All rights reserved.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Arena;

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    std::string dropPoisonVial();
    std::string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

#endif /* PLAYER_H */
