//
//  testnewMap.cpp
//  hw1
//
//  Created by Alex Chen on 1/20/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "newMap.h"
#include <iostream>
#include <cassert>

int main()
{
    Map a(1000);   // a can hold at most 1000 key/value pairs
    Map b(5);      // b can hold at most 5 key/value pairs
    Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
    KeyType k[6] = {"hi", "bye", "abc", "def", "ghi", "say"};
    ValueType v  = 123.4;

      // No failures inserting pairs with 5 distinct keys into b
    for (int n = 0; n < 5; n++)
        assert(b.insert(k[n], v)); // test insert()

      // Failure if we try to insert a pair with a sixth distinct key into b
    assert(!b.insert(k[5], v));

      // When two Maps' contents are swapped, their capacities are swapped
      // as well:
    a.swap(b);
    assert(!a.insert(k[5], v)  &&  b.insert(k[5], v));
    assert(b.size() == 1); // ensure sizes were swapped
    b.swap(a); // swap back for further tests
    Map d = b; // test copy constructor
    assert(d.size() == 5);
    c = b; // test assignment operator
    assert(c.size() == 5);
    std::cout << "All test cases passed!" << std::endl;
}


