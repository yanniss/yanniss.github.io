#include <iostream>
#include "prelude.h"

using std::cout;
using std::endl;

class Foo {};

template <class T>
void f( T t ) {
   cout << "hi";
}

int main() {
   using std::plus;
   List<int> l = enumFrom(1);
   l = map( stl_to_fun1( bind1st(plus<int>(),1) ), l );
   cout << head(l) << " " << head(tail(l)) << endl;

   l = enumFromTo(1,1);
   cout << foldr( stl_to_fun2( plus<int>() ), 0, l ) << endl;

   l = cat( l, l );   // should succeed now
   cout << head(l) << " " << head(tail(l)) << endl;
 
   Reverse()(l);

   return 0;
}

