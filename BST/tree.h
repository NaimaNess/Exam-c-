#ifndef __TREE__
#define __TREE__

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <node.h>
#include <iterator.h>

template < class T, class U >
class Tree {
  using Node = struct Node< T, U >;
 std::unique_ptr<Node> root = nullptr;
 Node * tail = nullptr;

 void kernel_balance ( Iterator< T, U > here, const std::vector<Node*>& nodes ); 
 
public:
 using Iterator = class Iterator< T, U >;
 using ConstIterator = class ConstIterator< T, U >;
 template < class ot, class ou >
    friend std::ostream& operator<< ( std::ostream&, Tree< ot, ou >& );
 Tree () = default;
 Tree ( const Tree & T_other );
 Tree& operator= ( const Tree & T_other ) {

    this->clear();
    auto tmp = T_other;
    (*this) = std::move( tmp );

    return *this;
    
  }
 Tree ( Tree&& T_other ) : root{ std::move( T_other.root ) },
			    tail{ std::move( T_other.tail ) } {}
 Tree& operator= ( Tree&& T_other ) {

    root = std::move( T_other.root );
    tail = std::move( T_other.tail );

    return *this;

  }
 ~Tree() noexcept = default;
 Iterator begin() { return Iterator{ tail }; }
 Iterator end() { return Iterator{ nullptr }; }
 Iterator top() { return Iterator{ root.get() }; }
 ConstIterator cbegin() const { return ConstIterator{ tail }; }
 ConstIterator cend() const { return ConstIterator{ nullptr }; }
 ConstIterator ctop() const { return ConstIterator{ root.get() }; }
 Iterator insert ( const T key, const U value, const bool substitute = false );

 void clear () {
    
    root.reset();
    
  }

 Iterator find ( const T key ) { return Iterator { root->find( key ) }; }
 ConstIterator find ( const T key ) const { return ConstIterator { root->find( key ) }; }
 U & operator[]( const T& key ) {

    Iterator n = find( key );
    
    if ( n == end() ) {
      
      n = insert( key, U{}, false );
      
    }
    
    return n->value();
    
  }
 struct key_not_found {
    
    std::string message;
    key_not_found( const std::string &s ) : message{s} {}
    
  };
 const U & operator[]( const T key ) const {
    
    ConstIterator n = find( key );
    
    if ( n == cend() )
      throw key_not_found{ "\nI'm sorry dave, I'm afraid I can't do that\n(key " +
	  std::to_string(key) +
	  " is not present.)"};
    
    return n->value();
    
  }
 
  
}; // end of class Tree 

template < class ot, class ou >
std::ostream& operator<< (std::ostream& os, Tree< ot, ou >& t) {
  
  Iterator< ot, ou > it = t.begin();
  
  if ( it.operator->() ) {
    Iterator< ot, ou > stop = t.end();
    while ( it != stop ) {
      os << it->key() << ":\t" << it->value() << "\n";
      ++it;
    }

  }
  else os << "Empty Tree!";

  return os;
  
}


#endif //__TREE__

