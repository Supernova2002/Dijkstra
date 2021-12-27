#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <list>

#include "graph.h"
using namespace  std;
int main() {
    string inputName;
    string vertName;
    string outName;
    graph newGraph(10000);
    cout << "Enter name of graph file: ";
    cin >> inputName;
    //cout << endl;
    newGraph.readFile(inputName);
    cout << "Enter name of starting vertex: ";
    newGraph.checkVert(vertName);

    cout << "Total time (in seconds) to apply Dijsktra's alogrithm: " << newGraph.dijkstra(vertName) << endl;
    cout << "Enter name of output file: ";
    cin >>outName;
    newGraph.writeFile(outName);



    return 0;
}
