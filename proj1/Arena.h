//
//  Arena.h
//  proj1
//
//  Created by Alex Chen on 1/11/20.
//  Copyright © 2020 cs. All rights reserved.
//

#ifndef ARENA_H
#define ARENA_H

#include "globals.h" // for MAXROWS, MAXCOLS, MAXVAMPIRES
#include "History.h" // for m_history
#include <string>

class Player; // for Player*
class Vampire; // for Vampire*

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     vampireCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfVampiresAt(int r, int c) const;
    void    display(std::string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();
    
    History& history();

  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;

      // Helper functions
    void checkPos(int r, int c, std::string functionName) const;
    bool isPosInBounds(int r, int c) const;
    
    History m_history;
};

#endif /* ARENA_H */
