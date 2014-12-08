#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

void print( List<int> l ) {
   while( !null(l) ) {
      cout << head(l) << ' ';
      l = tail(l);
   }
   cout << endl;
}

int main() {
   using fcpp::plus;     // only needed for g++2.95.2
   using fcpp::minus;

   // Create the list 1..5
   List<int> integers = take( 5, enumFrom(1) );

   print( integers );

   // These two do the same thing (2..6)
   print( map( inc, integers ) );
   print( map( plus(1), integers ) );

   // These two do the same thing (3..7)
   print( map( compose(inc,inc), integers ) );
   print( map( plus(2), integers ) );

   // These three do the same thing (0..-4)
   print( map( bind1of2(minus,1), integers ) );
   print( map( minus(1), integers ) );
   print( map( minus(1,_), integers ) );

   // These two do the same thing (0..4)
   print( map( bind2of2(minus,1), integers ) );
   print( map( minus(_,1), integers ) );

   // Of course, map() is curryable too...
   print( map( plus(2) )( integers ) );
   print( map( _, integers )( plus(2) ) );
}
