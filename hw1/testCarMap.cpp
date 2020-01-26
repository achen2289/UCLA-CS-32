//
//  testCarMap.cpp
//  hw1
//
//  Created by Alex Chen on 1/20/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "CarMap.h"
#include <iostream>
#include <cassert>

int main()
{
    CarMap carMap;
    assert(carMap.fleetSize() == 0);
    assert(carMap.addCar("abc")); // test addCar();
    assert(!carMap.addCar("abc")); // test addCar() if car already exists
    assert(carMap.fleetSize() == 1);
    assert(carMap.miles("abc") == 0); // test miles() if car exists
    assert(carMap.miles("bcd") == -1); // test miles() if car does not exist
    assert(carMap.drive("abc", 10)); // test drive()
    assert(carMap.miles("abc") == 10); // confirm drive()'s functionality
    assert(!carMap.drive("abc", -3)); // test drive() if distance is negative
    assert(carMap.drive("abc", 20.4)); // test drive() if car already has mileage
//    assert(carMap.miles("abc") == 30.4); // confirm drive()'s functionality whem car already has mileage
    std::cout << "All test cases have passed!" << std::endl;
    // additional tests below, test of print() below
    assert(carMap.addCar("subaru"));
    assert(carMap.addCar("ferrari"));
    assert(carMap.addCar("tesla"));
    assert(carMap.fleetSize() == 4);
    assert(carMap.drive("subaru", 30));
    assert(carMap.drive("subaru", 3.4));
    assert(carMap.drive("ferrari", 24.3));
    assert(carMap.drive("ferrari", 14.3));
    assert(carMap.miles("subaru") == 33.4);
    assert(carMap.miles("ferrari") == 38.6);
    assert(carMap.miles("tesla") == 0);
    carMap.print();
}
