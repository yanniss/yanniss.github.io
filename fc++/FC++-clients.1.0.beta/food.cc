#include <iostream>
#include <vector>
#include <algorithm>
#include "prelude.h"

template <class T>
struct CurryingFun2 {
   template <class A>
   binder1of2<T,A> operator()( const A& a ) const {
   //binder1of2<T,A> foo( const A& a ) const {
      return bind1of2( T(), a );
   }
};

struct XFilter : public CurryingFun2<XFilter> {
   template <class P, class LT>
   struct Sig : public FunType<P,LT,LT> {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) )
         return l;
      else if( p(head(l)) )
         return cons( head(l), bind1and2of2( XFilter(), p, tail(l) ) );
      else
         return XFilter()( p, tail(l) );
   }

   template <class A>
   binder1of2<XFilter,A> operator()( const A& a ) const {
      return bind1of2( *this, a );
   }
};
XFilter xfilter;

int main() {
   List<int> l = filter( odd, enumFrom(1) );
   List<int> m = xfilter( odd )( enumFrom(1) );
   cout << head(l) << " " << head(tail(l)) << endl;
   cout << head(m) << " " << head(tail(m)) << endl;

   return 0;
}

