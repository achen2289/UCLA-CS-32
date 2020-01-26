//
//  Map.cpp
//  hw1
//
//  Created by Alex Chen on 1/17/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "Map.h"

Map::Map()
{
    m_size = 0;
}

bool Map::empty() const
{
    return (m_size == 0);
}

int Map::size() const
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
  // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that either the key is already in the map, or the map has a fixed
  // capacity and is full).
{
    if ((!contains(key)) && m_size < DEFAULT_MAX_ITEMS)
    {
        m_pair[m_size].key = key;
        m_pair[m_size].value = value;
        m_size++;
        return true;
    }
    return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // Otherwise, make no change to the map and return false.
{
    for (int i=0; i<m_size; i++)
    {
        if (m_pair[i].key == key)
        {
            m_pair[i].value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value that it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // If key is not equal to any key currently in the map and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that the key is not already in the map and the map has a fixed
  // capacity and is full).
{
    if (update(key, value))
    {
        return true;
    }
    else if (insert(key, value))
    {
        return true;
    }
    return false;
}

bool Map::erase(const KeyType& key)
  // If key is equal to a key currently in the map, remove the key/value
  // pair with that key from the map and return true.  Otherwise, make
  // no change to the map and return false.
{
    for (int i=0; i<m_size; i++)
    {
        if (m_pair[i].key == key)
        {
            m_pair[i] = m_pair[m_size-1];
            m_size--;
            return true;
        }
    }
    return false;
}
 
bool Map::contains(const KeyType& key) const
  // Return true if key is equal to a key currently in the map, otherwise
  // false.
{
    for (int i=0; i<m_size; i++)
    {
        if (m_pair[i].key == key)
        {
            return true;
        }
    }
    return false;
}
 
bool Map::get(const KeyType& key, ValueType& value) const
  // If key is equal to a key currently in the map, set value to the
  // value in the map which that key maps to, and return true.  Otherwise,
  // make no change to the value parameter of this function and return
  // false.
{
    for (int i=0; i<m_size; i++)
    {
        if (m_pair[i].key == key)
        {
            value = m_pair[i].value;
            return true;
        }
    }
    return false;
}
 
bool Map::get(int i, KeyType& key, ValueType& value) const
  // If 0 <= i < size(), copy into the key and value parameters the
  // key and value of one of the key/value pairs in the map and return
  // true.  Otherwise, leave the key and value parameters unchanged and
  // return false.  (See below for details about this function.)
{
    if (0 <= i && i < m_size)
    {
        key = m_pair[i].key;
        value = m_pair[i].value;
        return true;
    }
    return false;
}

void Map::swap(Map& other)
  // Exchange the contents of this map with the other one.
{
    Map temp = other;
    other = *this;
    *this = temp;
}
