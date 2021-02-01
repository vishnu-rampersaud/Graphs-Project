/*
	Author: Vishnu Rampersaud 
	This creates an adjacency list that represents a given graph
	Date: 12/10/19
*/

#include "Vertex.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
using namespace std; 

// @graph_filename: name of the graph file
// @adjacency_list: Empty map to create adjacency list with 
// This function Uses a map structure to store vertices of a graph into an adjacency list 
template<typename Type>
void createAdjacencyList(const string& graph_filename, map <Type, Vertex<Type>>& adjacency_list)
{
	// Open graph text file that has the specifications of the graph, vertices, edges, and weights 
	ifstream graph_; 
	graph_.open(graph_filename); 

	string line; 
	int counter = 0, number_of_vertices = 0; 

	// Read each line of the graph text file, and create an adjacency list 
	while (!graph_.eof())
	{
		// get current line of the file which contains a Vertex and all of its adjacent vertices/ edges and weights 
		getline(graph_, line); 

		if (line.empty()) { continue; }

		// counter to keep track of which line the program is reading in the file 
		counter++; 

		// The first line gives the number of vertices for the graph
		// When program is at the first line, store the number of vertices into a variable, number_of_vertices 
		if (counter == 1) {
			stringstream ss(line);
			ss >> number_of_vertices; 
			continue; 
		}

		// For each vertex in the graph, store their adjacent vertices and the corresponding weights into a map 
		// This map will be the adjacency list
		stringstream ss(line); 
		for (int i = 0; i < number_of_vertices; i++) {

			Type vertex_name, adjacent_vertex; 
			double adjacent_weight; 
			
			// First element in the line is the vertex name
			ss >> vertex_name; 

			// Create a new Vertex object that will hold all adjacent vertices and their weights
			Vertex<Type> v(vertex_name); 

			// Add each adjacent vertex and its weight into the Vertex object
			while (ss >> adjacent_vertex) {

				ss >> adjacent_weight; 

				v.add_adjacent_vertex(adjacent_vertex, adjacent_weight); 
			}

			// Add vertex to map (adjacency list) 
			// This vertex now contains all data about its adjacent vertices and weights
			adjacency_list.insert(pair<Type, Vertex<Type>> {vertex_name, v}); 
		}

	}

	// Close the graph file
	graph_.close(); 
}

// @ adjacenyQuery_filename: file that contains a list of edges
// @ adjacency_list: adjacency list of a graph
// This function takes in a file (adjacencyQuery_filename) which has a list of edges (2 vertices) 
// and tests whether these edges are present in an actual graph using the adjacency list of a graph
template<typename Type> 
void test_graph(const string& adjacencyQuery_filename, const map <Type, Vertex<Type>>& adjacency_list)
{
	// Open adjacency query test file; contains list of edges
	ifstream adj_query; 
	adj_query.open(adjacencyQuery_filename); 

	Type vertex_1, vertex_2; 
	string line; 

	// For each edge in the file, check whether it exists on the graph
	// If it exists, output its weight
	while (!adj_query.eof())
	{
		// Get edge from file 
		getline(adj_query, line); 
		if (line.empty()) { continue; }

		// Store the two vertices of the edge in variables
		stringstream ss(line); 
		ss >> vertex_1 >> vertex_2; 
		
		// Checks to see if the first vertex is a part of the graph
		// If the vertex is in the graph, return its location so its data can be accessed 
		auto itr = adjacency_list.find(vertex_1); 
		if (itr == adjacency_list.end()) {
			cout << vertex_1 << " " << vertex_2 << ": " << "Vertex " << vertex_1 << " not found" << endl;
			continue; 
		}

		// Get the adjacent vertices of vertex_1 
		vector<pair<Type, double>> adjacent_vertices = itr->second.get_adjacent_nodes();
		
		// Check to see if vertex_2 of the test edge is adjacent to vertex_1 in the graph
		// If it is, then this edge exists on the graph; Output its weight
		for (int i = 0; i <= adjacent_vertices.size(); i++) {
			if (i == adjacent_vertices.size()) {
				cout << vertex_1 << " " << vertex_2 << ": Not connected" << endl;
			}
			else if (vertex_2 == adjacent_vertices[i].first) {
				double weight = adjacent_vertices[i].second;
				cout << vertex_1 << " " << vertex_2 << ": Connected, weight of edge is " << weight << endl;
				break; 
			}
		}
		
	}

	// Close file
	adj_query.close(); 
}
bool testFiles(const vector<string>& files)
{
	ifstream file; 
	for (int i = 0; i < files.size(); i++) {
		file.open(files[i]); 
		if (file.is_open()) {
			file.close(); 
			continue; 
		}
		else {
			cout << files[i] << " does not exist in the current directory." << endl;
			cout << "Program will terminate" << endl;
			return false; 
		}
	}

	return true; 
}

int main(int argc, char **argv)
{
	// Checks to see if the correct number of arguments are entered
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <graph_filename> <AdjacencyQuery_filename>" << endl;
		return 0; 
	}

	// Store arguments into a variable 
	const string graph_filename(argv[1]); 
	const string adjacencyQuery_filename(argv[2]); 

	// Test to see if files are valid; If they exist in the current directory
	vector<string> files; 
	files.push_back(graph_filename); 
	files.push_back(adjacencyQuery_filename); 
	if (!testFiles(files)) {
		return 0; 
	}
	
	// Display the files that the user inputted
	cout << "\nInput Graph file: " << graph_filename << "\nInput Adjacency Query file: " << adjacencyQuery_filename << "\n" << endl;

	// Create empty map to store adjacency list 
	map <int, Vertex<int>> adjacency_list; 

	// Create adjacency list to represent the input graph 
	createAdjacencyList(graph_filename, adjacency_list); 

	// Test the adjacency list for accuracy 
	test_graph(adjacencyQuery_filename, adjacency_list); 

	return 0; 
}
