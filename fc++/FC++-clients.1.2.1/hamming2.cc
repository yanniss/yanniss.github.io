#include <iostream>
#include "prelude.h"

using std::cout;
using std::endl;

struct LazyMerge : CFunType<List<int>,List<int>,List<int> > {
    List<int> operator () (const List<int>& xl, const List<int>& yl) const {
        int x = head(xl);
        int y = head(yl);
        if (x < y)
            return cons(x, curry2(LazyMerge(), tail(xl), yl));
        else if (x == y)
            return LazyMerge()(tail(xl), yl);
        else
            return cons(y, curry2(LazyMerge(), xl, tail(yl)));
    }
} merge;

struct HammingRest : public CFunType< List<int> > {
   List<int> operator()() const;
};

struct Hamming : public CFunType< List<int> > {
   List<int> operator () () const {
      HammingRest r;
      return cons(1,r);
   }
};

List<int> HammingRest::operator()() const {
   using fcpp::multiplies;   
   List<int> h = Hamming()();
   return merge( merge( map(multiplies(2),h),
                        map(multiplies(3),h) ),
                 map(multiplies(5),h) );
}

int main() {
   List<int> h = Hamming()();
   cout << at( h, 0 ) << endl;
   cout << at( h, 1 ) << endl;
   cout << at( h, 2 ) << endl;
   cout << at( h, 100 ) << endl;
}

