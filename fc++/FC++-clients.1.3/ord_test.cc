#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

/* output should be
0110
0010
0000
1110
1000
0100
0010
0001
*/

int main() {
   List<int> a = list_with(1,2);
   List<int> b = list_with(1,2,3);
   List<int> c = list_with(2,3);

   cout << (a<a) << (a<b) << (a<c) << (a<NIL) << endl;
   cout << (b<a) << (b<b) << (b<c) << (b<NIL) << endl;
   cout << (c<a) << (c<b) << (c<c) << (c<NIL) << endl;
   cout << (NIL<a) << (NIL<b) << (NIL<c) << (NIL<NIL) << endl;

   cout << (a==a) << (a==b) << (a==c) << (a==NIL) << endl;
   cout << (b==a) << (b==b) << (b==c) << (b==NIL) << endl;
   cout << (c==a) << (c==b) << (c==c) << (c==NIL) << endl;
   cout << (NIL==a) << (NIL==b) << (NIL==c) << (NIL==NIL) << endl;
}
