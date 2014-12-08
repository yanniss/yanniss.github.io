#include <typeinfo>
#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

struct Apply {
   template <class F, class X, class Y>
   struct Sig : public FunType<F,X,Y,
                               typename F::template Sig<X,Y>::ResultType> {};

   template <class F, class X, class Y>
   typename F::template Sig<X,Y>::ResultType
   operator()( F f, X x, Y y ) {
      return f(x,y);
   }
} apply;
   
int main() {
   cout << typeid(apply( makePair, 3, 'c' )).name() << endl;
}
