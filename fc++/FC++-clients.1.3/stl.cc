#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

int main() {
   using std::plus;
   List<int> l = enumFrom(1);
   l = map( stl_to_fun1( bind1st(plus<int>(),1) ), l );
   cout << head(l) << " " << head(tail(l)) << endl;

   l = enumFromTo(1,3);
   cout << foldr( stl_to_fun2( plus<int>() ), 0, l ) << endl;

   return 0;
}

