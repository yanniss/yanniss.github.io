#include <iostream>
#include "prelude.h"
#include "ref_count.h"

// A <-- B      X <-- Y

struct A {
   virtual void f() const =0;
};

struct B : public A {
   void f() const { cout << "B::f()" << endl; }
};

struct X {
   virtual void g() const =0;
};

struct Y : public X {
   void g() const { cout << "Y::g()" << endl; }
};

struct fun_from_A_to_Y : public CFunType<A*,Y*> {
   Y* operator()( A* ) const { return new Y; }
};

int main() {
   Fun1<A*,Y*> fay = makeFun1( fun_from_A_to_Y() );
   Fun1<B*,X*> fbx = fay;   // woohoo!
   fbx( new B )->g();

   //Fun1<A*,X*> oops = fbx;
   Fun1<A*,X*> oops = explicit_convert1<A*,X*>(fbx);
//   Fun1<B*,Y*> crap = fbx;

   return 0;
}
