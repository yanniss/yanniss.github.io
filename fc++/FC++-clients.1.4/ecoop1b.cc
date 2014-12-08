#include <iostream>
#include <vector>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;
using std::vector;

class Subject {
   typedef vector< Fun0<void> > V;
   V observers;
public:
   void attach( Fun0<void> o ) {
      observers.push_back( o );
   }
   void notify() {
      for( V::iterator i=observers.begin(); i!=observers.end(); ++i ) {
         (*i)();
      }
   }
};

class ConcreteSubject : public Subject {
   int state;
public:
   ConcreteSubject() : state(0) {}
   int get_state() const { return state; }
   void inc() { state++; notify(); }
};

class ConcreteObserverA {
   ConcreteSubject& subject;
public:
   ConcreteObserverA( ConcreteSubject& s ) : subject(s) {
      s.attach( makeFun0( 
         curry( ptr_to_fun(&ConcreteObserverA::update), this ) ) );
   }
   void update() { 
      cout << "A's update: new state is " << subject.get_state() << endl;
   }
};

class ConcreteObserverB {
   ConcreteSubject& subject;
public:
   ConcreteObserverB( ConcreteSubject& s ) : subject(s) {
      s.attach( makeFun0( 
         curry( ptr_to_fun(&ConcreteObserverB::get_notification), this ) ) );
   }
   void get_notification() { 
      cout << "B's get_notification: new state is " 
           << subject.get_state() << endl;
   }
};

int main() {
   ConcreteSubject s;
   ConcreteObserverA ao(s);
   ConcreteObserverB bo(s);
   (void)ao;
   (void)bo;
   s.inc();
}

