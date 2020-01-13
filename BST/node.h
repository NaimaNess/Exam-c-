#include <memory>
#include <utility>

#ifndef __BST__HH__
#define __BST__HH__

template< class T, class U >
struct Node {
 
	std::pair< T, U > content;
    	Node * parent = nullptr;

  	std::unique_ptr<Node> left = nullptr;
	std::unique_ptr<Node> right = nullptr;
	Node () : content{ std::pair<T, U>{} }, parent{ nullptr } {}
	Node ( T key, U value, Node * par = nullptr ): content{ std::pair<T,U>( key, value ) }, parent{ par } {}
	~Node() = default;
	const T& key() const { return content.first; }

  /// returns the value of the Node
  	const U& value() const { return content.second; }

  /// returns the value of the Node
  	U& value() { return content.second; }

  	Node * find ( const T key );
	Node * insert ( const T key, const U value, const bool substitute = false );
	Node * leftmost () {
		if ( left )
      			return left->leftmost();
    		else
      			return this;
		 }
 	void clear () {

    // reset everything on the left
    	left.reset();

    // reset everything on the right
   	right.reset();
    }
}; // end of class Node

template< class T, class U >
Node< T, U > * Node< T, U >::insert ( const T key, const U value, const bool sub ) {

  Node * n = nullptr;
  if ( sub && key == content.first ) {
    content.second = value;
    n = this;
  }
      
  if( key < content.first ) {
    if( left ) 
      n = left->insert( key, value, sub );
    else {
      left.reset( new Node { key, value, this } );
      n = left.get();
    }
  }

  if ( key > content.first ) {
    if ( right ) 
      n = right->insert( key, value, sub );
    else {
      right.reset( new Node{ key, value, parent } );
      n = right.get();
    }
  }

  return n;
      
}


template< class T, class U >
Node< T, U > * Node< T, U >::find ( const T key ) {
  
  if ( key == content.first ) 
    return this;

  if ( ( left ) && ( key < content.first ) ) 
    return left->find( key );

  if ( ( right ) && ( key > content.first ) ) 
    return right->find( key );

  return nullptr;

}


#endif //__NODE__
