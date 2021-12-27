//
// Created by dstek on 11/19/2021.
//

#include "graph.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <list>
#include <chrono>
#include <ctime>
using namespace  std;



graph::graph(int size): checking(size*2)  {


}

//checks if a vertex by the same name already exists, if not inserts a vertex with the given name
void  graph::insertVertex(string name) {
    if (!checking.contains(name)){

        Vertex *newVert = new Vertex(name);

        checking.insert(name, newVert);
        nodes.push_back(newVert);;

    }

}


//first checks if the destination vertex is present, and inserts it if it isn't. Then gets a pointer to the source
//vertex, and adds an edge to the adjacency list with the provided weight and destination name
void graph::insertEdge(int weight, string source, string destination) {
    bool check;
    if (!checking.contains(destination)){
        insertVertex(destination);

    }
    auto temp = static_cast<Vertex *>(checking.getPointer(source, &check));
    Edge newEdge(weight, destination);
    temp->adjList.push_back(newEdge);
}

//reads in the vertices, identifies the source, destination, and weight, and adds in the corresponding vertices and edge
void graph::readFile(string fileName){
    ifstream inFile;
    inFile.open(fileName);
    string line;
    if (!inFile)
    {
        cout << "No file by this name is found" << endl;
    }
    else {


        while (!inFile.eof()) {
            getline(inFile, line);
            if (line == ""){
                break;
            }
            int srcInd = line.find(" ");

            string source = line.substr(0, srcInd);
            int endInd = line.substr(srcInd + 1, line.size() - srcInd - 1).find(" ") + srcInd +1;
            string end = line.substr(srcInd + 1, endInd - srcInd - 1);

            string weightStr = line.substr(endInd + 1);
            int weight = 0;
            stringstream convert(weightStr);
            convert >> weight;
            insertVertex(source);
            insertEdge(weight, source, end);

        }
    }
    inFile.close();
}

string graph::dijkstra(string source) {
    //initialization of heap and necessary variables
    heap vHeap(nodes.size());
    string pId;
    int pKey;
    bool check1;
    bool check2;
    auto start = chrono::system_clock::now();
    Vertex *vertData;
    //inserts all vertices into the heap
    for (auto & i: nodes)
    {
        vHeap.insert(i->vertName, i->distance, &i);
    }
    //ensures that the source vertex is the lowest key in the heap, then sets the distance of the source to 0
    vHeap.setKey(source, 0);

    auto ptr =  static_cast<Vertex *>(checking.getPointer(source, &check1));
    ptr ->distance=0;
    while (true)
    {
        //deletes a vertex from the heap, getting a pointer to the deleted vertex. If the distance is at the default,
        //break as the algorithm is done. Otherwise, set the deleted vertex to known, and loop through all outgoing
        // edges of the vertex. If a lower distance to a vertex is found then already exists, that is set as the new
        //optimal distance. Additionally, then set the previous vertex of the destination vertex to the current vertex,
        //for use in the output later.
        if (vHeap.deleteMin(&pId, &pKey, &vertData) != 1){
            bool check3;
            auto delVertex = static_cast<Vertex *>(checking.getPointer(pId, &check3));
            if (delVertex->distance== INT32_MAX){
                break;
            }
            delVertex -> known = true;
            for (auto & i : delVertex->adjList){
                ptr =  static_cast<Vertex *>(checking.getPointer(i.destinationVertex, &check2));
                if (!ptr->known && i.weight + delVertex->distance < ptr->distance){
                    ptr -> distance = i.weight + delVertex->distance;
                    ptr -> prevVert = delVertex;
                    vHeap.setKey(ptr->vertName, ptr->distance);
                }
            }
        }
        else{
            break;
        }
    }
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    return to_string(elapsed_seconds.count());
    return 0;

}

//given the name of a vertex, gets a pointer to the corresponding vertex and gets a string of all the vertices on the
//optimal path to the vertex from the source, all in the desired format. If vertex distance is at default, there is no
//path and no vertex names are returned
string graph::getPrevNames(string vertName) {
    string output;
    bool check1;
    auto ptr =  static_cast<Vertex *>(checking.getPointer(vertName, &check1));
    if(ptr->distance == INT32_MAX){

    }
    else {
        while (ptr != nullptr) {


            if (ptr->vertName == vertName) {
                if (ptr->prevVert == nullptr) {
                    output = "[" + ptr->vertName + "]";
                } else {
                    output = ", " + ptr->vertName + "]";
                }

            } else if (ptr->prevVert == nullptr) {
                output = "[" + ptr->vertName + output;
            } else {
                output = ", " + ptr->vertName + output;
            }

            ptr = ptr->prevVert;
        }
    }


    output = output + "\n";
    return output;
}

//determines if a number should be written file or if it should be "NO PATH"
string graph::determineNum(int num) {
    string output;
    if(num == INT32_MAX)
    {
        output = "NO PATH";
    }
    else{
        output = to_string(num) + " ";
    }
    return output;
}

//Writes the necessary files to the output
void graph::writeFile(string fileName) {
    ofstream outFile;
    outFile.open(fileName);

    for(auto &i : nodes)
    {
        outFile <<  i->vertName <<": " << determineNum(i->distance) << getPrevNames(i->vertName);

    }



    outFile.close();
}
//Takes in the input for the starting vertex name, and if given an invalid vertex reprompts until a valid vertex is
//given
void graph::checkVert(string &vertName) {
    string newVert;
    cin >> newVert;
    if(!checking.contains(newVert)){
        cout <<"Please enter a valid vertex: ";

        checkVert(vertName);
    }
    else{
        vertName = newVert;
    }

}