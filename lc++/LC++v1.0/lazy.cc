#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "prolog.h"
using namespace lcpp;

#include "prelude.h"
using fcpp::plus;
using fcpp::List;
using fcpp::head;
using fcpp::tail;
using fcpp::null;

FUN1(n,int)

int main() {
   try {
      DECLARE(X   ,int,  10);
      DECLARE(Y   ,int,  11);

      lassert( n(0) );
      lassert( n(X) -= n(Y) && X.is(plus,Y,1) );

      std::cout << "---------------" << std::endl;
      typedef QRT<X_TYPE>::IE IE;
      List<IE> l = lquery( n(X) );
      for( int i=0; i<3; ++i ) {
         IE env = head(l);
         env->show();
         l = tail(l);
      }

      std::cout << "---------------" << std::endl;
      l = lquery( n(X) );
      fcpp::length( fcpp::map( fcpp::emptify( 
          fcpp::ptr_to_fun(&IE::WrappedType::show) ),
                               fcpp::take(3,l) ) );
   }
   catch( const char* s ) {
      std::cout << "exception: " << s << std::endl;
   }
   catch( ... ) {
      std::cout << "exception of some kind" << std::endl;
   }
}
