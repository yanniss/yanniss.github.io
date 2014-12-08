#include "prelude.h"
#include <iostream>

using namespace fcpp;

using std::cout;
using std::endl;

template <class R, class C, class A>
class mem_binder1of2 : public CFunType<A,R> {
   R (C::*f)(A);
   C& ref;
public:
   explicit mem_binder1of2( R (C::*f_)(A), C& ref_ ) 
   : f(f_), ref(ref_) {}

   R operator()( A x ) {
      return (ref.*f)(x);
   }
};

template <class R, class C, class A>
mem_binder1of2<R,C,A>
xbind1of2( R (C::*f)(A), C& ref ) {
   return mem_binder1of2<R,C,A>(f,ref);
}

template <class R, class C, class A>
mem_binder1of2<R,C,A>
xbind1of2( R (C::*f)(A), const C& ref ) {
   // select the non-const version and call it...
   mem_binder1of2<R,C,A> (*pf)( R (C::*f)(A), C& ref ) = &xbind1of2;
   return (*pf)(f,ref);      // ... as it gives a more helpful diagnostic
}

template <class R, class C, class A, class P>
mem_binder1of2<R,C,A>
xbind1of2( R (C::*f)(A), P ptr ) {
   return mem_binder1of2<R,C,A>(f,*ptr);
}

template <class R, class C, class A>
class constmem_binder1of2 : public CFunType<A,R> {
   R (C::*f)(A) const;
   const C& ref;
public:
   explicit constmem_binder1of2( R (C::*f_)(A) const, const C& ref_ ) 
   : f(f_), ref(ref_) {}

   R operator()( A x ) const {
      return (ref.*f)(x);
   }
};

template <class R, class C, class A>
constmem_binder1of2<R,C,A>
xbind1of2( R (C::*f)(A) const, const C& ref ) {
   return constmem_binder1of2<R,C,A>(f,ref);
}

template <class R, class C, class A, class P>
constmem_binder1of2<R,C,A>
xbind1of2( R (C::*f)(A) const, P ptr ) {
   return constmem_binder1of2<R,C,A>(f,*ptr);
}

struct Foo {};

struct O {
   int aa;
   O( int a ) : aa(a) {}
   int cf( int x ) const { return x+1; }
   int f( int x ) { return ++aa + x; }
};

int main() {
   O o(1);
   cout << xbind1of2( &O::cf, o )( 1 ) << endl;   // should be 2
   cout << xbind1of2( &O::f, o )( 1 ) << endl;    // should be 3
   cout << xbind1of2( &O::f, o )( 1 ) << endl;    // should be 4
   cout << xbind1of2( &O::cf, o )( 1 ) << endl;   // should be 2

   const O p(1);
   cout << xbind1of2( &O::cf, p )( 1 ) << endl;   // should be 2
   //cout << xbind1of2( &O::f, p )( 1 ) << endl;  // good error msg

   Ref<O> r( new O(3) );
   cout << xbind1of2( &O::cf, r )( 1 ) << endl;   // should be 2
   cout << xbind1of2( &O::f, r )( 1 ) << endl;    // should be 5
   cout << xbind1of2( &O::f, r )( 1 ) << endl;    // should be 6
   cout << xbind1of2( &O::cf, r )( 1 ) << endl;   // should be 2

   Ref<const O> s( new O(3) );
   cout << xbind1of2( &O::cf, s )( 1 ) << endl;   // should be 2
   //cout << xbind1of2( &O::f, s )( 1 ) << endl;  // good error msg

   //cout << xbind1of2( &O::cf, Foo() )( 1 ) << endl; 
   //cout << xbind1of2( &O::f, Foo() )( 1 ) << endl; 

   return 0;
}


