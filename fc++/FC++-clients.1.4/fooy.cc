#include <iostream>
#include <string>
#include "prelude.h"

using namespace fcpp;

using std::string;
using std::cout;
using std::endl;

struct Foo {
   template <class F>
   struct Sig : public FunType<F,typename F::ResultType> {};

   template <class F>
   typename F::ResultType operator()( const F& f ) const {
      typedef typename F::ResultType T;
      T x = T();
      return f(x);
   }
} foo;

int main() {
   string y = "yadda:";
   Fun1<bool,bool> n = logicalNot;
   Fun1<int,int> i = inc;
   Fun1<string,string> s = fcpp::plus(y);

   cout << foo(n) << endl;     // true
   cout << foo(i) << endl;     // 1
   cout << foo(s) << endl;     // "yadda:"
}
