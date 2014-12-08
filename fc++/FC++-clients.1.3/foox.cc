#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

struct Fact : public CFunType<int,int> {
   mutable int count;
   Fact() : count(0) {}
   int operator()( int x ) const {
      if( x==0 ) { cout << count << endl; return 1; }
      else { count++; return x * (*this)(x-1); }
   }
} fact;

int main() {
   cout << fact(5) << endl;
   cout << fact(5) << endl;

   Fact fa, fb;
   Fun1<int,int> f = makeFun1( fa );
   Fun1<int,int> g = makeFun1( fa );
   Fun1<int,int> h = makeFun1( fb );

   f(7);
   g(8);
   h(9);
   
   Fun1<int,int> i( h );
   i(2);

   return 0;
}
