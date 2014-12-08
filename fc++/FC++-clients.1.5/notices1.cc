#include <cassert>
#include <string>
#include <iostream>
#include "prelude.h"
using namespace fcpp;

using std::string;
using std::cout;
using std::endl;

int main() {
   int x=1, y=2, z=3;
   string s="foo", t="bar", u="qux";

   List<int> li = cons(x,cons(y,cons(z,NIL)));
   List<string> ls = cons(s,cons(t,cons(u,NIL)));

   assert( head(li) == 1 );
   // list_with() is shorthand to make small lists
   assert( tail(li) == list_with(2,3) );

   ls = compose(tail,tail)(ls);
   assert( head( ls ) == "qux" );
   assert( tail( ls ) == NIL );

   //cout << "Everything passed, now die." << endl;
   //assert( !(tail( ls ) == NIL) );
}
