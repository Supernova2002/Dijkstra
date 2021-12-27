//
// Created by David on 10/13/2021.
//


//
// This program allows the user to manipulate a binary heap.
// The program only inserts string ids with with associated keys
// into the heap. The heap class, however, is capable of storing
// arbitrary pointers along with each heap item.
//

#include <iostream>
#include <string>
#include <cstdlib>

#include "heap.h"

using namespace std;



heap::heap(int capacity) :mapping(capacity*2){
    // Allocate space for the nodes (0 slot is not used)
    data.resize(capacity+1);
    heapCapacity = capacity;
    currentSize = 0;

}

int heap::getPos(node *pn) {
    int pos = pn - &data[0];
    return pos;

}
//percolates the key at the given position in the array down
void heap::percolateDown(int posCur) {
    node hole = data[posCur];

    while (posCur * 2 <= currentSize) {
        int child = posCur * 2;
        if (child + 1 <= currentSize && data[child+1].key < data[child].key)
            child = child + 1;
        if (data[child].key > hole.key)
            break;
        data[posCur] = data[child];
        mapping.setPointer(data[posCur].id, &data[posCur]);
        posCur = child;
    }

    data[posCur] = hole;
    mapping.setPointer(data[posCur].id, &data[posCur]);

    /*    node hole = data[currentSize + 1];

      if (posCur * 2 <= heapCapacity) {
          // Iterate until both children are empty
          while (data[posCur * 2].id != "" && data[posCur * 2 + 1].id != "") {
              if (data[posCur * 2].key < data[posCur * 2 + 1].key) {
                  // Move child up
                  data[posCur] = data[posCur * 2];

                  // Update pointer and new hole that is shifted down
                  mapping.setPointer(data[posCur].id, &data[posCur]);
                  data[posCur * 2] = {"", 0, nullptr};

                  posCur *= 2;

                  // Break if outside heapCapacity to not check memory that
                  // is not allocated
                  if (posCur * 2 > heapCapacity) {
                      break;
                  }
              } else {
                  data[posCur] = data[posCur * 2 + 1];

                  mapping.setPointer(data[posCur].id, &data[posCur]);
                  data[posCur * 2 + 1] = {"", 0, nullptr};

                  posCur = posCur * 2 + 1;
                  if (posCur * 2 > heapCapacity) {
                      break;
                  }
              }
          }
      }
      if (posCur * 2 < heapCapacity) {
          // If right node is empty, but left is not
          // move the left node up
          if (data[posCur * 2].id != "") {
              data[posCur] = data[posCur * 2];

              mapping.setPointer(data[posCur].id, &data[posCur]);
              data[posCur * 2] = {"", 0, nullptr};

              // Exit function as final position in heap vector will be empty
              return;
          }
      }
      if (currentSize + 1 <= heapCapacity) {
          // If last child was not moved up in deleteMin
          // Insert it at the hole and percolate up in case
          // it is less than parent
          if (data[currentSize + 1].id != "") {
              data[currentSize + 1] = {"", 0, nullptr};
              data[posCur] = hole;
              percolateUp(posCur);
          }
      } */
}

//moves the key at the given position up to where it should fit
void heap::percolateUp(int posCur) {
    node hole = data[posCur];
    while(posCur !=1){
        if (hole.key >= data[posCur/2].key){
            data[posCur] = hole;

            mapping.setPointer(data[posCur].id, &data[posCur]);
            return;
        }
        else{
            data[posCur] = data[posCur/2];
            mapping.setPointer(data[posCur].id, &data[posCur]);
            posCur /= 2;
        }
    }
    data[posCur] = hole;
    mapping.setPointer(data[posCur].id, &data[posCur]);
}

//inserts the specific node to the heap, and maps the id to the hash table, and then updates the currentSize of the heap
int heap::insert(const std::string &id, int key, void *pv) {
    // checks if the heap is full, returns 1 if it is
    if (currentSize == heapCapacity)
    {
        return 1;
    }
    //checks if the heap already contains the id to be inserted
    if (mapping.contains(id))
    {
        return 2;
    }
    //if neither of the previous conditions is met, inserts the id into the hash and the key and pv into the heap
    int posCur = currentSize+1;
    data[posCur] = {id, key, pv};
    mapping.insert(id,&data[posCur]);
    percolateUp(posCur);
    currentSize++;
    return 0;
}

//deletes the minimum key in the heap, removes the id from the hash, and outputs the corresponding removed values
int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
    //checks if heap is empty
    if (currentSize== 0)
    {
        return 1;
    }
    if (pId != nullptr){
        *pId= data[1].id;
    }
    if (pKey != nullptr){
        *pKey = data[1].key;
    }
    if (ppData != nullptr){
        *(static_cast<void **> (ppData)) = data[1].pData;
    }

    //removes the corresponding id from the hash table, and then moves the corresponding key down to the bottom of the
    //heap and sets that value to 0
    mapping.remove(data[1].id);
    //data[1] = {"", INT32_MAX, nullptr};
    data[1] = data[currentSize];
    mapping.setPointer(data[1].id, &data[1]);

    //decreases the number of filled spots by 1
    currentSize --;
    percolateDown(1);


    return 0;
}
//sets the key of a given id to a desired value
int heap::setKey(const std::string &id, int key) {
    //checks if the id exists in the hash, returns 1 if it does not
    if (!mapping.contains(id)){
        return 1;
    }
    //if the id exists in the hash, grabs the pointer to the position in the heap and records the key. If the desired
    //new key value is greater than the original key value, the corresponding node is percolated down to maintain the
    //heap property. If the new key is less than the original, the node is percolated up, also to maintain the heap
    //order property
    bool check;
    node *temp = static_cast<node *>(mapping.getPointer(id, &check));
    int oldVal = temp->key;

    temp->key = key;

    if (key > oldVal){
        percolateDown(getPos(temp));

    }
    if (key<oldVal){
        percolateUp(getPos(temp));
    }
    return 0;



}
//removes a node from the heap given the id value
int heap::remove(const std::string &id, int *pKey, void *ppData) {
    //checks if the id exists within the heap
    if (!mapping.contains(id)){
        return 1;
    }
    //If the id exists in the heap, grabs the pointer to the corresponding node. Remove the corresponding id, then
    //percolate down the node and delete it
    bool check;
    node *temp = static_cast<node *>(mapping.getPointer(id, &check));
    int pos = getPos(temp);
    if (temp != nullptr){
        if (pKey != nullptr && temp != nullptr){
            *pKey = temp->key;
        }
        if (ppData != nullptr && temp != nullptr){
            ppData = temp->pData;
        }
        mapping.remove(data[pos].id);
        //data[pos] = {"", INT32_MAX, nullptr};
        data[pos] = data[currentSize];
        mapping.setPointer(data[pos].id, &data[pos]);
        currentSize --;
        percolateUp(pos);
        percolateDown(pos);

        return 0;
    }
    else{
        return 1;
    }







}
