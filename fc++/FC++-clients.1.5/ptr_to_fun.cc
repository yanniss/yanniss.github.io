#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

struct Foo {
   void f()       { cout << "f" << endl; }
   void g() const { cout << "g" << endl; }
   void h(int)       { cout << "h" << endl; }
   void i(int) const { cout << "i" << endl; }
   void j(int,int)       { cout << "j" << endl; }
   void k(int,int) const { cout << "k" << endl; }
};

int main() {
   Foo foo;
   const Foo cfoo = Foo();
   ptr_to_fun( &Foo::f )( &foo  );
   //ptr_to_fun( &Foo::f )( &cfoo );
   ptr_to_fun( &Foo::g )( &foo  );
   ptr_to_fun( &Foo::g )( &cfoo );
   ptr_to_fun( &Foo::h )( &foo , 1 );
   //ptr_to_fun( &Foo::h )( &cfoo, 1 );
   ptr_to_fun( &Foo::i )( &foo , 1 );
   ptr_to_fun( &Foo::i )( &cfoo, 1 );
   ptr_to_fun( &Foo::j )( &foo , 1, 1 );
   //ptr_to_fun( &Foo::j )( &cfoo, 1, 1 );
   ptr_to_fun( &Foo::k )( &foo , 1, 1 );
   ptr_to_fun( &Foo::k )( &cfoo, 1, 1 );

   Ref<Foo> pfoo( new Foo );
   Ref<const Foo> cpfoo( new Foo );
   ptr_to_fun( &Foo::f )( pfoo  );
   //ptr_to_fun( &Foo::f )( cpfoo );
   ptr_to_fun( &Foo::g )( pfoo  );
   ptr_to_fun( &Foo::g )( cpfoo );
   ptr_to_fun( &Foo::h )( pfoo , 1 );
   //ptr_to_fun( &Foo::h )( cpfoo, 1 );
   ptr_to_fun( &Foo::i )( pfoo , 1 );
   ptr_to_fun( &Foo::i )( cpfoo, 1 );
   ptr_to_fun( &Foo::j )( pfoo , 1, 1 );
   //ptr_to_fun( &Foo::j )( cpfoo, 1, 1 );
   ptr_to_fun( &Foo::k )( pfoo , 1, 1 );
   ptr_to_fun( &Foo::k )( cpfoo, 1, 1 );
}

