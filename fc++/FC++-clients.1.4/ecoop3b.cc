#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

class Counter {
   int x;
   friend class CounterIncrement;
   friend class CounterValue;
public:
   Counter();
   Fun0<void> increment;
   Fun0<int> value;
};

class CounterIncrement : public CFunType<void> {
   Counter* this_;
public:
   CounterIncrement( Counter* c ) : this_(c) {}
   void operator()() const { this_->x++; }
}; 

class CounterValue : public CFunType<int> {
   Counter* this_;
public:
   CounterValue( Counter* c ) : this_(c) {}
   int operator()() const { return this_->x; }
}; 

Counter::Counter() : x(0), 
                     increment(makeFun0(CounterIncrement(this))),
                     value(makeFun0(CounterValue(this))) {}

int main() {
   Counter a, b;
   cout << a.value() << " " << b.value() << endl;
   a.increment();
   a.increment();
   b.increment();
   cout << a.value() << " " << b.value() << endl;
}

