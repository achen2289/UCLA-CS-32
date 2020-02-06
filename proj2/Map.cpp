//
//  Map.cpp
//  proj2
//
//  Created by Alex Chen on 1/23/20.
//  Copyright © 2020 cs. All rights reserved.
//

#include "Map.h"

Map::Map()               // Create an empty map (i.e., one with no key/value pairs)
{
    head = nullptr;
    m_size = 0;
}

Map::Map(const Map &src)
{
    m_size = 0; // m_size incremented by insert
    head = nullptr; // insert deals with head = nullptr case
    Node* srcP = src.head; // starting point of copying nodes
    while (srcP != nullptr)
    {
        insert(srcP->m_key, srcP->m_value);
        srcP = srcP->next;
    }
}

Map& Map::operator=(const Map &src)
{
    if (&src == this)
    {
        return *this;
    }
    m_size = 0; // insert will increment m_size
    Node* p = head;
    while (p != nullptr) // delete all elements from linked list, except head
    {
        Node* temp = p;
        p = p->next;
        delete temp;
    }
    head = nullptr;
    p = src.head; // source linked list traversal pointer
    while (p != nullptr)
    {
        insert(p->m_key, p->m_value);
        p = p->next;
    }
    return *this;
}

Map::~Map()
{
    Node* p = head;
    while (p != nullptr) // delete all elements from linked list, except head
    {
        Node* temp = p;
        p = p->next;
        delete temp;
    }
    head = nullptr; // defensive coding??
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
    return head == nullptr;
}

int Map::size() const    // Return the number of key/value pairs in the map.
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
  // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that either the key is already in the map, or the map has a fixed
  // capacity and is full.
{
    Node *p;
    if (head == nullptr) // if nothing in list
    {
        p = new Node;
        p->m_key = key;
        p->m_value = value;
        p->next = nullptr;
        p->prev = nullptr;
        head = p;
        m_size++;
        return true;
    }
    else
    {
        p = head;
        while (p->next != nullptr) // find last item or element which contains key
        {
            if (p->m_key == key)
            {
                break;
            }
            p = p->next;
        }
        if (p->m_key == key) // case: last element contains key -- this conditional will test this case
        {
            return false;
        }
        Node *q = new Node; // dynamic allocation, q is new last element and key was not found in linked list
        q->m_key = key;
        q->m_value = value;
        q->next = nullptr;
        q->prev = p;
        p->next = q;
//        Node *temp = head;
//        temp->prev = q;
        m_size++;
        return true;
    }
}

bool Map::update(const KeyType& key, const ValueType& value)
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // Otherwise, make no change to the map and return false.
{
    Node *p = head;
    while (p != nullptr) // find last item
    {
        if (p->m_key == key)
        {
            p->m_value = value;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
  // If key is equal to a key currently in the map, then make that key no
  // longer map to the value that it currently maps to, but instead map to
  // the value of the second parameter; return true in this case.
  // If key is not equal to any key currently in the map, and if the
  // key/value pair can be added to the map, then do so and return true.
  // Otherwise, make no change to the map and return false (indicating
  // that the key is not already in the map and the map has a fixed
  // capacity and is full).
  // Insert should always be successful if update fails
{
    if (update(key, value)) // if true is returned, value is updated
    {
        return true;
    }
    else // if update returns false, then key is not in list
    {
        return insert(key, value); // if key is not in list, key must be able to be inserted
    }
}

bool Map::erase(const KeyType& key)
  // If key is equal to a key currently in the map, remove the key/value
  // pair with that key from the map and return true.  Otherwise, make
  // no change to the map and return false.
{
    if (head == nullptr) // case: no elements in list
    {
        return false;
    }
    else if (head->next == nullptr) // case: one element in list
    {
        if (head->m_key == key)
        {
            Node *p = head;
            head = head->next; // set head to nullptr
            delete p; // delete what head used to point to
            m_size--;
            return true;
        }
        return false; // if only one element in list, but element does not contain key, this is false
    }
    else // case: more than one element in list
    {
        Node *p = head; // no way p is nullptr
        while (p->next != nullptr)
        {
            if (p->m_key == key) // p will be pointing to element to be deleted, if p is last element, comparison will not have chance to run
            {
                break;
            }
            p = p->next;
        }
        if (p->m_key == key) // checks if element contains key
        {
            Node *kill = p;
            p->prev->next = p->next; // adjust pointer of element before p to element after p
            if (p->next != nullptr)
            {
                p->next->prev = p->prev; // adjust pointer of element after p to element before p
            }
            delete kill; // delete elment originally pointed to by p
            m_size--;
            return true;
        }
        return false; // key not found
    }
}
 
bool Map::contains(const KeyType& key) const
  // Return true if key is equal to a key currently in the map, otherwise
  // false.
{
    Node *p = head;
    while (p != nullptr) // all elements will be tested
    {
        if (p->m_key == key)
        {
            return true;
        }
        p = p->next;
    }
    return false;
}
 
bool Map::get(const KeyType& key, ValueType& value) const
  // If key is equal to a key currently in the map, set value to the
  // value in the map which that key maps to and return true.  Otherwise,
  // make no change to the value parameter of this function and return
  // false.
{
    Node *p = head; // start pointer at head
    while (p != nullptr) // all elements will be tested
    {
        if (p->m_key == key) // if p points at element w/ m_key == key
        {
            value = p->m_value;
            return true;
        }
        p = p->next;
    }
    return false;
}
 
bool Map::get(int i, KeyType& key, ValueType& value) const
  // If 0 <= i < size(), copy into the key and value parameters the
  // key and value of one of the key/value pairs in the map and return
  // true.  Otherwise, leave the key and value parameters unchanged and
  // return false.
{
    if (i >= 0 && i < m_size) // i must be less than m_size
    {
        Node *p = head;
        for (int a=0; a<i; a++)
        {
            if (p != nullptr)
            {
                p = p->next;
            }
        }
        if (p != nullptr) // once out of loop, p will point to correct element, and should not be equal to nullptr
        {
            key = p->m_key;
            value = p->m_value;
            return true;
        }
    }
    return false;
}

void Map::swap(Map& other)
  // Exchange the contents of this map with the other one.
{
    if (&other == this)
    {
        return;
    }
    
    // swap head pointers, effectively swapping contents of each Map
    Node* temp = other.head;
    other.head = head;
    head = temp;
    
    // swap sizes
    int tempSize = other.m_size;
    other.m_size = m_size;
    m_size = tempSize;
}

// non-member functions

// combine Maps m1 and m2 into result
// duplicate key and value pairs will only be copied once
// duplicate keys with different values will not be present in result, and function will return false
// otherwise, function returns true
bool combine(const Map& m1, const Map& m2, Map& result)
{
    bool status = true;
    Map smallerMap;
    
    // copy contents of larger map into result
    // copy contents of smaller map into smallerMap
    if (m1.size() < m2.size())
    {
        result = m2;
        smallerMap = m1;
    }
    else
    {
        result = m1;
        smallerMap = m2;
    }
    
    // end function if m1 and m2 refer to same Map, result has corrent contents already
    if (&m1 == &m2)
    {
        return true;
    }
    
    // iterate through smallerMap to find duplicate key value pairs
    for (int i=0; i<smallerMap.size(); i++)
    {
        KeyType k;
        ValueType v;
        smallerMap.get(i, k, v); // get key and value pair from each element
        if (result.contains(k))
        {
            ValueType v2;
            result.get(k, v2);
            if (v2 != v) // if keys match but values don't, erase the key from result, set status to false
            {
                result.erase(k);
                status = false;
            }
            // if keys and values match, result will already contain key value pair
        }
        else // if key is not found in result, copy it into result
        {
            result.insert(k, v);
        }
    }
    return status; // status will be true unless a duplicate key with different value was found
}

// reassign each key of m to a different value in m
// no key may take value from same other key in m
// result contains the swapped key value pairs
void reassign(const Map& m, Map& result)
{
    if (&result == &m)
    {
        return; // immediately end function if result is m
    }
    result = m; // result is now a copy of m
    KeyType k0; // current key
    ValueType v0; // current value
    KeyType k1; // symmetric key
    ValueType v1; // symmetric value
    int maxSize = result.size();
    // go through first half of Map and swap values with corresponding values of keys in second half of map
    for (int i=0; i<maxSize/2; i++)
    {
        result.get(i, k0, v0); // get current key and value
        result.get(maxSize - i - 1, k1, v1); // get symmetric key and value
        result.update(k0, v1); // update current key with symmetric value
        result.update(k1, v0); // update symmetric key with current value
    }
}
