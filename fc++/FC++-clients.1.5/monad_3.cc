/*
These files (monad_0,monad_2,monad_3) implement the monad examples taken 
from the Wadler paper "Monads for Functional Programming" found at

   http://cm.bell-labs.com/cm/cs/who/wadler/topics/monads.html#marktoberdorf

and the examples implement variations 0, 2, and 3 as described in sections 
2.5-2.9 of the paper.

These files supersede monad0/monad2/monad3, now that monads and lambda
are built into FC++.
*/

#include <iostream>
#include <string>
#define FCPP_ENABLE_LAMBDA
#include "prelude.h"

using std::cout;
using std::endl;
using std::pair;
using std::string;
using namespace fcpp;

//////////////////////////////////////////////////////////////////////
// useful for variation 3

#include <sstream>
template <class T>
string toString( const T& x ) {
   std::ostringstream oss;
   oss << x;
   return oss.str();
}

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
string xline( Ref<Term> t, int v ) { 
   return t->asString() + " --> " + toString(v) + "\n"; 
}
Fun2<Ref<Term>,int,string> line = ptr_to_fun(&xline);

//////////////////////////////////////////////////////////////////////

struct OutputM {
   struct XUnit {
      template <class A> struct Sig : public FunType<A,pair<string,A> > {};
      template <class A>
      typename Sig<A>::ResultType operator()( const A& a ) const {
         return makePair( string(), a );
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit unit;

   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,
         typename RT<K,typename M::second_type>::ResultType> {};
      
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         LambdaVar<1> XA;
         LambdaVar<2> YB;
         return lambda()[ let[ XA == m, YB == k[snd[XA]] ]
            .in[ makePair[ fst[XA] %plus% fst[YB], snd[YB] ] ] ]();
      }
   };
   typedef Full2<XBind> Bind;
   static Bind bind;
};
OutputM::Unit OutputM::unit;
OutputM::Bind OutputM::bind;

Fun1<string,pair<string,Empty> > out = makePair(_,empty);

//////////////////////////////////////////////////////////////////////

typedef OutputM          M;
typedef pair<string,int> M_int;

//////////////////////////////////////////////////////////////////////

struct Eval : CFunType<Ref<Term>,M_int> {
   M_int operator()( Ref<Term> term ) const {
      if( term->isCon() ) 
         return out(line(term,term->a())) ^bindM_<M>()^ M::unit( term->a() );
      else {
         LambdaVar<1> A;
         LambdaVar<2> B;
         Ref<Term> t = term->t(), u = term->u();
         return lambda()[ compM<M>()[ A %divides% B |
            A <= Eval()(t), B <= Eval()(u),
            out[line[term,A %divides% B]] ] ]();
      }
   }
};

//////////////////////////////////////////////////////////////////////

Ref<Term> answer() {
   return Div( Div( Con(1972), Con(2) ), Con(23) );
}

int main() {
   Eval e;

   M_int r = e( answer() );   
   cout << "(" << r.first << "," << r.second << ")" << endl;
}

