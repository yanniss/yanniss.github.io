#include <iostream>
#include <map>
#define FCPP_ENABLE_LAMBDA
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

//////////////////////////////////////////////////////////////////////
// Here I do the ST monad as per the "State in Haskell" paper

struct MutVar { 
   int name; 
   MutVar(int n):name(n) {} 
   bool operator<( const MutVar& m ) const { return name < m.name; }
};

struct STM {
   // FIX THIS
   // for simplicity for now, let's fix types so that the type of
   // references is always just MutVar and the type of thing they
   // reference is always just int
   typedef std::map<MutVar,int> State;

   struct XUnit {
      template <class A> struct Sig : public FunType<A,
         typename RT<MakePair,A>::ResultType> {};
      template <class A>
      typename Sig<A>::ResultType
      operator()( const A& a ) const { return makePair(a); }
   };
   typedef Full1<XUnit> Unit;
   static Unit unit;

   struct XBind {
      template <class M, class K> struct Sig : public FunType<M,K,
         typename LEType<LAM<LV<2>,LET<BIND<1,CALL<M,LV<2> > >,
                                       CALL<CALL<K,CALL<Fst,LV<1> > >,
                                            CALL<Snd,LV<1> > > 
                                      > > >::Type > {};
      template <class M, class K>
      typename Sig<M,K>::ResultType
      operator()( const M& m, const K& k ) const {
         LambdaVar<1> P;
         LambdaVar<2> s0;
         return lambda(s0)[ let[ P == m[s0] ].in[
                            k[fst[P]][snd[P]] ] ];
      }
   };
   typedef Full2<XBind> Bind;
   static Bind bind;

   // FIX THIS am currently copying the state; eventually the whole point
   // is not to, duh
   struct XNewVarHelper : public CFunType<State,int,std::pair<MutVar,State> > {
      std::pair<MutVar,State> operator()( State s, int a ) const {
         // find the first available name
         int i = 0;
         while( s.find( MutVar(i) ) != s.end() )
            ++i;
         // update the state, return the reference
         s.insert( makePair(MutVar(i),a) );
         return makePair(MutVar(i),s);
      }
   };
   typedef Full2<XNewVarHelper> NewVarHelper;
   struct XNewVar {
      template <class A> struct Sig : public FunType<int,
         LEType< LAM<LV<1>,CALL<NewVarHelper,LV<1>,int> > >::Type> {};
      Sig<int>::ResultType
      operator()( const int& a ) const {
         LambdaVar<1> S;
         return lambda(S)[ NewVarHelper()[S,a] ];
      }
   };
   typedef Full1<XNewVar> NewVar;
   static NewVar newVar;

   // ditto
   struct XWriteVarHelper 
   : public CFunType<State,MutVar,int,std::pair<Empty,State> > {
      std::pair<Empty,State> operator()( State s, MutVar v, int a ) const {
         s.find(v.name)->second = a;
         return makePair(empty,s);
      }
   };
   typedef Full3<XWriteVarHelper> WriteVarHelper;
   struct XWriteVar {
      template <class V, class A> struct Sig : public FunType<MutVar,int,
         LEType< LAM<LV<1>,CALL<WriteVarHelper,LV<1>,MutVar,int> 
         > >::Type > {};
      Sig<MutVar,int>::ResultType
      operator()( const MutVar& v, const int& a ) const {
         LambdaVar<1> S;
         return lambda(S)[ WriteVarHelper()[S,v,a] ];
      }
   };
   typedef Full2<XWriteVar> WriteVar;
   static WriteVar writeVar;

   // ditto
   struct XReadVarHelper 
   : public CFunType<State,MutVar,std::pair<int,State> > {
      std::pair<int,State> operator()( const State& s, MutVar v ) const {
         int r = s.find(v.name)->second;
         return makePair(r,s);
      }
   };
   typedef Full2<XReadVarHelper> ReadVarHelper;
   struct XReadVar {
      template <class V> struct Sig : public FunType<MutVar,
         LEType< LAM<LV<1>,CALL<ReadVarHelper,LV<1>,MutVar> > >::Type > {};
      Sig<MutVar>::ResultType
      operator()( const MutVar& v ) const {
         LambdaVar<1> S;
         return lambda(S)[ ReadVarHelper()[S,v] ];
      }
   };
   typedef Full1<XReadVar> ReadVar;
   static ReadVar readVar;

   struct XRun {
      template <class ST> struct Sig : public FunType<ST,int> {};
      template <class ST>
      int operator()( const ST& st ) const {
         State s;   // initial empty state (eventually could be new-ed)
         std::pair<int,State> p = st(s);
         int r = p.first;
         // eventually could delete here
         return r;
      }
   };
   typedef Full1<XRun> Run;
   static Run run;
};
STM::Unit     STM::unit;
STM::Bind     STM::bind;
STM::NewVar   STM::newVar;
STM::ReadVar  STM::readVar;
STM::WriteVar STM::writeVar;
STM::Run      STM::run;

//////////////////////////////////////////////////////////////////////

LEType<LAM<LET<BIND<1,int>,BIND<2,int>,
               CALL<Minus,LV<1>,LV<2> > > > >::Type
foo() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   return lambda()[ let[ X == 4, Y == 1 ].in[ minus[X,Y] ] ];
}

LEType<LAM<DOM<GETS<1,List<int> >,GETS<2,List<int> >,
               CALL<UnitM<ListM>::Type,CALL<MakePair,LV<1>,LV<2>
> > > > >::Type
bar() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   return lambda()[
      doM[ X <= list_with(1,2), Y <= list_with(3,4), 
           unitM<ListM>()[ makePair[X,Y] ] ] ];
}

LEType<LAM<LET<BIND<1,int>,IF0<CALL<Less,LV<1>,int>,int,int> > > >::Type
baz() {
   LambdaVar<1> X;
   return lambda()[ let[ X == 3 ].in[ if0[less[X,10],1,0] ] ];
}

LEType<LAM<COMP<ListM,CALL<MakePair,LV<1>,LV<2> >,GETS<1,List<int> >,
   GUARD<bool>,GETS<2,List<int> >,GUARD<CALL<Equal,CALL<Divides,LV<2>,LV<1> >,
   int> > > > >::Type
qux() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   return lambda()[ compM<ListM>()[ makePair[X,Y] | 
          X<=list_with(1,2), guard[true], Y<=list_with(3,4), 
          guard[equal[divides[Y,X],3] ] ]
      ];
}

template <class T>
std::ostream& operator<<( std::ostream& o, const Maybe<T>& mx ) {
   if( mx.is_nothing() )
      o << "Nothing";
   else
      o << "Just " << mx.value();
   return o;
}

int main() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   LambdaVar<3> L;
   LambdaVar<4> V;
   // comp<ListM>()[ makePair[X,Y] | X <= list_with(1,2),
   //                                Y <= list_with(3,4) ]
   //==>
   // bind[list_with(1,2), lambda(X)[ 
   //     comp<ListM>()[ makePair[X,Y] | Y <= list_with(3,4) ] ]
   //==>
   // bind[list_with(1,2), lambda(X)[ 
   //    bind[list_with(3,4), lambda(Y)[ unit<ListM>()[ makePair[X,Y] ]
   //    ] ] ] ]

   // g++ won't define the static members without this
   (void) &(StateM<int>::unit);
   (void) &(StateM<int>::bind);

   List<std::pair<int,int> > l =
      list_with(1,2) ^bind^ lambda(X)[ 
         list_with(3,4) %bind% lambda(Y)[ unitM<ListM>()[ makePair[X,Y] ]
         ] ];
   while( !null(l) ) {
      cout << head(l).first << "," << head(l).second << endl;
      l = tail(l);
   }
   cout << "---------" << endl;
   std::pair<int,int> p =
      bindM<IdentityM>()(1, lambda(X)[ 
         bindM<IdentityM>()[2, lambda(Y)[ unitM<IdentityM>()[ makePair[X,Y] ]
         ] ] ] );
   cout << p.first << "," << p.second << endl;
   cout << "---------" << endl;
   p = bindM<StateM<int> >()( lambda(X)[ makePair[3,X] ],
          lambda(Y)[ unitM<StateM<int> >()[ Y ] ])(0);
   cout << p.first << "," << p.second << endl;
   p = bindM_<StateM<int> >()( StateM<int>::assign(3),
          bindM<StateM<int> >()( StateM<int>::fetch(),
             lambda(X)[ unitM<StateM<int> >()[ X ] ] ) )(0);
   cout << p.first << "," << p.second << endl;
   cout << "---------" << endl;
/* FIX THIS (just a comment window)
*/
   cout << foo()() << endl;
   cout << "---------" << endl;
   l = lambda()[
      doM[ X <= list_with(1,2), Y <= list_with(3,4), 
           unitM<ListM>()[ makePair[X,Y] ] ] ]();
   while( !null(l) ) {
      cout << head(l).first << "," << head(l).second << endl;
      l = tail(l);
   }
   cout << "---------" << endl;
   l = bar()();
   while( !null(l) ) {
      cout << head(l).first << "," << head(l).second << endl;
      l = tail(l);
   }
   cout << "---------" << endl;
   l = joinM<ListM>()( mapM<ListM>()( lambda(X)[ mapM<ListM>()[
         lambda(Y)[ makePair[X,Y] ], list_with(3,4) ] ], list_with(1,2) ) );
   while( !null(l) ) {
      cout << head(l).first << "," << head(l).second << endl;
      l = tail(l);
   }
   cout << "---------" << endl;
   l = lambda()[ 
      compM<ListM>()[ makePair[X,Y] | X<=list_with(1,2), Y<=list_with(3,4) ] 
      ]();
   while( !null(l) ) {
      cout << head(l).first << "," << head(l).second << endl;
      l = tail(l);
   }
   cout << "---------" << endl;
   p = lambda()[
      compM<IdentityM>()[ makePair[X,Y] | X <= 1, Y <= 1 ]
      ]();
   cout << p.first << "," << p.second << endl;
   cout << "---------" << endl;
   p = lambda(X)[
      compM<IdentityM>()[ makePair[X,Y] | Y <= 1 ]
      ](1);
   cout << p.first << "," << p.second << endl;
   cout << "---------" << endl;
   List<int> li = lambda(L)[ map[ lambda(X)[ plus[head[L],X] ], L ] ](
      list_with(2,3,4) );
   cout << at(li,0) << " " << at(li,1) << " " << at(li,2) << endl;
   cout << "---------" << endl;
   cout << baz()() << endl;
   cout << "---------" << endl;
   l = lambda()[ compM<ListM>()[ makePair[X,Y] | 
          X<=list_with(1,2), Y<=list_with(3,4), guard[greater[X,Y]] ] 
      ]();
   cout << length(l) << endl;
   l = lambda()[ compM<ListM>()[ makePair[X,Y] | 
          X<=list_with(1,2), guard[greater[X,3]], Y<=list_with(3,4) ] 
      ]();
   cout << length(l) << endl;
   cout << "---------" << endl;
   l = lambda()[ compM<ListM>()[ makePair[X,Y] | 
          X<=list_with(1,2), guard[true], Y<=list_with(3,4), 
          guard[equal[divides[Y,X],3] ] ]
      ]();
   while( !null(l) ) {
      cout << head(l).first << "," << head(l).second << endl;
      l = tail(l);
   }
   cout << "---------" << endl;
   l = qux()();
   while( !null(l) ) {
      cout << head(l).first << "," << head(l).second << endl;
      l = tail(l);
   }
   cout << "---------" << endl;
   Maybe<int> mx = NOTHING;
   Maybe<int> my = just(3);
   mx = lambda()[ compM<MaybeM>()[ plus[X,Y] | X<=mx, Y<=my ] ]();
   cout << mx << endl;
   cout << "---------" << endl;
   mx = just(2);
   my = just(3);
   mx = lambda()[ compM<MaybeM>()[ plus[X,Y] | X<=mx, Y<=my ] ]();
   cout << mx << endl;
   cout << "---------" << endl;
   mx = just(2);
   my = just(3);
   mx = lambda()[ compM<MaybeM>()[ plus[X,Y] | X<=mx, Y<=my, guard[false] ] ]();
   cout << mx << endl;
   cout << "---------" << endl;
   mx = lambda()[ doM[ X<=just[2], Y<=just[4], unitM<MaybeM>()[plus[X,Y]] ] ]();
   cout << mx << endl;
   cout << "-----------------------" << endl;
   cout << STM::run( lambda()[ compM<STM>()[ X |
      V <= STM::newVar[3], 
      STM::writeVar[V,4],
      X <= STM::readVar[V]
      ] ]() ) << endl;
   cout << "---------" << endl;
}
