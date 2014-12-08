#include <iostream>
#include "prelude.h"

using namespace fcpp;

// Tests List<T> where T has no default constructor

class Foo {
   Foo();
public:
   int x;
   Foo(int y) : x(y) { std::cout << "Foo(" << x << ")" << std::endl; }
};

int main() {
   std::cout << "a" << std::endl;
   List<Foo> l;
   std::cout << "a" << std::endl;
   l = cons( Foo(1), cons( Foo(2), NIL ) );
   std::cout << "a" << std::endl;
   l = tail(l);
   std::cout << "a" << std::endl;
   head(l);
}
