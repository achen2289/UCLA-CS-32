//
//  Game.hpp
//  proj1
//
//  Created by Alex Chen on 1/11/20.
//  Copyright © 2020 cs. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include <string>

class Arena; // for Arena*

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};

#endif /* GAME_H */
