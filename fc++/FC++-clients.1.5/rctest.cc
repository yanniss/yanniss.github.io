#include <iostream>
#include "ref_count.h"

namespace fcpp {}
using namespace fcpp;

using std::cout;
using std::endl;

// A <-- B      X <-- Y

struct A {
   int x;
   A( int y ) : x(y) {}
   virtual void f() const { cout << x << " A::f()" << endl; }
   virtual ~A()           { cout << x << " A's destructor" << endl; }
};

struct B : public A {
   B( int y ) : A(y) {}
   void f() const { cout << x << " B::f()" << endl; }
   ~B()           { cout << x << " B's destructor" << endl; }
};

namespace fcpp {}
using namespace fcpp;

int main() {
   Ref<A> a;
   {
      Ref<B> b( new B(2) );
      a = b;
      //b = a;     // illegal
   }
   a->f();
   Ref<B> b2 = ref_dynamic_cast<B>( a );
   b2->f();

   cout << "----------------" << endl;

   a = Ref<A>( new A(1) );
   a->f();
   b2->f();

   cout << "----------------" << endl;

   b2 = ref_dynamic_cast<B>( a );
   if( !b2 ) {
      cout << "Good, should be NULL, and #2 should be dead already" << endl;
   }

   return 0;
}
