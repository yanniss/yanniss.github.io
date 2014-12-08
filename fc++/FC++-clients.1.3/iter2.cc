#include <vector>
#include <iostream>
#include "prelude.h"

using namespace fcpp;
using namespace std;

int main() {
   List<int> list;

   {
      vector<int> v;
      v.push_back(1);
      v.push_back(2);
      v.push_back(3);
      list = List<int>( v.begin(), v.end() );

      // Necessary to force evaluation, else dangling reference
      length( list );
   }
   while( list ) {
      cout << list.head() << " ";
      list = list.tail();
   }
   cout << endl;
}

