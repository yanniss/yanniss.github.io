#include "prelude.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace fcpp;

int f(int x) { return 2*x + 1; }
int g(int x) { return 3*x + 1; }
int h(int x) { return 5*x + 1; }

int main() {
   cout << f(g(h(1))) << endl;   
   cout << f(g(h(2))) << endl;   
   cout << compose( ptr_to_fun(&f), 
                    compose( ptr_to_fun(&g), ptr_to_fun(&h) ) )(1) << endl;
   cout << compose( ptr_to_fun(&f), 
                    compose( ptr_to_fun(&g), ptr_to_fun(&h) ) )(2) << endl;
   cout << compose( compose( ptr_to_fun(&f), 
                             ptr_to_fun(&g)), ptr_to_fun(&h) )(2) << endl;
   cout << (ptr_to_fun(&f) ^of^ ptr_to_fun(&g) ^of^ ptr_to_fun(&h)) (1) << endl;
   cout << (ptr_to_fun(&f) ^of^ ptr_to_fun(&g) ^of^ ptr_to_fun(&h)) (2) << endl;
   cout << (&f ^of^ &g ^of^ &h) (1) << endl;
   cout << (&f ^of^ &g ^of^ &h) (2) << endl;
   cout << (equal(69) ^of^ &f ^of^ &g ^of^ ptr_to_fun(&h)) (2) << endl;
}
