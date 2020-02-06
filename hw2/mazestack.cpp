//
//  main.cpp
//  hw2
//
//  Created by Alex Chen on 2/2/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include <iostream>
#include <string>
#include <stack>
using namespace std;

class Coord
{
    public:
      Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
      int r() const { return m_r; }
      int c() const { return m_c; }
    private:
      int m_r;
      int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr < 0 || sr >= nRows || sc < 0 || sc >= nCols ||
        er < 0 || er >= nRows || ec < 0 || ec >= nCols) // check for valid starting and ending positions
        return false;
    if (maze[sr][sc] == 'X' || maze[er][ec] == 'X') // check that starting and ending positions are not walls
        return false;

    stack<Coord> coordinate; // stack of Coord objects to store coordinates

    // deal with initial starting position before entering loop
    int curR = sr;
    int curC = sc;
    coordinate.push(Coord(curR, curC));
    maze[curR][curC] = 'X';

    // depth-first search
    while (!coordinate.empty())
    {
        // take row and column of top of stack
        curR = coordinate.top().r();
        curC = coordinate.top().c();
        coordinate.pop(); // remove coordinate from stack

        // return true if current coordinate is equal to target coordinate
        if (curR == er && curC == ec)
            return true;

        // CHECK BOXES AROUND CURRENT, PUSHING IF BOX IS VALID POSITION

        // push right coordinate if possible
        if (curC + 1 < nCols && maze[curR][curC + 1] != 'X')
        {
            maze[curR][curC + 1] = 'X';
            coordinate.push(Coord(curR, curC + 1));
        }

        // push bottom coordinate if possible
        if (curR + 1 < nRows && maze[curR + 1][curC] != 'X')
        {
            maze[curR + 1][curC] = 'X';
            coordinate.push(Coord(curR + 1, curC));
        }

        // push left coordinate if possible
        if (curC - 1 >= 0 && maze[curR][curC - 1] != 'X')
        {
            maze[curR][curC - 1] = 'X';
            coordinate.push(Coord(curR, curC - 1));
        }

        // push top coordinate if possible
        if (curR - 1 >= 0 && maze[curR - 1][curC] != 'X')
        {
            maze[curR - 1][curC] = 'X';
            coordinate.push(Coord(curR - 1, curC));
        }
    }
    return false;
}

//int main()
//{
//    string maze[10] = {
//        "XXXXXXXXXX",
//        "X...X..X.X",
//        "X.XXX....X",
//        "X.X.XXXX.X",
//        "XXX......X",
//        "X...X.XX.X",
//        "X.X.X..X.X",
//        "X.XXXX.X.X",
//        "X..X...X.X",
//        "XXXXXXXXXX"
//    };
//
//    if (pathExists(maze, 10,10, 4,3, 1,8))
//        cout << "Solvable!" << endl;
//    else
//        cout << "Out of luck!" << endl;
//}



