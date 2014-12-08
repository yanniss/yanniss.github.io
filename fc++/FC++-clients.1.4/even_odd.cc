#include <iostream>
#include <cmath>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

double xsqrt( double x ) {
   if( x < 0.0 )
      throw "bad";
   return sqrt(x);
}

OddList<double> picky() {
   return cons( xsqrt(-1.0), NIL );
}

int main() {
   using fcpp::plus;   // only g++2.95.2 needs this
   using fcpp::minus;

   cout << "--------------Starting section 0------------------" << endl;
   try{
      List<double> l = listUntil( Never1(), minus(_,1.0), 4.0 );
      l = take(5, map( id, map( ptr_to_fun( &xsqrt ), l ) ) ); 
      while(l) {
         cout << head(l) << endl;
//         cout << "bar" << endl;
         l = tail(l);
//         cout << "qux" << endl;
      }
   }
   catch( const char* s ) {
      cout << "Section 1 Oops: " << s << endl;
   }

   cout << "--------------Starting section 1------------------" << endl;
   cout << "This is the example in the Wadler paper" << endl;
   try{
      List<double> l = listUntil( Never1(), minus(_,1.0), 4.0 );
      l = take(5, map( ptr_to_fun( &xsqrt ), l ) ); 
      while(l) {
         cout << head(l) << endl;
//         cout << "bar" << endl;
         l = tail(l);  
//         cout << "qux" << endl;
      }
   }
   catch( const char* s ) {
      cout << "Section 1 Oops: " << s << endl;
   }

   cout << "--------------Starting section 2------------------" << endl;
   cout << "This is edge case of the example in the Wadler paper" << endl;
   try{
      List<double> l = listUntil( Never1(), minus(_,1.0), -1.0 );
      l = take(0, map( ptr_to_fun( &xsqrt ), l ) ); 
      if( l == NIL ) 
         cout << "empty list, as expected" << endl;
   }
   catch( const char* s ) {
      cout << "Section 2 Oops: " << s << endl;
   }

   cout << "--------------Starting section 3------------------" << endl;
   cout << "This is edge case with the first call curried" << endl;
   try{
      List<double> l = listUntil( Never1(), minus(_,1.0), -1.0 );
      l = curry2( map, ptr_to_fun( &xsqrt ), l ); 
      l = take( 0, l );
      if( l == NIL ) 
         cout << "empty list, as expected" << endl;
   }
   catch( const char* s ) {
      cout << "Section 3 Oops: " << s << endl;
   }

   cout << "--------------Starting section 4------------------" << endl;
   try{
      List<double> m = picky();
      cout << "happy" << endl;
   }
   catch( const char* s ) {
      cout << "Section 4 Oops: " << s << endl;
   }

   cout << "--------------Starting section 5------------------" << endl;
   try{
      List<double> m = ptr_to_fun( &picky );
      cout << "happy" << endl;
   }
   catch( const char* s ) {
      cout << "Section 5 Oops: " << s << endl;
   }
}
