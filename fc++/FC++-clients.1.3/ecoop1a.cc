#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

class Observer {
public:
   virtual void update() = 0; 
};

class Subject {
   typedef vector<Observer*> V;
   V observers;
public:
   void attach( Observer* o ) {
      observers.push_back( o );
   }
   void notify() {
      for( V::iterator i=observers.begin(); i!=observers.end(); ++i ) {
         (*i)->update();
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

class ConcreteObserverA : public Observer {
   ConcreteSubject& subject;
public:
   ConcreteObserverA( ConcreteSubject& s ) : subject(s) {
      s.attach( this );
   }
   void update() { 
      cout << "A's update: new state is " << subject.get_state() << endl;
   }
};

class ConcreteObserverB : public Observer {
   ConcreteSubject& subject;
public:
   ConcreteObserverB( ConcreteSubject& s ) : subject(s) {
      s.attach( this );
   }
   void update() { 
      cout << "B's update: new state is " << subject.get_state() << endl;
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


