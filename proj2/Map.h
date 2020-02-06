//
//  Map.h
//  proj2
//
//  Created by Alex Chen on 1/23/20.
//  Copyright © 2020 cs. All rights reserved.
//

#ifndef MAP_H
#define MAP_H

#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
  public:
    Map();
    Map(const Map &src);
    Map &operator=(const Map &src);
    ~Map();
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
  private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* next;
        Node* prev;
    };
    Node* head;
    int m_size;
};

// non-member functions

bool combine(const Map& m1, const Map& m2, Map& result);

void reassign(const Map& m, Map& result);

#endif /* MAP_H */
