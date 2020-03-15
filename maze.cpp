//
//  maze.cpp
//  hw3
//
//  Created by Alex Chen on 2/8/20.
//  Copyright © 2020 cs. All rights reserved.
//

//#include <iostream>
//#include <string>
//using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (er < 0 || er >= nRows || ec < 0 || ec >= nCols) // ensure ending rows and columns are valid
        return false;
    if (sr < 0 || sr >= nRows || sc < 0 || sc >= nCols) // ensure starting rows and columns are valid
        return false;
    if (maze[sr][sc] == 'X' || maze[er][ec] == 'X') // ensure starting points and ending points are not walls
        return false;
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '#';
    if (sc+1 < nCols && maze[sr][sc+1] == '.') // east
    {
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
            return true;
    }
    if (sr+1 < nRows && maze[sr+1][sc] == '.') // south
    {
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
            return true;
    }
    if (sc-1 >= 0 && maze[sr][sc-1] == '.') // west
    {
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
            return true;
    }
    if (sr-1 >= 0 && maze[sr-1][sc] == '.') // north
    {
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
            return true;
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

