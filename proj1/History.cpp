//
//  History.cpp
//  proj1
//
//  Created by Alex Chen on 1/12/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int i=1; i<=m_rows; i++)
    {
        for (int j=1; j<=m_cols; j++)
        {
            displayGrid[i-1][j-1] = '.';
        }
    }
}

bool History::record(int r, int c)
{
    if (r > m_rows || r < 1 || c > m_cols || c < 1)
    {
        return false;
    }
    else
    {
        if (displayGrid[r-1][c-1] == '.')
        {
            displayGrid[r-1][c-1] = 'A';
        }
        else if (displayGrid[r-1][c-1] < 'Z')
        {
            displayGrid[r-1][c-1]++;
        }
        return true;
    }
}

void History::display() const
{
    clearScreen();
    for (int i=1; i<=m_rows; i++)
    {
        for (int j=1; j<=m_cols; j++)
        {
            cout << displayGrid[i-1][j-1];
        }
        cout << endl;
    }
    cout << endl;
}
