#include <iostream>
#include "prelude.h"

int main() {
   List<int> l = enumFrom( 1 );
   l = compose( tail, tail )( l );
   l = take( 3, l );
   
   while( l ) {
      cout << head(l) << " ";
      l = tail(l);
   }
   cout << endl;
   return 0;
}
   
