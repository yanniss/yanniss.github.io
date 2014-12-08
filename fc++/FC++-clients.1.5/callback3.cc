// This example demonstrates using FC++ to facilitate callbacks.

#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include "prelude.h"

using namespace std;
using namespace fcpp;

void C_reg_callback( void (*)(void*), void* ); // Given C interface
void fcpp_register_callback( const Fun0<void>& );   // FC++ wrapper
void trigger_callbacks();          // For convenience of exposition

void f() { cout << "f()" << endl; }
void g(int x) { cout << "g(" << x << ")" << endl; }

struct Foo {
   int id;
   void f() { cout << "Foo(" << id << ")::f()" << endl; }
   void g(int x) { cout << "Foo(" << id << ")::g(" << x << ")" << endl; }
};

int main() {
   Foo foo1;   foo1.id = 1;
   Foo foo2;   foo2.id = 2;

   // Register "f()"
   fcpp_register_callback( ptr_to_fun(&f) );

   // Register "g(3)"
   fcpp_register_callback( curry1(ptr_to_fun(&g),3) );

   // Register "foo1.f()"
   fcpp_register_callback( curry1(ptr_to_fun(&Foo::f),&foo1) );

   // Register "foo2.g(4)"
   fcpp_register_callback( curry2(ptr_to_fun(&Foo::g),&foo2,4) );

   trigger_callbacks();
}

typedef pair<void (*)(void*),void*> Thunk;
vector<Thunk> thunks;

void C_reg_callback( void (*f)(void*), void* x ) {
   thunks.push_back( make_pair(f,x) );
}   
void trigger_callbacks() {
   for( vector<Thunk>::iterator i=thunks.begin(); i!=thunks.end(); ++i ) {
      void (*f)(void*) = i->first;
      void *x = i->second;
      f(x);
   }
}

void call_fun0( void* x ) {
   Fun0<void>* p = static_cast<Fun0<void>*>(x);
   (*p)();
}


void fcpp_register_callback( const Fun0<void>& f ) {
   // Note: need to store the callbacks somewhere, as they have state
   // and need a lifetime.  Use a list, because insertion doesn't move
   // elements, and so we have permanent pointers to them.  The list is
   // never freed; there's no way to unregister a callback.
   static list<Fun0<void> > callbacks;
   callbacks.push_back( f );
   C_reg_callback( call_fun0, & callbacks.back() );
}

