//
//  testMap.cpp
//  hw1
//
//  Created by Alex Chen on 1/17/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <cassert>

int main()
{
    Map m;  // test Map()
    assert(m.empty()); // test empty()
    ValueType v = -1234.5; // declare double
    assert( !m.get("abc", v)  &&  v == -1234.5); // get() fails when key is not found
    assert(m.insert("xyz", 9876.5)); // insert() first key value pair
    assert(!m.insert("xyz", 9876.5)); // insert() failure when key is already present
    assert(m.size() == 1); // test size()
    assert(m.insert("what", 10.0)); // test insert()
    assert(m.size() == 2); // test size()
    KeyType k = "hello"; // declare string
    assert(m.get(1, k, v)  &&  k == "what"  &&  v == 10.0); // test get(), which changes k and v
    assert(m.update("what", 300)); // test update()
    assert(m.get(1, k, v)  &&  k == "what"  &&  v == 300); // confirm update() success
    assert(m.insertOrUpdate("no", 628)); // test insertOrUpdate()'s insertion functionality
    assert(m.get(2, k, v)  &&  k == "no"  &&  v == 628); // test insertOrUpdate()'s insertion functionality
    assert(m.insertOrUpdate("no", 700)); // test insertOrUpdate()'s update functionality
    assert(m.get(2, k, v)  &&  k == "no"  &&  v == 700); // confirm insertOrUpdate()'s update functionality
    assert(m.erase("what")); // test erase()
    assert(!m.contains("what")); // confirm erase()'s functionality and tests contains()
    int mSize = m.size();
    Map m2;
    m2.swap(m); // test swap()
    assert(m2.size() == mSize); // confirm swap()'s functionality
    std::cout << "Passed all tests" << std::endl;
}
