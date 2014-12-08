// This program is based on the the paper
//    "Monadic Parser Combinators"
// by Graham Hutton and Erik Meijer.

#include <string>
#include <iostream>
#define FCPP_ENABLE_LAMBDA
#include "prelude.h"

using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::ostream;

using namespace fcpp;

typedef List<char> String;

struct ParserM {
   // M a = String -> [(a,String)]

   // We use indirect functoids as a representation type, since we will
   // often need two functions with different behaviors (but the same
   // signatures) to appear to have the same type.
   template <class A> struct Rep 
      { typedef Fun1<String,List<std::pair<A,String> > > Type; };
   template <class MA> struct UnRep { typedef typename 
      RT<MA,String>::ResultType::ElementType::first_type Type; };

   // ReRep is a type-identity in Haskell; here it "indirect"ifies the
   // type, so direct functoids are turned into indirect ones so that
   // only the signature information appears in the type.
   template <class MA> struct ReRep 
      { typedef typename Rep<typename UnRep<MA>::Type>::Type Type; };

   struct XUnit {
      template <class A> struct Sig : public FunType<A,
         typename Rep<A>::Type> {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const {
         LambdaVar<1> S;
         return lambda(S)[ cons[makePair[a,S],NIL] ];
      }
   };
   typedef Full1<XUnit> Unit;
   static Unit unit;

   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,typename 
         ReRep<typename RT<K,typename UnRep<M>::Type>::ResultType>::Type> {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         LambdaVar<1> P;
         LambdaVar<2> S;
         return lambda(S)[ concat[ compM<ListM>()
            [ k[fst[P]][snd[P]] | P <= m[S] ] ] ];
      }
   };
   typedef Full2<XBind> Bind;
   static Bind bind;

   typedef Fun1<String,AUniqueTypeForNil> Zero;
   static Zero zero;
};
ParserM::Unit ParserM::unit;
ParserM::Bind ParserM::bind;
ParserM::Zero ParserM::zero = ignore( const_(NIL) );

struct XItem : public CFunType<String,OddList<std::pair<char,String> > > {
   OddList<std::pair<char,String> > operator()( const String& s ) const {
      if( null(s) )
         return NIL;
      else
         return cons( makePair( head(s), tail(s) ), NIL );
   }
};
typedef Full1<XItem> Item;
Item item;

struct XPlusP {
   template <class P, class Q, class S> struct Sig : public 
      FunType<P,Q,String,typename RT<Cat,typename RT<P,String>::ResultType,
      typename RT<Curry1,Q,String>::ResultType>::ResultType> {};
   template <class P, class Q>
   typename Sig<P,Q,String>::ResultType
   operator()( const P& p, const Q& q, const String& s ) const {
      return p(s) ^cat^ curry1(q,s);
   }
};
typedef Full3<XPlusP> PlusP;
PlusP plusP;

template <class Monad>
struct XManyM {
   // Monad a -> Monad [a]
   template <class MA>
   struct Sig : public FunType<MA, typename RT<typename
      PlusM<Monad>::Type, typename LEType<LAM<COMP<CALL<Cons,LV<1>,LV<2>
      >,GETS<1,MA>,GETS<2,CALL<Full1<XManyM<Monad> >,MA> > > > >::Type,
      typename RT<typename UnitM<Monad>::Type, List<typename
      Monad::template UnRep<MA>::Type> >::ResultType>::ResultType> {};

   template <class MA>
   typename Sig<MA>::ResultType operator()( const MA& ma ) const {
      typedef typename Monad::template UnRep<MA>::Type AA;
      LambdaVar<1> A;
      LambdaVar<2> AS;
      return lambda()[ compM<Monad>()[ cons(A,AS) | 
         A <= ma, AS <= makeFull1(*this)[ma] ] ]() 
         ^plusM<Monad>()^ unitM<Monad>()( List<AA>() );
   }
};
// Just using parser version here:    Parser a -> Parser [a]
typedef Full1<XManyM<ParserM> > Many;
Many many;

// sat :: (char -> bool) -> Parser char
struct XSat {
   template <class P> struct Sig : public FunType<P,
      typename RT<typename LEType<LAM<COMP<ParserM,LV<1>,GETS<1,Item>,
      GUARD<CALL<P,LV<1> > > > > >::Type>::ResultType> {};
   template <class P>
   typename Sig<P>::ResultType
   operator()( const P& p ) const {
      LambdaVar<1> C;
      return lambda()[ compM<ParserM>()[ C | C<=item, guard[p[C]] ] ]();
   }
};
typedef Full1<XSat> Sat;
Sat sat;

struct XCharP : public CFunType<char, 
   RT<Sat,RT<Equal,char>::ResultType>::ResultType> {
   RT<Sat,RT<Equal,char>::ResultType>::ResultType
   operator()( char c ) const {
      return sat( equal(c) );
   }
};
typedef Full1<XCharP> CharP;
CharP charP;

struct XBetween {
   template <class T, class U, class V> struct Sig 
      : public FunType<T,T,T,bool> {};
   template <class T>
   bool operator()( const T& goal, const T& lower, const T& upper ) const {
      return lessEqual(goal,upper) && greaterEqual(goal,lower);
   }
};
typedef Full3<XBetween> Between;
Between between;

typedef RT<Sat,RT<Between,AutoCurryType,char,char>
   ::ResultType>::ResultType Digit;
Digit digit = sat( between(_,'0','9') );

typedef Digit Lower;
Lower lower = sat( between(_,'a','z') );

typedef Digit Upper;
Upper upper = sat( between(_,'A','Z') );

typedef RT<PlusP,Lower,Upper>::ResultType Letter;
Letter letter = lower ^plusP^ upper;

typedef RT<PlusP,Letter,Digit>::ResultType AlphaNum;
AlphaNum alphaNum = letter ^plusP^ digit;

template <class Monad>
struct XMany1M {
   // Monad a -> Monad [a]
   template <class MA>
   struct Sig : public FunType<MA, typename LEType<LAM<COMP<
      CALL<Delay,CALL<Cons,LV<1>,LV<2> > >,GETS<1,MA>,GETS<2,
      CALL<Full1<XManyM<Monad> >,MA> > > > >::Type> {};

   template <class MA>
   typename Sig<MA>::ResultType
   operator()( const MA& ma ) const {
      LambdaVar<1> A;
      LambdaVar<2> AS;
      return lambda()[ compM<Monad>() [ delay[cons[A,AS]] | 
         A <= ma, AS <= makeFull1(XManyM<Monad>())[ma] ] ]();
   }
};
// Just using parser version here:    Parser a -> Parser [a]
typedef Full1<XMany1M<ParserM> > Many1;
Many1 many1;

struct XChainl1 {
   // Parser a -> Parser (a->a->a) -> Parser a
   // parses a series of items separated by left-associative operators

   typedef BindM<ParserM>::Type BIND;
   typedef UnitM<ParserM>::Type UNIT;
   template <class P, class O>
   struct XRest {
      P p;
      O op;
      XRest( const P& pp, const O& oo ) : p(pp), op(oo) {}

      template <class X> struct Sig : public FunType<X,
         typename ParserM::Rep<X>::Type> {};
      template <class X>
      typename Sig<X>::ResultType
      operator()( const X& x ) const {
         LambdaVar<1> F;
         LambdaVar<2> Y;
         return (op ^BIND()^ lambda(F)[ 
                 p  %BIND()% lambda(Y)[
                 makeFull1(*this)[ F[x,Y] ] ] ]) ^plusP^ UNIT()(x);
      }
   };

   template <class P, class O> struct Sig : public FunType<P,O,
      typename RT<BIND,P,Full1<XRest<P,O> > >::ResultType> {};
   template <class P, class O>
   typename Sig<P,O>::ResultType
   operator()( const P& p, const O& op ) const {
      return p ^BIND()^ makeFull1(XRest<P,O>(p,op));
   }
};
typedef Full2<XChainl1> Chainl1;
Chainl1 chainl1;

struct XChainr1 {
   // Parser a -> Parser (a->a->a) -> Parser a
   // parses a series of items separated by right-associative operators

   template <class P, class O> struct Sig : public FunType<P,O,
      typename ParserM::Rep<typename ParserM::UnRep<P>::Type>::Type> {};
   template <class P, class O>
   typename Sig<P,O>::ResultType
   operator()( const P& p, const O& op ) const {
      LambdaVar<1> F;
      LambdaVar<2> X;
      LambdaVar<3> Y;
      return p ^bindM<ParserM>()^ lambda(X)[ compM<ParserM>()
         [ F[X,Y] | F <= op, Y <= makeFull2(*this)[p][op] ]
         %plusP% unitM<ParserM>()[X] ];
   }
};
typedef Full2<XChainr1> Chainr1;
Chainr1 chainr1;

struct XChainl {
   template <class P, class O, class V> struct Sig : public FunType<P,O,V,
      typename RT<PlusP,typename RT<Chainl1,P,O>::ResultType,
      typename RT<typename UnitM<ParserM>::Type,V>::ResultType>::ResultType> {};
   template <class P, class O, class V>
   typename Sig<P,O,V>::ResultType
   operator()( const P& p, const O& op, const V& v ) const {
      return (p ^chainl1^ op) ^plusP^ unitM<ParserM>()(v);
   }
};
typedef Full3<XChainl> Chainl;
Chainl chainl;

struct XChainr {
   template <class P, class O, class V> struct Sig : public FunType<P,O,V,
      typename RT<PlusP,typename RT<Chainr1,P,O>::ResultType,
      typename RT<typename UnitM<ParserM>::Type,V>::ResultType>::ResultType> {};
   template <class P, class O, class V>
   typename Sig<P,O,V>::ResultType
   operator()( const P& p, const O& op, const V& v ) const {
      return (p ^chainr1^ op) ^plusP^ unitM<ParserM>()(v);
   }
};
typedef Full3<XChainr> Chainr;
Chainr chainr;

typedef RT<LEType<LAM<LET<BIND<1,LAM<LV<2>,LV<3>,CALL<Plus,
   CALL<Multiplies,int,LV<2> >,LV<3> > > >,CALL<Chainl1,COMP<ParserM,
   CALL<Construct1<int>::Type,CALL<Minus,LV<4>,char> >,GETS<4,Digit> >,
   CALL<UnitM<ParserM>::Type,LV<1> > > > > >::Type>::ResultType Nat;
Nat xnat() {
   LambdaVar<1> OP;
   LambdaVar<2> M;
   LambdaVar<3> N;
   LambdaVar<4> X;
   return lambda()[ let[ 
      OP == lambda(M,N)[ plus[multiplies[10,M],N] ]
      ].in[ compM<ParserM>()[ construct1<int>()[minus[X,'0']] | X<=digit ]
            %chainl1% unitM<ParserM>()[OP] ] ]();
}
Nat nat = xnat();

typedef RT<LEType<LAM<LET<BIND<3,CALL<PlusP,COMP<ParserM,CALL<
   Construct1<Fun1<int,int> >::Type,Negate>,CALL<CharP,char> >,CALL<
   UnitM<ParserM>::Type,CALL<Construct1<Fun1<int,int> >::Type,Id> > > >, 
   COMP<ParserM,CALL<LV<1>,LV<2> >,GETS<1,LV<3> >,GETS<2,Nat> 
   > > > >::Type>::ResultType IntP;
IntP xintp() {
   LambdaVar<1> F;
   LambdaVar<2> N;
   LambdaVar<3> OP;
   Construct1<Fun1<int,int> >::Type cf = construct1<Fun1<int,int> >();
   return lambda()[ let[ OP == compM<ParserM>()[ cf[negate] | charP['-'] ]
                               %plusP% unitM<ParserM>()[ cf[id] ] ] 
      .in[ compM<ParserM>()[ F[N] | F<=OP, N<=nat ] ]   ]();
}
IntP intP = xintp();

struct XSepBy1 {
   // Parser a -> Parser b -> Parser [a]
   // parses "p (sep p)*", throwing away the separators
   template <class P, class S> struct Sig : public FunType<P,S,
      typename RT<typename LEType<LAM<COMP<ParserM,CALL<Delay,
      CALL<Cons,LV<1>,LV<2> > >,GETS<1,P>,GETS<2,CALL<Many,
      COMP<ParserM,LV<3>,S,GETS<3,P> > > > > > >::Type>::ResultType> {};
   template <class P, class S>
   typename Sig<P,S>::ResultType
   operator()( const P& p, const S& sep ) const {
      LambdaVar<1> X;
      LambdaVar<2> XS;
      LambdaVar<3> Y;
      return lambda()[ compM<ParserM>()[ delay[cons[X,XS]] |
         X <= p, XS <= many[ compM<ParserM>()[ Y | sep, Y <= p ] ] ] ]();
   }
};
typedef Full2<XSepBy1> SepBy1;
SepBy1 sepBy1;

struct XBracket {
   template <class O, class P, class C> struct Sig : public FunType<O,P,C,
      typename RT<typename LEType<LAM<COMP<ParserM,LV<1>,O,GETS<1,P>,C>
      > >::Type>::ResultType> {};
   template <class O, class P, class C>
   typename Sig<O,P,C>::ResultType
   operator()( const O& open, const P& p, const C& close ) const {
      LambdaVar<1> X;
      return lambda()[ compM<ParserM>()[ X | open, X<=p, close ] ]();
   }
};
typedef Full3<XBracket> Bracket;
Bracket bracket;

struct XSepBy {
   template <class P, class S> struct Sig : public FunType<P,S,
      typename RT<PlusP,typename RT<SepBy1,P,S>::ResultType,
      typename RT<UnitM<ParserM>::Type,List<typename
      ParserM::UnRep<P>::Type> >::ResultType>::ResultType> {};
   template <class P, class S>
   typename Sig<P,S>::ResultType
   operator()( const P& p, const S& sep ) const {
      typedef typename ParserM::UnRep<P>::Type A;
      List<A> l = NIL;
      return (p ^sepBy1^ sep) ^plusP^ unitM<ParserM>()( l );
   }
};
typedef Full2<XSepBy> SepBy;
SepBy sepBy;

struct XOps {
   // [(Parser a, b)] -> Parser b
   // given a list of pair<parser to parse op,op>, returns a parser
   template <class X> struct Sig : public FunType<X,
      typename RT<Foldr1,PlusP,typename RT<typename LEType<LAM<
      COMP<ListM,COMP<ParserM,CALL<Snd,LV<1> >,CALL<Fst,LV<1> > >,
      GETS<1,X> > > >::Type>::ResultType>::ResultType> {};
   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& xs ) const {
      LambdaVar<1> P;
      return foldr1( plusP, lambda()[ compM<ListM>()[ 
         compM<ParserM>()[ snd[P] | fst[P] ]   | P <= xs ] ]() );
   }
};
typedef Full1<XOps> Ops;
Ops ops;

//////////////////////////////////////////////////////////////////////

ostream& operator<<( ostream& o, const String& s ) {
   string ss( s.begin(), s.end() );
   return o << "\"" << ss << "\"";
}

template <class A, class B>
ostream& operator<<( ostream& o, const std::pair<A,B>& p ) {
   return o << "(" << p.first << "," << p.second << ")";
}

template <class T>
ostream& operator<<( ostream& o, OddList<T> l ) {
   o << "[";
   if(l)
      for(;;) {
         o << head(l);
         l = tail(l);
         if(l)
            o << ",";
         else 
            break;
      }
   return o << "]";
}

template <class T>
ostream& operator<<( ostream& o, List<T> l ) {
   return o << l.force();
}

//////////////////////////////////////////////////////////////////////

int my_pow( int x, int y ) {
   int r = 1;
   while(y) {
      r *= x;
      --y;
   }
   return r;
}

typedef ParserM::Rep<int>::Type ExprP;
extern ExprP exprP;

typedef RT<Ops,List<std::pair<RT<CharP,char>::ResultType,Fun2<int,int,int> 
   > > >::ResultType AddOp;
AddOp xaddOp() {
   typedef Fun2<int,int,int> F;
   return ops( list_with( 
      makePair( charP('+'), F(plus)  ), 
      makePair( charP('-'), F(minus) )   ) );
}
AddOp addOp = xaddOp();

typedef AddOp ExpOp;
ExpOp xexpOp() {
   typedef Fun2<int,int,int> F;
   return ops( list_with( makePair(charP('^'),F(ptr_to_fun(&my_pow))) ) );
}
ExpOp expOp = xexpOp();
   
typedef RT<PlusP,Nat,RT<Bracket,RT<CharP,char>::ResultType,
   ExprP,RT<CharP,char>::ResultType>::ResultType>::ResultType Factor;
Factor xfactor() {
   static Factor result = nat ^plusP^ bracket( charP('('), exprP, charP(')') );
   return result;
}

typedef ExprP Term;  // I am too lazy to direct-type this
Term xterm() {
   static Term result = thunkFuncToFunc(ptr_to_fun(&xfactor)) ^chainr1^ expOp;
   return result;
}

ExprP xexprP() {
   return thunkFuncToFunc(ptr_to_fun(&xterm)) ^chainl1^ addOp;
}
ExprP exprP = xexprP();

//////////////////////////////////////////////////////////////////////
// Here I just want to show the straightforward way using indirect
// functoid types:

typedef ParserM::Rep<int>::Type P_int;
P_int dummy = ignore(const_(cons(makePair(0,String()),NIL)));
P_int group=dummy, factor=dummy, term=dummy, expression=dummy;

int main() {
   LambdaVar<91> S;
   factor     = lambda(S)[ (nat %plusP% dereference[&group])[S] ];
   term       = factor ^chainr1^ expOp;
   expression = term ^chainl1^ addOp;
   group      = bracket( charP('('), expression, charP(')') );
//////////////////////////////////////////////////////////////////////

   string ss;
   cout << "Enter a string to parse (e.g. '1+2^3-(2-1)^5'):" << endl;
   getline(cin,ss);
   String s( ss.begin(), ss.end() );
   length(s);  // force evaluation


   typedef ParserM P;
   LambdaVar<1> X;
   List<std::pair<char,String> > lpcs = lambda()[ compM<P>()
      [ X | X <= item ] ]()(s);
   cout << "item: " << lpcs << endl;
   cout << "List of expression parses:" << endl << exprP(s) << endl;
   cout << "List of expression parses:" << endl << expression(s) << endl;

/*
This works.  I did it just to prove a point.
   List<std::pair<String,String> > lpss;
   LambdaVar<12> lower;
   LambdaVar<13> upper;
   LambdaVar<14> letter;
   LambdaVar<15> word;
   lpss = lambda()[ let[
      lower == compM<P>()[ X | X<=item, guard[logicalAnd[greaterEqual[X,'a'],
                                                         lessEqual[X,'z']]]],
      upper == compM<P>()[ X | X<=item, guard[logicalAnd[greaterEqual[X,'A'],
                                                         lessEqual[X,'Z']]]],
      letter == lower %plusP% upper,
      word == many[letter] 
      ].in[ word[s] ] ]();
   cout << lpss << endl;
*/

/* FIX THIS comment window
   List<std::pair<String,String> > lpss;
   lpss = many(letter)(s);
   cout << "many(letter): " << lpss << endl;
   lpss = many1(letter)(s);
   cout << "many1(letter): " << lpss << endl;

   List<std::pair<int,String> > lpis;
   lpis = nat(s);
   cout << "nat: " << lpis << endl;

   List<std::pair<List<int>,String> > lplis;
   lplis = bracket( charP('['), intP ^sepBy^ charP(','), charP(']') )(s);
   cout << "list of ints: " << lplis << endl;

   cout << "List of expression parses:" << endl << exprP(s) << endl;

   LambdaVar<21> ex;
   LambdaVar<22> tm;
   LambdaVar<23> fc;
   LambdaVar<24> S;
   Fun1<String,List<std::pair<int,String> > > dummy = ignore( const_(
      List<std::pair<int,String> >() ) );
   cout << "Expr parses via letrec defs:" << endl <<
      lambda()[ letrec[
// need some way to break recursion... one way is shown below
//         ex == lambda(S)[ (tm %chainl1% addOp)[S] ],
         ex == if2[true,lambda(S)[ (tm %chainl1% addOp)[S] ],dummy],
         tm == lambda(S)[ (fc %chainr1% expOp)[S] ],
         fc == lambda(S)[ (nat %plusP% bracket[charP['('],ex,charP[')']])[S] ]
      ].in[ ex[s] ] ]() << endl;
*/
}
