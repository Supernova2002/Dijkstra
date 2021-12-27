//
// Created by David on 10/13/2021.
//

#include "hash.h"
//
// Created by David on 9/11/2021.
//


#include<bits/stdc++.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>


using namespace std;
//implementation of constructor for hash table, sets size to nearest prime number above needed size
hashTable::hashTable( int size)
{
    capacity = getPrime(size);

    data.resize(capacity);



}


//hash function
int hashTable::hash(const std::string &key, int size)
{
    unsigned hash = 0;

    for (int i = 0; i < key.length(); ++i)
        hash ^= (hash << 5) + (hash >> 2) + key[i];

    return hash % capacity;
}


//list of 9 prime values which should cover any dictionary up to 1.5 million words to be used when rehashing the table
unsigned int hashTable::getPrime(int size)
{
    bool check;
    int primes[9] = {12289,24593,49157,98317,196613,393241, 786433, 1572869,3145739 };
    for (int i=0; i<9; i++)
    {
        if (size<primes[i] && !check)
        {
            size = primes[i];
            check = true;
            return size;
        }
    }



}


// checks if a rehash is needed, and executes it if it is. If a rehash is attempted and fails, returns 2 and exits. If
// a rehash is not needed, checks if the key already exists in the table. If it does not, it hashes the key and inputs
// it along with any corresponding pointer either at the hashed position or the next free one, an implementation of
// linear probing. Then updates the filled variable to properly keep track of how many positions in the table are filled
int hashTable::insert(const std::string &key, void *pv) {
    int counter;
    int original=filled;


    if (filled > (capacity/2))
    {

        if(!rehash())
        {
            return 2;
        }

    }
    int position = hash(key, capacity);
    int range = capacity- position;
    if (contains(key))
    {
        return 1;
    }
    else {
        while (data.at(position).isOccupied)
        {
            position ++;
            position %= capacity;


        }
        data.at(position).key = key;
        data.at(position).pv = pv;
        data.at(position).isOccupied = true;
        data.at(position).isDeleted = false;
        filled ++;
        return 0;
    }



}

//calls findPos to determine whether or not the key exists in the hash table
bool hashTable::contains(const std::string &key) {




    if (findPos(key) != -1)
    {
        return true;
    }
    else
    {
        return false;
    }



}
// hashes the given key and checks the outputted position, if that fails goes until next empty position at which point
// it exits and returns -1. If the key is found, either at the output from the hash function or before the next empty
// position, returns its position.
int hashTable::findPos(const std::string &key) {
    int pos = hash(key, capacity);
    //int counter = 0;

    while (data.at(pos).isOccupied) {
        if (data.at(pos).key == key && !data.at(pos).isDeleted)
            return pos;

        pos = (pos + 1) % capacity;
    }

    return -1;

    /*
    while (data.at(pos).key != key )
    {
        pos ++;
        pos %= capacity;
        if(!data.at(pos).isOccupied)
        {

            return -1;

        }


    }
    if (data.at(pos).key == key){
        if(!data.at(pos).isOccupied & data.at(pos).isDeleted ){
            return -1;
        }
        else {
            return pos;
        }

    }
    else {
        return -1;
    }
    */
}


void *hashTable::getPointer(const std::string &key, bool *b) {
    bool exists = false;
    int position = hash(key, capacity);
    int range = capacity- position;
    if (!contains(key))
    {
        *b = false;
        return nullptr;
    }
    else {
        while (data.at(position).isOccupied)
        {
            if(!data.at(position).isDeleted && data.at(position).key == key)
            {
                *b = true;
                return data.at(position).pv;



            }
            position ++;
            position %= capacity;


        }

        return nullptr;
    }
}

int hashTable::setPointer(const std::string &key, void *pv) {
    bool exists = false;
    int position = hash(key, capacity);
    int range = capacity- position;
    if (!contains(key))
    {
        return 1;
    }
    else {
        while (data.at(position).isOccupied)
        {
            if(!data.at(position).isDeleted && data.at(position).key == key)
            {
                data.at(position).pv = pv;
                return 0;
            }
            position ++;
            position %= capacity;


        }
    }
}

bool hashTable::remove(const std::string &key) {
    bool exists = false;
    int position = hash(key, capacity);
    int range = capacity- position;
    if (!contains(key))
    {
        return false;
    }
    else {
        while (data.at(position).isOccupied)
        {
            if(data.at(position).key == key && !data.at(position).isDeleted)
            {
                //data.at(position).isOccupied = false;
                data.at(position).isDeleted = true;
                return true;
            }
            position ++;
            position %= capacity;


        }
    }
}



//rehashes the hash table to a larger size, returning false if memory is not allocated properly
bool hashTable::rehash(){
    std::vector<hashItem> temp;
    int hashSuccess=true;
    int newSize = getPrime(data.size());
    capacity = newSize;
    temp.resize(data.size());
    for (int i = 0; i<data.size(); i++)
    {
        temp.at(i) = data.at(i);

    }
    data.clear();
    data.resize(newSize);
    filled = 0;
    for (int i = 0; i<temp.size(); i++)
    {
        if (temp.at(i).isOccupied && !temp.at(i).isDeleted)
            insert(temp.at(i).key, temp.at(i).pv);
        /*
            if(!contains(temp.at(i).key))
            {
                return false;
            }
        */

    }
    temp.clear();
    return hashSuccess;


}





