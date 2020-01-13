#ifndef __ITERATOR__
#define __ITERATOR__

#include <memory>
#include <node.h>

template < class T, class U >
class Iterator {

 using Node = struct Node<T, U>;

 Node * current;
  
public:

 Iterator () noexcept = default;

 Iterator( Node * n ) : current{ n } {}
 Node& operator*() { return *current; }
 Node* operator->() const { return current;  }
 Iterator& operator++() {

    if ( current ) {
      if ( current->right )
	current = current->right->leftmost() ;
      else
	current = current->parent;
    }
      
    return *this;
    
  }
 Iterator operator++( int ) {
    
    Iterator it{*this};
    
    ++(*this);
    
    return it;
    
  }
 bool operator==(const Iterator& other) { return current == other.current; }
 bool operator!=(const Iterator& other) { return !( *this == other ); }
  
}; // end of class Iterator
template < class T, class U >
  class ConstIterator : public Iterator<T, U> {
 using Node = struct Node<T, U>;
    
 public:
 using parent = Iterator<T, U>;
  
  using parent::Iterator;
 const Node* operator->() const { return parent::operator->();  }
 const Node& operator*() const { return parent::operator*(); }
  
  using parent::operator==;
  
  using parent::operator!=;
  
}; // end of class ConstIterator

#endif //__ITERATOR__
