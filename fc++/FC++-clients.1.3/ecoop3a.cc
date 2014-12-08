#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

class Counter {
   int x;
public:
   Counter() : x(0) {}
   void increment() { x++; }
   int value() const { return x; }
};

int main() {
   Counter a, b;
   cout << a.value() << " " << b.value() << endl;
   a.increment();
   a.increment();
   b.increment();
   cout << a.value() << " " << b.value() << endl;
}

