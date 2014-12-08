#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include "prelude.h"

using namespace fcpp;

using namespace std;

int main() {
   // prints 5 6 7 8 9 10
   List<int> l = enumFromTo( 1, 10 );
   List<int>::iterator i = find( l.begin(), l.end(), 5 );
   copy( i, l.end(), ostream_iterator<int>( cout, " " ) );
   cout << endl;

   // prints 5 6 7 8 9 10
   vector<int> v(6);
   copy( i, l.end(), v.begin() );
   List<int> list( v.begin(), v.end() );
   while( list ) {
      cout << head(list) << " ";
      list = tail(list);
   }
   cout << endl;

   return 0;
}

