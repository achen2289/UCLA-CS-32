//
//  eval.cpp
//  hw2
//
//  Created by Alex Chen on 2/2/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    postfix = ""; // initial postfix string
    stack<char> operators; // stack to store operators
    
    string infix2 = ""; // create infix string with no spaces
    for (int i=0; i<infix.length(); i++)
    {
        if (infix[i] != ' ')
            infix2 += infix[i];
    }
    
    //infix to postfix
    for (int i=0; i<infix2.length(); i++)
    {
        bool status = true;
        switch (infix2[i])
        {
            case '(': // push open parenthesis
            {
                if (i+1 == infix2.size()) // cannot end with an open parenthesis
                    return 1;
                if (i != 0 && isalpha(infix2[i-1])) // open parenthesis cannot be preceded by an alphabetic char
                    return 1;
                operators.push(infix2[i]);
                break;
            }
            case ')': // add all operators to expression until open parenthesis is reached
            {
                if (i == 0 || infix2[i-1] == '*' || infix2[i-1] == '/' || infix2[i-1] == '+' || infix2[i-1] == '-' || infix2[i-1] == '(') // first operator being closed parenthesis, or closed parenthesis following non-alphabet character except for itself not allowed
                    return 1;
                if (operators.empty()) // if operators empty, no operators present in infix before closed parenthesis
                    return 1;
                while (operators.top() != '(')
                {
                    postfix += operators.top();
                    operators.pop();
                    if (operators.empty()) // will be true if operators does not contain open parenthesis
                        return 1;
                }
                operators.pop(); // must be the corresponding opening parenthesis
                break;
            }
            case '*':
            case '/':
                if (!operators.empty() && (operators.top() == '+' || operators.top() == '-')) // to determine precendence relationship between current infix operator and top operator on stack
                    status = false;
            case '+':
            case '-':
            {
                if (i == 0) // operator cannot be first of infix
                    return 1;
                if (infix2[i-1] == '*' || infix2[i-1] == '+' || infix2[i-1] == '-' || infix2[i-1] == '/' || infix2[i-1] == '(') // operator cannot be preceded by an operator or open parenthesis
                    return 1;
                if (i+1 == infix2.size())
                    return 1;
                while (!operators.empty() && operators.top() != '(' && status) // if precedence of the operator is less than or equal to precendence of top operator, add top operator to postfix, then push current operator
                {
                    postfix += operators.top();
                    operators.pop();
                }
                operators.push(infix2[i]);
                break;
            }
            default:
            {
                if (!isalpha(infix2[i]) || isupper(infix2[i])) // check for alphabet character and lower case
                    return 1;
                if (i != 0)
                {
                    if (infix2[i-1] == ')' || isalpha(infix2[i-1])) // alphabetic char cannot be preceded by a closed parenthesis or alphabetic char
                        return 1;
                }
                postfix += infix2[i];
                break;
            }
        }
    }
    while (!operators.empty()) // deal with last operators
    {
        if (operators.top() == '(') // means there was more open parentheses than close parentheses
            return 1;
        postfix += operators.top();
        operators.pop();
    }
    
    stack<int> operands; // stack to store operands
    
    // evaluate postfix
    for (int i=0; i<postfix.length(); i++)
    {
        // if postfix char is alphabetic (already checked for lower case)
        if (isalpha(postfix[i]))
        {
            if (!values.contains(postfix[i])) // if map does not contain char key, return 2
                return 2;
            int c;
            values.get(postfix[i], c);
            operands.push(c); // otherwise push operand to stack after converting operand to int
        }
        
        // postfix char must be an operator
        // use top two operands in stack, joining them with the operator
        else
        {
            if (operands.empty()) // meaning operator appears before operand
                return 1;
            int a = operands.top();
            operands.pop();
            if (operands.empty()) // meaning only one operand is present for one operator
                return 1;
            int b = operands.top();
            operands.pop();
            
            // push back result of two operands
            switch (postfix[i])
            {
                case '+':
                    operands.push(b + a);
                    break;
                case '-':
                    operands.push(b - a);
                    break;
                case '*':
                    operands.push(b * a);
                    break;
                case '/':
                    if (a == 0)
                        return 3;
                    operands.push(b / a);
                    break;
                default: // if operator is not one of previous four, there is a syntax error
                    return 1;
            }
        }
    }
    if (operands.empty() || operands.size() > 1) // meaning postfix contained no operands or infix was empty
        return 1;
    result = operands.top();
    return 0;
}

//int main()
//{
//    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
//    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
//    Map m;
//    for (int k = 0; vars[k] != '#'; k++)
//        m.insert(vars[k], vals[k]);
//    string pf;
//    int answer;
//    assert(evaluate("a+ e", m, pf, answer) == 0  &&
//                            pf == "ae+"  &&  answer == -6);
//    answer = 999;
//    assert(evaluate("(a+b)c", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("(a+b)(", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a(b+c)", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a+b)", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a+b(/a", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("((a+b)", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("b++b+c", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("bb+c", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("b+", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
//      // unary operators not allowed:
//    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
//    assert(evaluate("a*b", m, pf, answer) == 2  &&
//                            pf == "ab*"  &&  answer == 999);
//    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
//                            pf == "yoau-*+"  &&  answer == -1);
//    answer = 999;
//    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
//                            pf == "oyy-/"  &&  answer == 999);
//    assert(evaluate(" a  ", m, pf, answer) == 0  &&
//                            pf == "a"  &&  answer == 3);
//    assert(evaluate("((a))", m, pf, answer) == 0  &&
//                            pf == "a"  &&  answer == 3);
//    assert(evaluate("((a+e))", m, pf, answer) == 0 &&
//                            pf == "ae+" && answer == -6);
//    cout << "Passed all tests" << endl;
//}
