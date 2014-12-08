#include "prelude.h"
#include <iostream>

using namespace fcpp;

using std::cout;
using std::endl;

struct Foo {};

struct O {
   int aa;
   O( int a ) : aa(a) {}
   int cf( int x ) const { return x+1; }
   int f( int x ) { return ++aa + x; }
};

int main() {
   O o(1);
   cout << ptr_to_fun( &O::cf )( &o, 1 ) << endl;   // should be 2
   cout << ptr_to_fun( &O::f  )( &o, 1 ) << endl;   // should be 3
   cout << ptr_to_fun( &O::f  )( &o, 1 ) << endl;   // should be 4
   cout << ptr_to_fun( &O::cf )( &o, 1 ) << endl;   // should be 2
   cout << endl;

   const O p(1);
   cout << ptr_to_fun( &O::cf )( &p, 1 ) << endl;   // should be 2
   //cout << ptr_to_fun( &O::f  )( &p, 1 ) << endl;  // good error msg
   cout << endl;

   Ref<O> r( new O(3) );
   cout << ptr_to_fun( &O::cf )( r, 1 ) << endl;   // should be 2
   cout << ptr_to_fun( &O::f  )( r, 1 ) << endl;   // should be 5
   cout << ptr_to_fun( &O::f  )( r, 1 ) << endl;   // should be 6
   cout << ptr_to_fun( &O::cf )( r, 1 ) << endl;   // should be 2
   cout << endl;

   Ref<const O> s( new O(3) );
   cout << ptr_to_fun( &O::cf )( s, 1 ) << endl;   // should be 2
   //cout << ptr_to_fun( &O::f  )( s, 1 ) << endl;  // good error msg
   cout << endl;

   //cout << ptr_to_fun( &O::cf )( Foo(), 1 ) << endl; 
   //cout << ptr_to_fun( &O::f )( Foo(), 1 ) << endl; 
}

