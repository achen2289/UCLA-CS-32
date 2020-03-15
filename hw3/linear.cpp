//
//  linear.cpp
//  hw3
//
//  Created by Alex Chen on 2/7/20.
//  Copyright © 2020 cs. All rights reserved.
//

//#include <iostream>

//bool somePredicate(double x)
//{
//    return x > 0;
//}

// Return true if the somePredicate function returns false for at
  // least one of the array elements; return false otherwise.
bool anyFalse(const double a[], int n)
{
    if (n <= 0)
        return false;
    if (n == 1) // base case, returns true if last element does not pass somePredicate()
        return !somePredicate(a[0]);
    return (!somePredicate(a[0]) || anyFalse(a+1, n-1));
}

  // Return the number of elements in the array for which the
  // somePredicate function returns true.
int countTrue(const double a[], int n)
{
    if (n <= 0)
        return 0;
    if (n == 1) // base case, returns 1 if last element passes somePredicate, 0 if not
    {
        if (somePredicate(a[0]))
            return 1;
        return 0;
    }
    if (somePredicate(a[0]))
        return 1 + countTrue(a+1, n-1);
    return countTrue(a+1, n-1);
}

  // Return the subscript of the first element in the array for which
  // the somePredicate function returns true.  If there is no such
  // element, return -1.
int firstTrue(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1) // base case, returns 0 if last element passes somePredicate, -1 if not
    {
        if (somePredicate(a[0]))
            return 0;
        return -1;
    }
    int result = firstTrue(a+1, n-1);
    if (result == -1) // statement will be ran every iteration until first element (from end) passes somePredicate()
    {
        if (somePredicate(a[0]))
            return 0;
        return -1;
    }
    else // whenever element after first element (from end) has passed somePredicate(), 0 is returned, or 1, and 1+1, and 1+1+1, etc.
    {
        if (somePredicate(a[0]))
            return 0;
        return 1 + result;
    }
}

  // Return the subscript of the smallest element in the array (i.e.,
  // return the smallest subscript m such that a[m] <= a[k] for all
  // k from 0 to n-1).  If the function is told to examine no
  // elements, return -1.
int positionOfSmallest(const double a[], int n)
{
    if (n <= 0)
        return -1;
    if (n == 1) // base case, if size of array is one, return 0
        return 0;
    int min = positionOfSmallest(a+1, n-1);
    if (a[0] <= a[min+1]) // begins by having 2nd to last element compared to last element
        return 0;
    return min + 1; // increases returned int by one, unless a smaller value is found
}

  // If all n2 elements of a2 appear in the n1 element array a1, in
  // the same order (though not necessarily consecutively), then
  // return true; otherwise (i.e., if the array a1 does not contain
  // a2 as a not-necessarily-contiguous subsequence), return false.
  // (Of course, if a2 is empty (i.e., n2 is 0), return true.)
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then the function should return true if a2 is
  //    50 20 30
  // or
  //    50 40 40
  // and it should return false if a2 is
  //    50 30 20
  // or
  //    10 20 20
bool contains(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0) // base case 1, if a2 reaches end, this means all elements in a2 were found in a1 in appropriate order
        return true;
    if (n1 <= 0) // base case 2, if a1 reaches end, this means a1 was iterated through and did not find all elements of a2
        return false;

    // keeps going through a1 until elements match up, then goes through both a1 and a2
    if (a1[0] == a2[0])
        return contains(a1+1, n1-1, a2+1, n2-1);
    return contains(a1+1, n1-1, a2, n2);
}

//int main()
//{
//   double arr[] = {-6000, -500, -700, -10, -2900};
//    std::cout << anyFalse(arr, 5) << std::endl;
//    std::cout << countTrue(arr, 5) << std::endl;
//    std::cout << firstTrue(arr, 5) << std::endl;
//    std::cout << positionOfSmallest(arr, 5) << std::endl;
//    double arr1[] = {20, 30, 40, 50, 20, 40};
//    double arr2[] = {20, 40, 60};
//    std::cout << contains(arr1, 6, arr2, 3) << std::endl;
//}


