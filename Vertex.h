/*
	Author: Vishnu Rampersaud
	Vertex.h header file for the Vertex class
*/

#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std; 


// Vertex class 
// Used to store information about an individual vertex in a graph
// Stores the adjacent vertices of this vertex, and the weight of the corresponding edges
template <typename Object> 
class Vertex {
public: 

	// Constructor
	Vertex() {  
		distance = 0; 
	}

	// @ name: the name of the vertex
	// One parameter constructor that sets the name of this vertex
	Vertex(const Object& name)
	{
		vertex_name = name; 
	}

	// Copy constructor 
	Vertex(const Vertex& rhs) = default; 

	// Copy assignment Constructor
	// Only operates on distance variable
	Vertex& operator=(const Vertex& rhs)
	{
		distance = rhs.distance; 
		return *this; 
	}

	//Move Constructor
	Vertex(Vertex&& rhs) = default; 

	//Move assignment constructor
	Vertex& operator=(Vertex&& rhs) = default;

	//Destructor
	~Vertex() = default; 

	// returns name of the vertex
	Object get_vertex_name() const
	{
		return vertex_name; 
	}

	// @ node: name of the adjacent vertex 
	// @ weight: weight of the edge from vertex to this adjacent vertex
	// Adds adjacent vertices along with their corresponding weights to this Vertex
	void add_adjacent_vertex(const Object& node, const double& weight)
	{
		// Create new data structure that holds the adjacent vertex and its weight
		pair<Object, double> adjacent_vertex{ node, weight };

		// Add adjacent vertex to list of adjacent vertices
		adjacent_vertices.push_back(adjacent_vertex);
	}

	// Returns list of adjacent vertices; not their weights
	const vector<pair<Object, double>> get_adjacent_nodes() const
	{
		return adjacent_vertices;
	}

	// Overload the less than operator so that it operates on the distance variables
	bool operator< (const Vertex<Object>& rhs) const
	{
		if (distance < rhs.distance)
		{
			return true;
		}

		else { return false; }
	}

	// Overload the equality operator so that it compares the Vertex names
	bool operator== (const Vertex<Object>& rhs) const
	{
		if (vertex_name == rhs.vertex_name)
		{
			return true;
		}

		else { return false; }
	}

	// Overload ostream operator to print out name of the Vertex
	friend std::ostream& operator<<(std::ostream& out, const Vertex v) {

		out << v.vertex_name; 

		return out;
	}

	void setPath(Object& node) {
		path = node; 
	}

	void setPath(Object&& node) {
		path = node;
	}

	Object getPath() const {
		return path; 
	}

	void setDistance(const double new_dist) {
		distance = new_dist; 
	}

	double get_distance() const {
		return distance; 
	}

	void set_topNum(int topological_number) {
		topNum = topological_number; 
	}

	int get_topNum() const {
		return topNum; 
	}

	void set_indegree(int new_indegree) {
		indegree = new_indegree; 
	}

	int get_indegree() const {
		return indegree; 
	}

	void set_minPathCost(double cost) {
		minPathCost = cost; 
	}

	double get_minPathCost() const {
		return minPathCost; 
	}


private: 

	// Contains a list of the adjacent vertices, and their weights
	vector<pair<Object, double>> adjacent_vertices; 

	// Name of this Vertex 
	Object vertex_name; 

	// Stores the last vertex that has accessed and updated this vertex in a shortest path algorithm 
	Object path; 

	// Stores the distance of the vertex in a shortest path algorithm
	double distance; 

	// Stores the cost of the minimum path of this Vertex in a shortest path algorithm 
	double minPathCost;

	// Stores the topological number of the Vertex in a Topological sort 
	int topNum; 
	
	// Stores the indegree of the vertex during a topological sort 
	int indegree; 
		
};
