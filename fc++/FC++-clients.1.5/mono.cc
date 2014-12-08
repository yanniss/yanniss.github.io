#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

class Foo {};

int main() {
   List<int> l = enumFrom(49);
   cout << monomorphize1<List<int>,int>(head)( l ) << endl;

   cout << monomorphize1<List<int>,char>(head)( l ) << endl;

   // these are illegal (rightly so)
   //cout << monomorphize1<List<char>,char>(head)( l ) << endl;
   //cout << monomorphize1<List<Foo>,Foo>(head)( l ) << endl;

   Fun1<int,int> f= makeFun1( monomorphize1<int,int>(inc) );
   cout << f(49) << endl;
   Fun1<char,char> g= makeFun1( monomorphize1<char,char>(inc) );
   cout << g(49) << endl;
   Fun1<int,char> h= makeFun1( monomorphize1<int,char>(inc) );
   cout << h(49) << endl;

   Fun2<int,int,bool> ff= makeFun2( monomorphize2<int,int,bool>(Greater()) );
   cout << ff( 3, 4 ) << endl;

   List<int> li = take( 2, l );
   Fun2<int,int,int> fp= makeFun2( monomorphize2<int,int,int>(Plus()) );
   int sum=monomorphize3<Fun2<int,int,int>,int,List<int>,int>( foldr )(fp,0,li);
   cout << "sum is " << sum << endl;

   int s = monomorphize2<int,List<int>,int>(curry3(foldr,Plus())) (0,li);
   cout << "sum is " << s << endl;

   s = bind2of3(foldr,0) (Plus(),li);
   cout << "sum is " << s << endl;

   s = bind3of3(foldr,li) (Plus(),0);
   cout << "sum is " << s << endl;

   return 0;
}

