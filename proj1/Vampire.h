//
//  Vampire.h
//  proj1
//
//  Created by Alex Chen on 1/11/20.
//  Copyright © 2020 cs. All rights reserved.
//

#ifndef VAMPIRE_H
#define VAMPIRE_H

class Arena; // for Arena*

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* VAMPIRE_H */
