#include <iostream>
#include <vector>
#include <algorithm>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

bool prime( int x ) {
   if( x<2 ) return false;
   for( int i=2; i<x; ++i )
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

template <class F>
void f( F x ) {
   cout << "sums are:" << endl;
   print( x(fcpp::plus) );
   cout << "differences are:" << endl;
   print( x(fcpp::minus) );
   cout << "products are:" << endl;
   print( x(fcpp::multiplies) );
}

int main() {
   List<int> integers = enumFrom(1);
   List<int> primesTo1000 = filter( ptr_to_fun(&prime), take(1000,integers) );
   List<int> primesBetween100and120 = 
      filter( fcpp::compose2( logicalAnd, fcpp::greater(_,100), fcpp::less(_,120) ), primesTo1000 );
   print( primesBetween100and120 );

   cout << until(ptr_to_fun(&prime),inc,114) << endl;  // first prime from 114

   List<int> l = list_with( 1, 4, 7, 3 );
   print( scanl1(fcpp::plus,l) );

   List<int> m = list_with( 4, 5, 6 );
   List<int> n = list_with( 1, 2, 3 );
   f( zipWith(_,m,n) );

   int a[5] = { 1, 2, 3, 4, 5 };
   std::vector<int> v( a, a+5 );
   std::transform( v.begin(), v.end(), v.begin(), fcpp::plus(3) );
   std::copy( v.begin(), v.end(), std::ostream_iterator<int>(std::cout," ") );
   std::cout << std::endl;
}
