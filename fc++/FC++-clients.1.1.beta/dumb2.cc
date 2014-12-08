#include <iostream>
#include "prelude.h"

int main() {
   new int;
   new int();

   Fun1<int,bool> f = makeFun1( monomorphize1<int,bool>(even) );
   cout << f( 5 ) << endl;    // false
   f = makeFun1( monomorphize1<int,bool>(odd) );
   cout << f( 5 ) << endl;    // true

   return 0;
}

