#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

int f( int x ) {
   cout << "Executing f(" << x << ")" << endl;
   return x;
}

template <class T>
void test( List<T> l ) {
   cout << "Top of test(), about to get first element" << endl;
   at(l,0);
   cout << "After demanding first element" << endl;
   at(l,1);
   cout << "After demanding second element" << endl;
   at(l,2);
   cout << "After demanding third element" << endl;
}

int main() {
   // illustrates laziness of map()
   List<int> k=map(ptr_to_fun(&f), enumFromTo(0, 9));
   test(k);
cout << "----------------------------------" << endl;
   // illustrates how to make calls to map() totally lazy
   List<int> l=curry2(map,ptr_to_fun(&f),enumFromTo(0, 9));
   test(l);
cout << "----------------------------------" << endl;
   // illustrates eagerness of reverse()
   List<int> m=map(ptr_to_fun(&f), enumFromTo(0, 9));
   m=reverse(m);
   test(m);
cout << "----------------------------------" << endl;
   // easy way to change behavior: reorder map/reverse calls
   List<int> n=reverse(enumFromTo(0, 9));
   n=map(ptr_to_fun(&f),n);
   test(n);
cout << "----------------------------------" << endl;
   // hard/unusual way to change behavior: store thunks, not ints
   List<Fun0<int> > o = map( compose(construct1<Fun0<int> >(),
                                     curry2(curry1,ptr_to_fun(&f))), 
                             enumFromTo(0, 9) );
   o = reverse(o);
   test(o);
   cout << "About to _compute_ first element" << endl;
   at(o,0)();
   cout << "About to _compute_ second element" << endl;
   at(o,1)();
   cout << "About to _compute_ third element" << endl;
   at(o,2)();

   // illustrates general transform for data-laziness
   {
      cout << "---" << endl;
      int x = 3;
      x = f(x);
      x = f(x);
      cout << "foo" << endl;
      cout << x << endl;
   }

   {
      cout << "---" << endl;
      Fun0<int> x = const_(3);
      x = compose(ptr_to_fun(&f),x);
      x = compose(ptr_to_fun(&f),x);
      cout << "foo" << endl;
      cout << x() << endl;
   }
}
