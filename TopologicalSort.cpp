/*
	Author: Vishnu Rampersaud
	Sorts a graph using the Topological sorting algorithm 
*/

#include "Vertex.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include <queue>
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

// Computes the indegree of each vertex in the graph
// @ adjacency list: an adjacency list that represents the graph
template<typename Type> 
void compute_indegree(map <Type, Vertex<Type>>& adjacency_list)
{
	// Set indegree for all vertices to zero
	for (auto itr = adjacency_list.begin(); itr != adjacency_list.end(); ++itr) {
		itr->second.set_indegree(0); 
	}

	// For each vertex, compute the indegree of their adjacent vertices
	// By the end of the nested loops, the correct indegrees of each vertex will be known 
	for (auto itr = adjacency_list.begin(); itr != adjacency_list.end(); ++itr) {

		// Vector that conatains all adjacent vertices of the current vertex 
		vector<pair<Type, double>> adjacent_vertices = itr->second.get_adjacent_nodes();

		// Compute indegree of adjacent vertices
		for (int i = 0; i < adjacent_vertices.size(); i++) {

			// find the vertex and its information in the graph 
			auto itr2 = adjacency_list.find(adjacent_vertices[i].first);
			if (itr2 == adjacency_list.end()) { 
				cout << "Error" << endl; 
				return; 
			}

			// Get current indegree
			int indegree_of_adjacent_vertex = itr2->second.get_indegree(); 
			// Increment indegree
			indegree_of_adjacent_vertex++;
			// Store new indegree 
			itr2->second.set_indegree(indegree_of_adjacent_vertex);
		}
	}
}

// Displays the topological order of the graph
// @topological_order: vector that contains the topological sorting of the graph
template <typename Type>
void display_topSort(const vector<Type>& topological_order)
{
	cout << "Topological Sort: "; 
	for (int i = 0; i < topological_order.size(); i++) {
		if (i == 0) {
			cout << "{" << topological_order[i] << ", ";
		}
		else if (i == topological_order.size() - 1) {
			cout << topological_order[i] << "}" << endl;
		}
		else {
			cout << topological_order[i] << ", ";
		}
	}
}

// Topological sorting function 
// Sorts the graph using the topological sorting algorithm 
// @ adjacency_list: An adjacency list the represents a graph
template <typename Type> 
void topSort(map <Type, Vertex<Type>>& adjacency_list)
{
	// Initialize a queue that will hold all vertices with indegree zero
	queue <Vertex<Type>> indegree_of_zero_queue; 

	// Initialize a vector that will store the topological order of the graph
	vector<Type> topological_order;

	// Computes the initial indegree of all vertices 
	compute_indegree(adjacency_list);

	// @ counter: to asses whether the graph has a cycle
	// @ num_of_vertices: number of vertices in the graph
	int counter = 0, num_of_vertices=0; 

	// Push all vertices with indegree zero onto the queue
	// Also calculate the number of vertices in the graph
	for (auto itr = adjacency_list.begin(); itr != adjacency_list.end(); ++itr) {
		num_of_vertices++; 
		if (itr->second.get_indegree() == 0) {
			indegree_of_zero_queue.push(itr->second); 
		}
	}

	// Topologically sort the graph
	while (!indegree_of_zero_queue.empty()) {
		
		// Get and pop the first vertex from queue with indegree of zero 
		Vertex<Type> v = indegree_of_zero_queue.front(); 
		indegree_of_zero_queue.pop(); 

		// Set the topological number for the vertex
		v.set_topNum(++counter); 

		// Store this vertex in the topological order vector 
		topological_order.push_back(v.get_vertex_name()); 

		// Obtain all adjacent vertices 
		// Compute indegree for all adjacent vertices of this vertex
		//vector <Type> temp = v.get_adjacent_nodes(); 
		vector<pair<Type, double>> adjacent_vertices = v.get_adjacent_nodes();
		for (int i = 0; i < adjacent_vertices.size(); i++) {

			// Find the vertex
			auto itr = adjacency_list.find(adjacent_vertices[i].first);
			if (itr == adjacency_list.end()) {
				cout << "Error" << endl;
				return;
			}

			// Compute new indegree
			int indegree_of_adjacent_vertex = itr->second.get_indegree(); 
			indegree_of_adjacent_vertex--; 
			itr->second.set_indegree(indegree_of_adjacent_vertex); 

			// If indegree is zero, push it onto the queue
			if (indegree_of_adjacent_vertex == 0) {
				indegree_of_zero_queue.push(itr->second); 
			}

		}
	}

	// Checks if the graph has a cycle 
	if (counter != num_of_vertices) {
		cout << "Cycle found" << endl;
		exit(0); 
	}

	// Displays the topological order of the graph
	display_topSort(topological_order); 
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

int main(int argc, char** argv)
{
	// Checks to see if the correct number of arguments are entered
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <GRAPH_FILE>" << endl;
		return 0;
	}

	// Store argument into a variable
	const string graph_filename(argv[1]);

	// Test to see if files are valid; If they exist in the current directory
	vector<string> files;
	files.push_back(graph_filename);
	if (!testFiles(files)) {
		return 0;
	}

	// Display the files being used/ inputted 
	cout << "\nInput Graph file: " << graph_filename << endl;
	cout << endl;

	// Create empty map to store adjacency list 
	map <int, Vertex<int>> adjacency_list;

	// Create adjacency list to represent the input graph 
	createAdjacencyList(graph_filename, adjacency_list);
	
	// Topologically sort the graph
	topSort(adjacency_list); 
 
	return 0;
}