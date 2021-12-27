//
// Created by dstek on 11/19/2021.
//

#ifndef DSA2PROGRAM3_GRAPH_H
#define DSA2PROGRAM3_GRAPH_H
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include "heap.h"
#include "hash.h"

using namespace std;



class graph {
//creates the Edges for the graph, consisting of just a weight and a string with the name of the destination vertex
//A constructor is also made
private: class Edge {

    int weight;
    string destinationVertex;
    Edge (int cost, string dest){
        weight = cost;
        destinationVertex = dest;
    }
    friend class graph;
    };
//creates the vertices, with a name, distance, known variable, adjacency list for edges, pointer to previous nodes, and
//a constructor
private: class Vertex {
        string vertName;
        int distance;
        bool known;
        int adjNum;
        list<Edge> adjList;
        Vertex *prevVert;

        Vertex(string name){
            vertName = name;
            distance = INT32_MAX;
            known = false;
            adjNum = 0;
            list<Edge> adjList;
            prevVert = nullptr;

        }


        friend class graph;
    };
public:
    //constructor for graph
    graph(int size);

   public: hashTable checking;
    //loads the graph
    void readFile(string fileName);
    //writes to the file
    void writeFile (string fileName);

    //adds a vertex to the graph
    void insertVertex(string name);

    //adds an edge
    void insertEdge(int weight, string source, string destination);
    list <Vertex *> nodes;




    string dijkstra(string source);

    string getPrevNames(string vertName);
    void getOutList(ofstream outFile);
    void checkVert(string &vertName);
    private: string determineNum(int num);

};


#endif //DSA2PROGRAM3_GRAPH_H
