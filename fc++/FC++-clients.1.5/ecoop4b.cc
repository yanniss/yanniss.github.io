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

void show( Counter* c ) {
   cout << "The counter is " << c->get_count() << endl;
}

int main() {
   Counter c;
   Fun0<void> menu_actions[] = {
      makeFun0( curry( ptr_to_fun( &show ), &c ) ),
      makeFun0( curry( ptr_to_fun( &Counter::increment_count ), &c ) ),
      makeFun0( curry2( ptr_to_fun( &Counter::increment_count_by ), &c, 5 ) )
   };
   while( true ) {
      cout << "***Menu: (1) show count (2) increment "
           << "(3) inc. by 5 (4) quit" << endl;
      cout << "Select option: ";
      int choice;
      cin >> choice;
      if( choice==1 || choice==2 || choice==3 )
         menu_actions[choice-1]();
      else
         break;
   }
}

