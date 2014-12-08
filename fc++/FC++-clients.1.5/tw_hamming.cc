#include <iostream>
#include "prelude.h"

#ifdef REAL_TIMING
#  include "timer.h"
#else
struct Timer { int ms_since_start() { return 0; } };
#endif

using namespace fcpp;

using std::cout; using std::endl;

#ifndef NUM
#define NUM 12000
#endif

using namespace fcpp;

struct Merge {
  template <class L, class M>
  struct Sig : public FunType<L,M,OddList<typename L::ElementType> > {};

  template <class T>
  OddList<T> operator()( const List<T>& a, const List<T>& b ) const {
    T x = head(a);
    T y = head(b);
    if( x < y )
      return cons( x, curry2(Merge(),tail(a),b));
    else if( x > y )
      return cons( y, curry2(Merge(),a,tail(b)));
    else
      return cons( x,
                curry2(Merge(),tail(a),tail(b)));
  }
} merge;

// Yes, yes, this is not ISO C++...
typedef long long int FOO;

struct Hamming : public CFunType< List<FOO> > {
   List<FOO> operator () () const {
      using fcpp::multiplies;
      static List<FOO> h = Hamming();
      static List<FOO> x = curry2(map,multiplies((FOO)2),h);
      static List<FOO> y = curry2(map,multiplies((FOO)3),h);
      static List<FOO> z = curry2(map,multiplies((FOO)5),h);
      static List<FOO> m1= curry2( merge, x, y );
      static List<FOO> m2= curry2( merge, m1, z );
      return cons( (FOO)1, m2 );
   }
} hamming;

int main() {
   Timer timer;
   cout << "The " << NUM << "th hamming number is: ";
   int start = timer.ms_since_start();
      cout << (long) at( hamming(), NUM ) << endl;
   int end = timer.ms_since_start();
   cout << "took " << end-start << " ms" << endl;
}

