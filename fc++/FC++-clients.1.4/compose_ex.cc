#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

int main() {
cout << "a" << endl;
   List<int> l = enumFrom( 1 );
cout << "b" << endl;
   l = compose( tail, tail )( l );
cout << "c" << endl;
   l = take( 3, l );
cout << "d" << endl;
   
   while( l ) {
      cout << head(l) << " ";
      l = tail(l);
   }
   cout << endl;
   return 0;
}
   
