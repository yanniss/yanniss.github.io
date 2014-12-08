#include <iostream>
#include "prelude.h"

int main() {
   List<int> li = NIL;
   List<bool> lb = NIL;
   
   while( lb ) {
      cout << head(lb) << " ";
      lb = tail(lb);
   }

   // or just
   lb = take( 5, map( odd, enumFrom(1) ) );

   while( lb ) {
      cout << head(lb) << " ";
      lb = tail(lb);
   }
   cout << endl;
   return 0;
}
   
