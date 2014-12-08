#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

// This test file is a horrible mess and I should really clean it up.

void print( int x ) {
   cout << x << endl;
}
void print2( int x, int y ) {
   cout << "(" << x << "," << y << ")" << endl;
}

struct Foo {
   Foo() { cout << "0-arg" << endl; }
   Foo(int) { cout << "1-arg" << endl; }
   Foo(int,int) { cout << "2-arg" << endl; }
   Foo(int,int,int) { cout << "3-arg" << endl; }
};

int main() {
   using fcpp::plus;
   using fcpp::compose;
   Fun1<int,void> pr = ptr_to_fun(&print);
   Fun1<int,void> pr3 = compose(pr,plus(10));
   before( curry1(pr,1), curry1(pr,2) )();
   before( curry1(pr,1), pr )(2);
   before( curry1(pr,2), before( curry1(pr,1), pr ) )(2);
   cout << before( noOp, const_(true) )() << endl;
   cout << before( noOp, before( noOp, const_(true) ) )() << endl;

   // h(f,g)(x,y)  = { f(x); g(y); }
   cout << "-----" << endl;
   compose( before(_,pr), curry2(curry1,pr) )(1)(2);
   //compose( before(_,pr), curry2(curry1,pr) )(1,2); // not uncurryable
   uncurry(compose( before(_,pr), curry2(curry1,pr) ))(1,2);
   uncurry(compose( before(_,pr3), curry2(curry1,pr) ))(1,2);

   int x = 10, y = 0;
   Fun2<int,int,void> f = ptr_to_fun(&print2);
   Fun1<int,int>      g = plus(1);
   Fun1<int,int>      h = plus(2);

   cout << "-----" << endl;
   // i(x,y) = f(g(x),h(y))
   compose(flip(compose)(h),compose(f,g))(x)(y);
   compose(compose(_,h),compose(f,g))(x)(y);

   // i(x) = f(g(x),h(x))
   duplicate(compose(flip(compose)(h),compose(f,g)))(x);
   duplicate(compose(compose(_,h),compose(f,g)))(x);
   Compose2()(f,g,h)(x);

   cout << "-----" << endl;
   List<int> l = listUntil( Never1(), plus(1), 1 );
   cout << at(l,0) << at(l,1) << at(l,2) << endl;
   l = listUntil( ignore(const_(false)), plus(1), 1 );
   cout << at(l,0) << at(l,1) << at(l,2) << endl;
   cout << ignore(plus)(1)(2)(3) << endl;
   cout << ignore(plus)(1,2,3) << endl;
   // ignore second arg
   cout << flip(ignore(length)) (list_with(1),4) << endl;
   // ignore third arg
   cout << uncurry(makeCurryable3(bind2and3of3)(ignore(plus))) (2,3,l) << endl;
   cout << "-----" << endl;
   cout << after(Const()(0),curry1(pr,3))() << endl;
   cout << after(plus,curry1(pr,3))(2,3) << endl;
   cout << "-----" << endl;
   Maybe<int> xx = Maybe<int>();
   Maybe<int> yy = Maybe<int>(3);
   if( !xx.is_nothing() ) 
      cout << xx.value() << endl; 
   else 
      cout << "nothing" << endl;
   if( !yy.is_nothing() ) 
      cout << yy.value() << endl; 
   else 
      cout << "nothing" << endl;
   cout << "-----" << endl;
   xx = NOTHING;
   yy = just(3);
   if( !xx.is_nothing() ) 
      cout << xx.value() << endl; 
   else 
      cout << "nothing" << endl;
   if( !yy.is_nothing() ) 
      cout << yy.value() << endl; 
   else 
      cout << "nothing" << endl;
   cout << "-----" << endl;
   before( noOp, construct0<Foo>() )();
   before( noOp, construct1<Foo>() )(1);
   before( noOp, construct2<Foo>() )(1,1);
   before( noOp, construct3<Foo>() )(1,1,1);
   cout << "-----" << endl;
   List<int> lll = repeat(0);
   cout << head( compose(force,tail)(lll) ) << endl;
   cout << head( compose(delay,compose(force,tail))(lll) ) << endl;
   int xyz = 3;
   cout << compose(dereference,addressOf)(xyz) << endl;
}
