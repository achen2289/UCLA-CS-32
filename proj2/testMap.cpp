//
//  testMap.cpp
//  proj2
//
//  Created by Alex Chen on 1/23/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <cassert>

int main()
{
    Map m; // new map
    assert(m.size() == 0 && m.empty()); // test size() and empty()
    assert(m.insert("Alex", 5.23)); // test insert()
    assert(m.size() == 1); // insure insert increments size
    assert(m.contains("Alex")); // test contains()
    KeyType k; // string to be used for tests
    ValueType v; // double to be used for tests
    assert(m.get("Alex", v) && v == 5.23); // test get(string, double)
    assert(m.get(0, k, v) && k == "Alex" && v == 5.23); // test get(i, string, double)
    assert(m.update("Alex", 3.02)); // test update()
    assert(m.get("Alex", v) && v == 3.02); // test get(string, double) after update()
    assert(m.insert("Billy", 4.11) && !m.empty() && m.size() == 2); // test insert() and empty() and size() after insertion()
    assert(m.get("Billy", v) && v == 4.11);
    assert(m.get(1, k, v) && k == "Billy" && v == 4.11);
    assert(!m.get(2, k, v) && !m.get("slfkj", v)); // test failure of get(i, string, double) and get(string, double)
    assert(m.contains("Billy")); // test contains()
    assert(!m.update("aldskj", v)); // test failure of update()
    assert(m.insertOrUpdate("Alex", 10.2)); // test insertOrUpdate when key exists
    assert(m.insertOrUpdate("Casey", 100.01)); // test insertOrUpdate when key does not exist
    assert(m.insertOrUpdate("Don", 23.4));
    
    Map test = m; // test copy constructor
    assert(test.size() == 4);
    assert(test.get("Alex", v) && v == 10.2);

    // test erase on all elements in list
    assert(m.erase("Don"));
    assert(!m.erase("Esuc"));
    assert(m.erase("Billy"));
    assert(m.erase("Casey"));
    assert(m.erase("Alex"));

    assert(m.size() == 0);
    assert(m.insert("Elaine", 4.002));
    assert(m.insert("Foo", 23.09));
    Map test2 = m;
    assert(test2.size() == 2);
    m = test; // test assignment operator
    assert(m.size() == 4);
    assert(test.size() == 4);
    assert(m.get("Alex", v) && v == 10.2);
    m.swap(test2); // test swap()
    assert(m.size() == 2); // ensure test worked
    assert(m.get("Foo", v) && v == 23.09); // ensure test values swapped
    std::cout << "All test cases (1st round) passed!" << std::endl;
    
    Map testing1;
    testing1.insert("Alex", 50.2);
    testing1.insert("Billy", 404.2);
    testing1.insert("Cab", 401.09);
    Map testing2;
    testing2.insert("Derrick", 909.3);
    testing2.insert("Elaine", 43.2);
    Map result;
    assert(combine(testing1, testing2, result)); // test combine()
    assert(result.size() == 5); // ensure size of result is correct
    assert(result.get(3, k, v) && k == "Derrick" && v == 909.3);
    assert(result.get(1, k, v) && k == "Billy" && v == 404.2);
    assert(testing1.insert("Derrick", 909.3));
    assert(combine(testing1, testing2, result));
    assert(result.size() == 5); // ensure combine works when derrick existed already
    assert(testing1.insert("Elaine", 10.1));
    assert(testing1.insert("Fos", 22.2) && testing2.insert("Foul", 33.3));
    assert(!combine(testing2, testing1, result));
    assert(result.size() == 6); // ensure combine works when key is same but value is not
    assert(!combine(testing2, testing1, result));
    assert(result.size() == 6);
    std::cout << "All test cases (2nd round) passed!" << std::endl;
    
    Map result2;
    Map testing3 = testing1;
    assert(testing3.insert("Gary", 69.3));
    reassign(testing3, result2); // test reassign()
    assert(result2.size() == 7);
    for (int i=0; i<result2.size(); i++) // compare key and value pairs
    {
        result2.get(i, k, v);
        std::cout << k << " " << v << std::endl;
        testing3.get(i, k, v);
        std::cout << k << " " << v << std::endl;
    }
    std::cout << "All test cases (3rd round) passed!" << std::endl;
}
