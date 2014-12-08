// This example demonstrates using FC++ to facilitate callbacks.

#include <iostream>
#include <vector>
#include "prelude.h"

using namespace fcpp;

using namespace std;
using namespace fcpp;

void f(int x) { cout << "f(" << x << ")" << endl; }

struct Foo {
   int id;
   void f(int x) { cout << "Foo(" << id << ")::f(" << x << ")" << endl; }
   void g(int x, int y) { 
      cout << "Foo(" << id << ")::g(" << x << "," << y << ")" << endl; 
   }
};

// There are a number of common situations (like the Observer pattern
// and Command pattern) where you may desire "callbacks"--pointers to
// other functionality that you invoke, despite not knowing exactly
// what's on the other end.  Indirect functoids in FC++ make callbacks
// trivial to express.  For example, we can easily 'broadcast an
// integer' to a number of functions.  The code below shows how to set
// up callbacks so we can broadcast 'x' so that these
//    f(x);
//    foo1.f(x);
//    foo2.g(x,3);
// all get called during the broadcast.  

int main() {
   Foo foo1;   foo1.id = 1;
   Foo foo2;   foo2.id = 2;

   typedef vector<Fun1<int,void> > CB;
   CB callbacks;

   // A normal function is ok
   callbacks.push_back( ptr_to_fun(&f) );

   // A member function is, too; we can bind the receiver object
   callbacks.push_back( ptr_to_fun(&Foo::f)(&foo1) );

   // Even functions with the 'wrong' signature can be easily adapted
   callbacks.push_back( ptr_to_fun(&Foo::g)(&foo2,_,3) );

   int x = 6;
   for( CB::iterator i=callbacks.begin(); i!=callbacks.end(); ++i )
      (*i)(x);

   // The output is:
   // f(6)
   // Foo(1)::f(6)
   // Foo(2)::g(6,3)
}

