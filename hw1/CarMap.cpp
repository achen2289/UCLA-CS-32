//
//  CarMap.cpp
//  hw1
//
//  Created by Alex Chen on 1/19/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "CarMap.h"
#include <string>
#include <iostream>

CarMap::CarMap()       // Create an empty car map.
{
}

bool CarMap::addCar(std::string license)
  // If a car with the given license plate is not currently in the map,
  // and there is room in the map, add an entry for that car recording
  // that it has been driven 0 miles, and return true.  Otherwise,
  // make no change to the map and return false.
{
    return m_map.insert(license, 0);
}

double CarMap::miles(std::string license) const
  // If a car with the given license plate is in the map, return how
  // many miles it has been driven; otherwise, return -1.
{
    if (m_map.contains(license))
    {
        double value;
        m_map.get(license, value);
        return value;
    }
    return -1;
}

bool CarMap::drive(std::string license, double distance)
  // If no car with the given license plate is in the map or if
  // distance is negative, make no change to the map and return
  // false.  Otherwise, increase by the distance parameter the number
  // of miles the indicated car has been driven and return true.
{
    double value;
    m_map.get(license, value);
    return (distance >= 0 && m_map.update(license, value+distance));
}

int CarMap::fleetSize() const  // Return the number of cars in the CarMap.
{
    return m_map.size();
}

void CarMap::print() const
  // Write to cout one line for every car in the map.  Each line
  // consists of the car's license plate, followed by one space,
  // followed by the number of miles that car has been driven.  Write
  // no other text.  The lines need not be in any particular order.
{
    for (int i=0; i<m_map.size(); i++)
    {
        std::string key;
        double value;
        m_map.get(i, key, value);
        std::cout << key << " " << value << std::endl;
    }
}
