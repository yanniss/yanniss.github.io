#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

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

struct fun_from_A_to_Y : public CFunType<Ref<A>,Ref<Y> > {
   Ref<Y> operator()( Ref<A> ) const { return Ref<Y>(new Y); }
};

Y* foo( A*, A*, A* ) { return 0; }

int main() {
   Fun1<Ref<A>,Ref<Y> > fay = makeFun1( fun_from_A_to_Y() );
   Fun1<Ref<B>,Ref<X> > fbx = fay;   // woohoo!
   fbx( Ref<B>(new B) )->g();

   Ref<B> rb;
   Ref<const B> rcb = rb;
   //Ref<void> rv = rb;       // unsafe
   if( rcb == rb )
      cout << "yay" << endl;
   //rb = rcb;

   //Fun1<A*,X*> oops = fbx;
   //Fun1<A*,X*> oops = explicit_convert1<A*,X*>(fbx);
   //Fun1<B*,Y*> crap = fbx;

   Fun3<A*,A*,A*,Y*> faaay = ptr_to_fun(&foo);
   Fun3<B*,B*,B*,X*> fbbbx = faaay;
   B* bn = 0;
   X* xxx = fbbbx(bn)(bn)(bn);
   if( !xxx )
      cout << "yay" << endl;
}
