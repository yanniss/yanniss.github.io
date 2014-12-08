#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

bool prime( int x ) {
   if( x<2 ) return false;
   for( int i=2; i<x; i++ )
      if( x%i == 0 )
         return false;
   return true;
}

void print( List<int> l ) {
   while( !null(l) ) {
      cout << head(l) << ' ';
      l = tail(l);
   }
   cout << endl;
}

int main() {
   List<int> integers = enumFrom(1);
   print( take(5,integers) );

   List<int> evens = filter( even, integers );
   print( take(5,evens) );

   List<int> primes = filter( ptr_to_fun(&prime), integers );
   print( take(5,primes) );

}
