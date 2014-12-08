#include "prelude.h"
#include <iostream>
#include <typeinfo>

using namespace fcpp;

using std::cout;
using std::endl;

int main() {
   using fcpp::greater;
   using fcpp::minus;
   using fcpp::plus;
   cout << minus(7,4) << endl;
   cout << minus(7)(4) << endl;

   cout << minus(_,4)(7) << endl;
   cout << minus(7,_)(4) << endl;

   cout << until( greater(_,9), inc, 1 ) << endl;
   cout << until( greater(_,9), inc )(1) << endl;
   cout << until( greater(_,9) )(inc,1) << endl;

   cout << until( _, inc, _ )( greater(_,9), 1) << endl;
   cout << until( _, inc )( greater(_,9), 1) << endl;
   cout << until( _, _, 1 )( greater(_,9), inc) << endl;
   cout << until( greater(_,9), _, _ )(inc,1) << endl;

   cout << until( _, inc, 1 )(greater(_,9)) << endl;
   cout << until( greater(_,9), _, 1 )(inc) << endl;
   cout << until( greater(_,9), inc, _ )(1) << endl;


   cout << until( greater(_,9) )(inc)(1) << endl;
   cout << until( greater(_,9), _ )(inc)(1) << endl;
   cout << until( greater(_,9), _, _ )(inc)(1) << endl;

   cout << "test" << endl;

   cout << until( _, inc, _ )(greater(_,9) )(1) << endl;
   cout << until( _, _, 1 )(greater(_,9) )(inc) << endl;




   //Fun2<int,int,int> f = monomorphize2<int,int,int>( minus );
   Fun2<int,int,int> f = makeFun2(monomorphize2<int,int,int>( minus ));


   
   cout << f(_,4)(5) << endl;
   Fun1<int,int> sub4 = f(_,4);
   cout << sub4(5) << endl;

   Fun1<int,int> decrement = minus(_,1);
   Fun1<int,int> in = inc;
   cout << decrement(9) << ' ' << in(7) << endl;

   Fun2<int,int,int> z = minus;
   cout << z(7,1) << endl;
   cout << z(7,_)(1) << endl;
   cout << minus(7,_)(1) << endl;

   //makeFun2(minus);
////////////////////////////////////////////////////////////////////// 
   cout << hUncurry(plus)( makePair(3,4) ) << endl;
   cout << hCurry(hUncurry(plus))(3,4) << endl;
}

