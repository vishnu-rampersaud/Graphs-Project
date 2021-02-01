/*
	Author: Vishnu Rampersaud 

	binary_heap.h header file for the binary heap class 
	This class is used to implement a priority queue for Dijkstra's algorithm. 

	I added the decreaseKey function. All other code below was provided by Professor Stamos, 
	and sourced from Data Structures and Algorithm Analysis in C++, 4th edition, by Mark Allen Weiss.
*/
#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "dsexceptions.h"
#include <vector>
using namespace std;

// BinaryHeap class
//
// CONSTRUCTION: with an optional capacity (that defaults to 100)
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( minItem )   --> Remove (and optionally return) smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void decreaseKey(node) --> Decrease the priority of a node in the heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

// BinaryHeap class 
// Implements a binary heap which can be used for priority queue purposes
template <typename Comparable>
class BinaryHeap
{
  public:
	  
	// Constructor that allows you to set the capcity for the heap array 
    explicit BinaryHeap( int capacity = 100 )
      : array( capacity + 1 ), currentSize{ 0 }
    {
    }

	// Constructor that builds a binary heap out of an array 
    explicit BinaryHeap( const vector<Comparable> & items )
      : array( items.size( ) + 10 ), currentSize{ items.size( ) }
    {
        for( int i = 0; i < items.size( ); ++i )
            array[ i + 1 ] = items[ i ];
        buildHeap( );
    }
	
	// Checks to see if the heap is empty 
    bool isEmpty( ) const
      { return currentSize == 0; }

    /**
     * Find the smallest item in the priority queue.
     * Return the smallest item, or throw Underflow if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException{ };
        return array[ 1 ];
    }

	/*
	* Decreases the priority of an item in the heap 
	* Percolates up to restore order to the heap 
	*/
	void decreaseKey(Comparable& node)
	{
		// variable to hold the index of the desired item
		int x; 

		// find the location of the item and update its value 
		for (int i = 0; i < array.size(); i++) {
			if (array[i] == node) {
				array[i] = node; 
				x = i; 
				break; 
			}
			else if (i == array.size() - 1) {
				std::cout << "node not found in heap; Cannot change priority" << std::endl;
				return; 
			}
		}

		// Percolate up 
		int hole = x;	// set hole to the updated item location/ index 
		Comparable copy = array[ x ];

		array[0] = std::move(copy);
		for (; array[ x ] < array[hole / 2]; hole /= 2)
			array[hole] = std::move(array[hole / 2]);
		array[hole] = std::move(array[0]);
	}
    
    /**
     * Insert item x, allowing duplicates.
     */
    void insert( const Comparable & x )
    {
        if( currentSize == array.size( ) - 1 )
            array.resize( array.size( ) * 2 );

            // Percolate up
        int hole = ++currentSize;
        Comparable copy = x;
        
        array[ 0 ] = std::move( copy );
        for( ; x < array[ hole / 2 ]; hole /= 2 )
            array[ hole ] = std::move( array[ hole / 2 ] );
        array[ hole ] = std::move( array[ 0 ] );
    }
    

    /**
     * Insert item x, allowing duplicates.
     */
    void insert( Comparable && x )
    {
        if( currentSize == array.size( ) - 1 )
            array.resize( array.size( ) * 2 );

            // Percolate up
        int hole = ++currentSize;
        for( ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 )
            array[ hole ] = std::move( array[ hole / 2 ] );
        array[ hole ] = std::move( x );
    }
    
    /**
     * Remove the minimum item.
     * Throws UnderflowException if empty.
     */
    void deleteMin( )
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        array[ 1 ] = std::move( array[ currentSize-- ] );
        percolateDown( 1 );
    }

    /**
     * Remove the minimum item and place it in minItem.
     * Throws Underflow if empty.
     */
    void deleteMin( Comparable & minItem )
    {
        if( isEmpty( ) )
            throw UnderflowException{ };

        minItem = std::move( array[ 1 ] );
        array[ 1 ] = std::move( array[ currentSize-- ] );
        percolateDown( 1 );
    }

    void makeEmpty( )
      { currentSize = 0; }

  private:
    int                currentSize;  // Number of elements in heap
    vector<Comparable> array;        // The heap array

    /**
     * Establish heap order property from an arbitrary
     * arrangement of items. Runs in linear time.
     */
    void buildHeap( )
    {
        for( int i = currentSize / 2; i > 0; --i )
            percolateDown( i );
    }

    /**
     * Internal method to percolate down in the heap.
     * hole is the index at which the percolate begins.
     */
    void percolateDown( int hole )
    {
        int child;
        Comparable tmp = std::move( array[ hole ] );

        for( ; hole * 2 <= currentSize; hole = child )
        {
            child = hole * 2;
            if( child != currentSize && array[ child + 1 ] < array[ child ] )
                ++child;
            if( array[ child ] < tmp )
                array[ hole ] = std::move( array[ child ] );
            else
                break;
        }
        array[ hole ] = std::move( tmp );
    }
};

#endif
