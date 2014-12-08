#include <iostream>
#include <vector>
#include <string>
#include "prelude.h"

using namespace fcpp;

using std::vector;
using std::cout;
using std::endl;
using std::string;

// Demonstrates how FC++ can be used to help make an abstract callback
// interface

class EventFirer {
   typedef vector< Fun1<int,void> > VL;
   VL listeners;
public:
   void add_listener( const Fun1<int,void>& f ) {
      listeners.push_back( f );
   }
   void fire_event( int x ) {
      for( VL::iterator i=listeners.begin(); i!=listeners.end(); ++i ) {
         (*i)(x);
      }
   }
};

struct Foo {
   void blah( int x ) const {
      cout << "Foo.blah called with " << x << endl;
   }
};

struct Bar {
   void icky( int x, string s ) const {
      cout << "Bar.icky called with " << x << ": " << s << endl;
   }
};

int main() {
   Foo foo;
   Bar bar;
   EventFirer ef;

   string one = "one", two = "two";
   ef.add_listener( ptr_to_fun( &Foo::blah )( &foo )         );
   ef.add_listener( ptr_to_fun( &Bar::icky )( &bar, _, one ) );
   ef.add_listener( ptr_to_fun( &Bar::icky )( &bar, _, two ) );

   ef.fire_event( 3 );

   return 0;
}

