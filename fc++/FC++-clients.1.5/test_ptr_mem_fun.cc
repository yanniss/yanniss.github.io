#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

class Foo {
   int x;
public:
   Foo() : x(0) {}
   int f( int y ) const { return x+y; }
   int g( int y ) { return x+=y; }
   int h( int a, int b ) const { return x+a+b; }
   int i( int a, int& b ) const { return b=x+a+b; }
};

int main() {
   Foo f;
   cout << ptr_to_fun( &Foo::f )( &f, 3 ) << endl;
   cout << ptr_to_fun( &Foo::g )( &f, 3 ) << endl;
   cout << ptr_to_fun( &Foo::g )( &f, 3 ) << endl;
   cout << ptr_to_fun( &Foo::h )( &f, 3, 4 ) << endl;
//   cout << ptr_to_fun( &Foo::i )( &f, 3, 4 ) << endl;

   const Foo cf;
   cout << ptr_to_fun( &Foo::f )( &cf, 3 ) << endl;
//   cout << ptr_to_fun( &Foo::g )( &cf, 3 ) << endl;
//   cout << ptr_to_fun( &Foo::g )( &cf, 3 ) << endl;
}
