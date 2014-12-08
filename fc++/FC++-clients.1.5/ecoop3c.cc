#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

class Base {
   int x;
   friend class Value;
   friend class Show;
protected:
   Base( int xx, Fun0<int> vv, Fun0<void> ss ) : x(xx), value(vv), show(ss) {}
public:
   Base(int);
   Fun0<int>  const value;
   Fun0<void> const show;
};

class Show : public CFunType<void> {
   Base* this_;
public:
   Show( Base* c ) : this_(c) {}
   void operator()() const { 
      cout << "I am a base with value " << this_->x << endl; 
   }
}; 

class Value : public CFunType<int> {
   Base* this_;
public:
   Value( Base* c ) : this_(c) {}
   int operator()() const { return this_->x; }
}; 

Base::Base(int y) : x(y), 
                    value(makeFun0(Value(this))),
                    show(makeFun0(Show(this))) {}

class Derived : public Base {
   int y;
   friend class DShow;
public:
   Derived(int,int);
};

class DShow : public CFunType<void> {
   Derived* this_;
public:
   DShow( Derived* c ) : this_(c) {}
   void operator()() const { 
      cout << "I am a derived with value " 
           << this_->value() << ", " << this_->y << endl; 
   }
}; 

Derived::Derived( int a, int b )
   : Base(a, makeFun0(Value(this)), makeFun0(DShow(this))), y(b) {}
   
void f( Base& b ) {
   b.show();
}

int main() {
   Base b(3);
   Derived d(3,4);
   f(b);
   f(d);
}

