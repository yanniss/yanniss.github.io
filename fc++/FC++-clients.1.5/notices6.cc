#include <iostream>
#include <cassert>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

bool prime( int x ) {
   if( x<2 ) return false;
   for( int i=2; i<x; i++ )
      if( x%i == 0 )
         return false;
   return true;
}

bool big( int x ) {
   return x > 100;
}

int main() {
   using fcpp::plus;
   using fcpp::minus;

   Fun1<int,bool> f = makeFun1( ptr_to_fun(&prime) );
   assert( f(11) == true );
   f = makeFun1( ptr_to_fun(&big) );
   assert( f(11) == false );

   Fun2<int,int,int> g = makeFun2( monomorphize2<int,int,int>( plus ) );
   assert( g(3,2) == 5 );
   g = minus;  // makeFun2 and monomorphize2 can be implicit
   assert( g(3,2) == 1 );

   cout << "ok" << endl;
}
