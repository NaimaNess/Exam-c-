#include <chrono>
#include <fstream>
#include <memory>
#include <tree.h>
#include <map>

void fill_me ( Tree< int, double > &T ) { 
  bool sub = true;
  

  T.insert( 2, 7.3, sub  );
  T.insert( 5, 8.2 , sub );
  T.insert( 7, 1.1 , sub );
  T.insert( 8, 9.4, sub );
  T.insert( 11, 2.1, sub );
  T.insert( 7, 6.2, sub );
  T.insert( 1, 4.1, sub );
  T.insert( 9, 1.1, sub );
  T.insert( 10, 9.5, sub );
  T.insert( 3, 11.2, sub );
  T.insert( 4, 5.1, sub );
  T.insert( 15, 5.5, sub );
  T.insert( 13, 1.1, sub );
  T.insert( 14, 9.5, sub );
  T.insert( 7, 3.3, sub  );
}

int main() {

Tree< int, double > T{};
fill_me(T); 

//  T.balance();
  std::cout << T << std::endl;  
  T.clear();

 
  // run some tests:
  // test balance time
  return 0;
 
}
