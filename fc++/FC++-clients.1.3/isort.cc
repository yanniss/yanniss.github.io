#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cerr;
using std::cout;
using std::endl;

struct Insert : public CFunType<int,List<int>,List<int> > {
   List<int> operator()( int x, const List<int>& l ) const {
      if( null(l) || (x > head(l)) )
         return cons( x, l );
      else
         return cons( head(l), curry2(Insert(),x,tail(l)) );
   }
};

struct Isort : public CFunType<List<int>,List<int> > {
   List<int> operator()( const List<int>& l ) const {
      return foldr( Insert(), List<int>(), l );
   }
};

int main() {
   List<int> list = list_with(3,8,14,5,7);
   list = cat( list, list_with(21,2,6,19,1) );
   List<int> l = Isort()( list );
   while( l ) {
      cerr << head(l) << " ";
      l = tail(l);
   }
   cerr << endl;

   return 0;
}

