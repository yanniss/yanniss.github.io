#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cin;
using std::cout;
using std::endl;

template <class Unary, class ArgThunk>
class thunkbinder1of1
: public CFunType<typename RT<Unary,ArgThunk>::ResultType> {
   Unary f;
   ArgThunk a;
public:
   thunkbinder1of1( const Unary& x, const ArgThunk& y ) : f(x), a(y) {}
   typename RT<Unary,ArgThunk>::ResultType operator()() const { return f(a()); }
};

// note to self; same as curry except for thunkbinder reference; can
// templatize it rather than have a 'new' class
struct CurryThunk1 {
  template <class Unary, class A1>
  struct Sig : public FunType<Unary, A1, thunkbinder1of1<Unary,A1> > {};

  template <class Unary, class A1>
  typename Sig<Unary, A1>::ResultType
  operator()( const Unary& f, const A1& a1 ) const {
    return thunkbinder1of1<Unary,A1>(f,a1);
  }
};
namespace { CurryThunk1 currythunk1, currythunk; }

class Counter {
   int count;
public:
   Counter() : count(0)             {}
   int get_count() const            { return count; }
   void increment_count()           { ++count; }
   void increment_count_by( int x ) { count += x; }
};

void show( int x ) {
   cout << "The counter is " << x << endl;
}

int main() {
   Counter c;
   Fun0<void> menu_actions[] = {
      makeFun0( currythunk( ptr_to_fun( &show ),
                            curry(ptr_to_fun(&Counter::get_count),&c) ) ),
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

