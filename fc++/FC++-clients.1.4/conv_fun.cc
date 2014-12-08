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

struct fun_from_A_to_Y : public CFunType<A*,Y*> {
   Y* operator()( A* ) const { return new Y; }
};

Y* return_y() { return new Y; }
Y* ayy(A*,A*) { return new Y; }

int main() {
   Fun1<A*,Y*> fay = makeFun1( fun_from_A_to_Y() );
   Fun1<B*,X*> fbx = fay;          // subtype polymorphism

   Fun1<int,int> add3 = fcpp::plus(3);   // implicit direct->indirect

   Fun0<Y*> fy = ptr_to_fun(&return_y);
   Fun0<X*> fx = fy;

   Fun2<A*,A*,Y*> fayy = ptr_to_fun(&ayy);
   Fun2<B*,B*,X*> fbbx = fayy;
   fbx = fayy(new B);

   //Fun0<int> foo = 3;   // error message sucks
}
