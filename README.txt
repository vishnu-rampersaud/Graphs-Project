Graphs 
Author: Vishnu Rampersaud
Class: CSCI 335
Professor: Stamos

There are three ".cpp" main files and three ".h" header files.

The three .cpp files are: 
CreateGraphAndTest.cpp
FindPaths.cpp
TopologicalSort.cpp

The two header files are: 
Vertex.h
binary_heap.h
dsexceptions.h

I created a Vertex class to operate as a vertex of a graph in order to provide all functionality needed. 

I used a map data structure to implement an adjacency list of all vertices in the graph.
The Key is the name of the Vertex, and the value is the Vertex class object (which stores all vertex info)

I used the binary_heap code to create a priority queue 

A makefile was created. 

To compile all programs, run the command: 
make all 

To clear all object files, run command:
make clean 

To run each cpp file, run "make all" command and then the corresponding command below: 

./CreateGraphAndTest <graph_filename> <adjacencyQuery_filename> 
example: ./CreateGraphAndTest Graph1.txt AdjacencyQueries1.txt
 
./FindPaths <graph_filename> <starting_vertex> 
example: ./FindPaths Graph2.txt 1

./TopologicalSort <graph_filename> 
example: ./TopologicalSort Graph3.txt
