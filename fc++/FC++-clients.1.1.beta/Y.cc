/* Implementation of the Y-combinator. It's questionnable whether this
   should make into the library or it should just be left as an
   example file. */

#include <iostream>
#include "prelude.h"

/* A functoid to test the fixpoint operator. Implements the Fibonacci
   function. */
template <class X>
struct FibGenHelper : public CFunType<int, int> {
  X x;

  FibGenHelper (const X& in_x) : x(in_x) {}
  
  int operator () (const int& a) const {
    if (a == 0 || a == 1)
      return 1;
    else
      return x(a - 1) + x(a - 2);
  }
};


struct FibGen {
  template <class X>
  struct Sig : public FunType<X, FibGenHelper<X> > {};

  template <class X>
  FibGenHelper<X> operator () (const X& x) const {
    return FibGenHelper<X>(x);
  }
} fibgen;


/* A dummy routine, just to test the function transformer */
struct Testx : public CFunType<int, int> {
  int operator () (const int & n) const {
    return 3;
  }
} testx;




template <class F>
class YHelper {
private:
  F function;

public:
  template <class A>
  struct Sig : public 
  FunType<A, 
    typename RT<typename RT<F,ANY_TYPE>::ResultType,A>::ResultType> {};

  YHelper(const F& f) : function(f) {}

  template <class A> 
  Sig<A>::ResultType operator () (const A& a) const {
    //    return function(YHelper<F>(function))(a);
    return function(*this)(a);
  }
};


struct Y {
  template <class F>
  struct Sig : 
    public FunType<F, YHelper<F> > {};

  template <class F>
  typename Sig<F>::ResultType operator () (const F& f) const {
    return YHelper<F>(f);
  }
} y;




int main () {
  cout << 
    "Testing the function transformer." << endl <<
    "Should return 1, 6, 6, actually returned: " <<
    fibgen(testx)(1) << ", " << 
    fibgen(testx)(2) << ", " << 
    fibgen(testx)(3) << "."  <<
    endl;


  cout <<
    "Testing the Y combinator." << endl <<
    "Should return a finonacci sequence, actually returned: ";
  for (int i = 0; i < 10; i++)
    cout << y(fibgen)(i) << " ";
  cout << endl;
}
