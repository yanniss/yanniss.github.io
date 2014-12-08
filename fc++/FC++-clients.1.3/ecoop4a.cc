#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cin;
using std::cout;
using std::endl;

class Counter {
   int count;
public:
   Counter() : count(0)             {}
   int get_count() const            { return count; }
   void increment_count()           { ++count; }
   void increment_count_by( int x ) { count += x; }
};

class Command {
public:
   virtual void execute()=0;
};

class ShowCommand : public Command {
   Counter* c;
public:
   ShowCommand( Counter* cc ) : c(cc) {}
   void execute() {
      cout << "The counter is " << c->get_count() << endl;
   }
};

class IncCommand : public Command {
   Counter* c;
public:
   IncCommand( Counter* cc ) : c(cc) {}
   void execute() {
      c->increment_count();
   }
};

class IncByNCommand : public Command {
   Counter* c;
   int n;
public:
   IncByNCommand( Counter* cc, int nn ) : c(cc), n(nn) {}
   void execute() {
      c->increment_count_by(n);
   }
};

int main() {
   Counter c;
   Command* menu_actions[] = {
      new ShowCommand(&c),
      new IncCommand(&c),
      new IncByNCommand(&c,5)
   };
   while( true ) {
      cout << "***Menu: (1) show count (2) increment "
           << "(3) inc. by 5 (4) quit" << endl;
      cout << "Select option: ";
      int choice;
      cin >> choice;
      if( choice==1 || choice==2 || choice==3 )
         menu_actions[choice-1]->execute();
      else
         break;
   }
}

