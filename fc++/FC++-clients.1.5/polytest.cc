// This file may produce warning diagnostics, but that's ok.
#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::ostream;
using std::cout;
using std::endl;

struct Foo {
   int x;
   explicit Foo( int y ) : x(y) {}
};
ostream& operator<<( ostream& o, Foo f ) {
   o << "Foo: " << f.x;
   return o;
}

struct Bar {
   int x;
   Bar( Foo y ) : x(y.x) {}
};
ostream& operator<<( ostream& o, Bar f ) {
   o << "Bar: " << f.x;
   return o;
}

int main() {
   Fun0<int> fi = makeFun0( const_( 48 ) );
   cout << fi() << " " << fi() << endl;     // prints "48 48"

   int i = 5;
   double d = 3;
   char c = 2;

   d = i;      // legal
   i = c;      // legal
  
   Fun0<double> fd = fi;
   cout << fd() << " " << fd() << endl;
   
   c = i;                    // legal?  yes... therefore so is next line
   Fun0<char> fc = fi;
   cout << fc() << " " << fc() << endl;     // prints "0 0"
   
   c = d;      // legal?  yes... (with a warning), so this is legal too
   fc = fd;   
   cout << fc() << " " << fc() << endl;

//   Foo foo = i;           // legal?  no, so neither is this
//   Fun0<Foo> ff = fi;
   Fun0<Foo> ff = explicit_convert0<Foo>( fi );     // but this is
   cout << ff() << " " << ff() << endl;

   Foo foo = Foo(i);           // this is legal, but...
//   Fun0<Foo> ff = fi;          // ...this is unsupported

   Bar bar = foo;         // legal
   Fun0<Bar> fb = ff;
   cout << fb() << " " << fb() << endl;

   // And now for the kicker, right?
//   bar = Bar(i);      // illegal
//   fb = fi;           // also illegal, though g++ has crummy error message

   return 0;
}

