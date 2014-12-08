#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

int main() {
   List<int> l = take( 5, enumFrom(1) );
   std::vector<int> v(5);
   std::copy( l.begin(), l.end(), v.begin() );
   std::reverse( v.begin(), v.end() );
   List<int> r( v.begin(), v.end() );
   assert( r == list_with(5,4,3,2,1) );
   cout << "ok" << endl;
}
