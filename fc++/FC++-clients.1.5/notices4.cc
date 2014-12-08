#include <string>
#include <cassert>
#include "prelude.h"

using namespace fcpp;

int f( int x, int y ) {
   return 3*x + y;
}

class Foo {
   int n;
public:
   Foo( int nn ) : n(nn) {}
   int bar( int x, int y ) const {
      return n*x + y;
   }
};
 
int main() {
   assert( ptr_to_fun(&f)(3)(1) == 10 );

   Foo foo(3);
   assert( ptr_to_fun(&Foo::bar)(&foo,3)(1) == 10 );
}
