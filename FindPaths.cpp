/*
	Author: Vishnu Rampersaud
	Uses Dijkstra's algorithm to find the shortest path to all vertices
*/

#include "Vertex.h"
#include "binary_heap.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <limits>
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

// Dijkstra's algorithm 
// Finds the shortest path to all vertices in a graph 
// @ starting_vertex: the starting vertex of where to start running the algorithm based on user input
// @ priority_queue: a binary heap priority queue 
// @ adjacency_list: an adjacency list that represents a graph
template <typename Type>
void dijkstra(const Type& starting_vertex, BinaryHeap<Vertex<Type>>& priority_queue, map <Type, Vertex<Type>>& adjacency_list)
{
	// Make sure the priority queue is empty 
	priority_queue.makeEmpty();

	// Set a constant for infinity 
	const int infinity = std::numeric_limits<int>::max();

	// Initialize starting vertex distance, cost, and path to 0
	// Initialize all other Vertex distances to infinity and path to 0
	// Then store all Vertices in the priority queue
	for (auto itr = adjacency_list.begin(); itr != adjacency_list.end(); ++itr) {
		if (itr->first == starting_vertex) {
			itr->second.setDistance(0); 
			itr->second.setPath(0);
			itr->second.set_minPathCost(0.0); 
		} 
		else {
			itr->second.setDistance(infinity);
			itr->second.setPath(0);
		}
		priority_queue.insert(itr->second); 
	}

	// Run Dijkstra's algorithm until the shortest path to all vertices is found
	while (!priority_queue.isEmpty()) {

		// Delete the minimum value from the heap (smallest distance) 
		// Store the vertex into a variable, minVertex
		// This is the vertex with the shortest known distance
		Vertex<Type> minVertex; 
		priority_queue.deleteMin(minVertex);

		// get all adjacent vertices of the minimum vertex
		vector<pair<Type, double>> adjacent_vertices = minVertex.get_adjacent_nodes();

		// get the distance of the minimum vertex
		double distance_of_minVertex = minVertex.get_distance(); 

		// get the name of the minimum vertex
		Type name_of_minVertex = minVertex.get_vertex_name();

		// for each vertex that is adjacent to the minimum vertex
		for (int i = 0; i < adjacent_vertices.size(); i++) {

			// find the adjacent vertex  
			auto itr = adjacency_list.find(adjacent_vertices[i].first); 
			if (itr == adjacency_list.end()) {
				cout << "Error" << endl;
				return;
			}

			// get the adjacent vertex name
			Type adjacent_name = itr->second.get_vertex_name();
			// get the adjacent vertex distance
			double adjacent_distance = itr->second.get_distance(); 
			// get the adjacent vertex edge cost/ weight
			double adjacent_edge = adjacent_vertices[i].second;

			// Compute a new shortest path distance for this vertex
			// if it is smaller than the current distance, then update it with this distance 
			if (distance_of_minVertex + adjacent_edge < adjacent_distance) {

				// Compute the shortest path for this adjacent vertex
				double new_distance = distance_of_minVertex + adjacent_edge;

				// Update the distance of the adjacent vertex 
				itr->second.setDistance(new_distance);

				// Update the path variable of this vertex to the minimum vertex
				itr->second.setPath(name_of_minVertex); 

				// Set the cost of the new shortest path of this vertex
				itr->second.set_minPathCost(new_distance); 

				// Update the distance of this adjacent vertex in the priority queue
				Vertex<Type> updateQueueValue(adjacent_name);
				updateQueueValue.setDistance(new_distance); 
				priority_queue.decreaseKey(updateQueueValue); 
			}
		}
	}
}

// Print shortest path of specified vertex
// Recursive algorithm 
// @ v: print the shortest path of this vertex, v
// @ adjacency_list: adjacency list that represents a graph
template <typename Type>
void printPath(const Vertex<Type>& v, const map <Type, Vertex<Type>>& adjacency_list)
{
	if (v.getPath() != 0) {
		auto itr = adjacency_list.find(v.getPath()); 
		printPath(itr->second, adjacency_list); 
		cout << ", "; 
	}
	cout << v; 
}

// Print shortest path of all vertices in the graoh along with their costs
// @ adjacency_list: adjacency list that represents a graph
template <typename Type>
void printOutput(const map <Type, Vertex<Type>>& adjacency_list)
{
	for (auto itr = adjacency_list.begin(); itr != adjacency_list.end(); ++itr) {
		cout << itr->first << ": "; 
		printPath(itr->second, adjacency_list); 
		double cost = itr->second.get_minPathCost(); 
		cout << " (Cost: " << cost << ")" << endl;
	}
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
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <GRAPH_FILE> <STARTING_VERTEX>" << endl;
		return 0;
	}

	// Store arguments into a variable 
	const string graph_filename(argv[1]);

	// Test to see if files are valid; If they exist in the current directory
	vector<string> files;
	files.push_back(graph_filename);
	if (!testFiles(files)) {
		return 0;
	}

	// Cast vertex to int variable because that's what is needed for the project
	int starting_vertex;
	starting_vertex = std::atoi(argv[2]); 

	// Display files that will be used
	cout << "\nInput Graph file: " << graph_filename << "\nInput Starting Vertex: " << starting_vertex << "\n" << endl;

	// Create empty map to store adjacency list 
	map <int, Vertex<int>> adjacency_list;

	// Create empty priority Queue
	BinaryHeap<Vertex<int>> priority_queue;

	// Create adjacency list to represent the input graph 
	createAdjacencyList(graph_filename, adjacency_list);

	// Run the Dijkstra algorithm on the graph
	dijkstra(starting_vertex, priority_queue, adjacency_list); 

	// Print the shortest path of the vertices and their costs
	printOutput(adjacency_list); 
	return 0;
}
