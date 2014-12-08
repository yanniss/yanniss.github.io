#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

// take 5 (map odd [1..])
// [True,False,True,False,True]

int main() {
   List<int> li = enumFrom( 1 );
   List<bool> lb = map( odd, li );
   lb = take( 5, lb );
   
   // or just
   
   lb = take( 5, map( odd, enumFrom(1) ) );

   while( lb ) {
      cout << head(lb) << " ";
      lb = tail(lb);
   }
   cout << endl;
   return 0;
}
   
