//
//  History.h
//  proj1
//
//  Created by Alex Chen on 1/12/20.
//  Copyright © 2020 cs. All rights reserved.
//

#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h" // for MAXROWS, MAXCOLS

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    int m_rows;
    int m_cols;
    char displayGrid[MAXROWS][MAXCOLS];
};

#endif /* HISTORY_H */
