/*
These files (monad0.cc, monad2.cc, and monad3.cc) implement monads using
FC++.  The monad examples are taken from the Wadler paper "Monads for
Functional Programming" found at

   http://cm.bell-labs.com/cm/cs/who/wadler/topics/monads.html#marktoberdorf

and the examples implement variations 0, 2, and 3 as described in sections 
2.5-2.9 of the paper.

Note that as of v1.5 of the library, we can do much better (e.g. monad.h
and monad.cc).
*/

#include <iostream>
#include <string>
#include "prelude.h"

using std::cout;
using std::endl;
using std::pair;
using std::string;
using namespace fcpp;

//////////////////////////////////////////////////////////////////////
// useful for variation 3

#if __GNUC__ == 2     
#  include <strstream>
   template <class T>
   string toString( const T& x ) {
      std::ostrstream o;
      o << x << std::ends;
      return string(o.str());
   }
#else
#  include <sstream>
   template <class T>
   string toString( const T& x ) {
      std::ostringstream oss;
      oss << x;
      return oss.str();
   }
#endif

//////////////////////////////////////////////////////////////////////

class Term {
   int a_;   // also a
   Ref<Term> t_, u_;
   enum { Con, Div } type;
public:
   Term( int aa ) : a_(aa), type(Con) {}
   Term( Ref<Term> tt, Ref<Term> uu ) : t_(tt), u_(uu), type(Div) {}
   bool isCon() const { return type==Con; }
   int a() const { if( !isCon() ) throw "oops"; return a_; }
   Ref<Term> t() const { if( isCon() ) throw "oops"; return t_; }
   Ref<Term> u() const { if( isCon() ) throw "oops"; return u_; }
   string asString() const {
      if( isCon() ) return "(Con " + toString(a()) + ")";
      else return "(Div " + t()->asString() + " " + u()->asString() + ")";
   }
};

Ref<Term> Con( int a ) { return Ref<Term>( new Term(a) ); }
Ref<Term> Div( Ref<Term> t, Ref<Term> u ) { return Ref<Term>( new Term(t,u) ); }

// useful for variation 3
string line( Ref<Term> t, int v ) { 
   return t->asString() + " --> " + toString(v) + "\n"; 
}

//////////////////////////////////////////////////////////////////////
// We have static methods (unit, star, etc.) instead of static vars 
// because g++ sucks.

struct IdentityMonad {
   // We set up some handy typedefs so that
   //    M::of<a>::Type    ==  M a
   // and
   //    M::inv<Ma>::Type  ==  a
   // which enable us to "construct" and "deconstruct" the monad type.
   typedef IdentityMonad M;
   template <class A> struct of { typedef A Type; };
   template <class Ma> struct inv { typedef Ma Type; };

   struct Unit {
      template <class A>
      struct Sig : public FunType<A,typename M::template of<A>::Type> {};

      template <class A>
      typename M::template of<A>::Type operator()( const A& a ) const {
         return a;
      }
   };
   static Unit unit() { return Unit(); }
   
   struct Star {
      // MM == M a
      // KK == a -> M b
      template <class MM, class KK>
      struct StarHelp {
         typedef typename M::template inv<MM>::Type        A;
         typedef typename M::template of<A>::Type          Tmp;  // M a
         typedef typename KK::template Sig<A>::ResultType  K;    // M b
         typedef typename M::template inv<K>::Type         B;
         typedef typename M::template of<B>::Type          R;    // M b
      };

      template <class M, class K> 
      struct Sig : public FunType<M,K,typename StarHelp<M,K>::R> {};
   
      template <class M, class K>
      typename StarHelp<M,K>::R
      operator()( const M& m, const K& k ) const {
         return k(m);
      }
   };
   static Curryable2<Star> star() { return makeCurryable2( Star() ); }
};

//////////////////////////////////////////////////////////////////////

template <class M>
struct Eval : CFunType<Ref<Term>,typename M::template of<int>::Type> {

   // Lams are lambdas.  Their constructors take any 'captured'
   // variables.
   struct Lam2 : public CFunType<int,typename M::template of<int>::Type> {
      int a;
      Lam2( int aa ) : a(aa) {}
      typename M::template of<int>::Type operator()( int b ) const {
         return M::unit()( a/b );
      }
   };

   struct Lam1 : public CFunType<int,typename M::template of<int>::Type> {
      Ref<Term> u;
      Lam1( Ref<Term> uu ) : u(uu) {}
      typename M::template of<int>::Type operator()( int a ) const {
         return M::star()( Eval()(u), Lam2(a) );
      }
   };

   typename M::template of<int>::Type operator()( Ref<Term> term ) const {
      if( term->isCon() ) 
         return M::unit()( term->a() );
      else {
         Ref<Term> t = term->t(), u = term->u();
         return M::star()( Eval()(t), Lam1(u) );
      }
   }
};

//////////////////////////////////////////////////////////////////////

Ref<Term> answer() {
   return Div( Div( Con(1972), Con(2) ), Con(23) );
}

int main() {
   typedef IdentityMonad M;
   typedef Eval<M> E;
   E e;

   M::of<int>::Type r = e( answer() );   
   cout << r << endl;
}

