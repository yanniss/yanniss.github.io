#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#define FCPP_ENABLE_LAMBDA
#include "prelude.h"

using std::cout;
using std::endl;
using std::ostream;
using namespace fcpp;

struct Crazy {
   template <class T> struct Sig : public FunType<T,int> {};
   template <class T>
   int operator()( const T& ) const { return 2; }
} crazy;

LEType<LAM<LETREC<BIND<1,LAM<LV<3>,IF1<CALL<Equal,LV<3>,int>,bool,
   CALL<LV<2>,CALL<Minus,LV<3>,int> > > > >,BIND<2,LAM<LV<3>,IF2<
   CALL<NotEqual,LV<3>,int>,CALL<LV<1>,CALL<Minus,LV<3>,int> >,bool>
   > >,CALL<LV<1>,int> > > >::Type
whoa() {
   LambdaVar<1> even;
   LambdaVar<2> odd;
   LambdaVar<3> X;
   return lambda()[ 
      letrec[ even==lambda(X)[ if1[equal[X,0],true,odd[minus[X,1]]] ],
              odd ==lambda(X)[ if2[notEqual[X,0],even[minus[X,1]],false] ] 
      ].in[ even[3] ] ];
}

LEType<LAM<LV<1>,LV<2>,LV<3>,IF0<CALL<Equal,LV<1>,int>,int,CALL<Minus,
   LV<2>,LV<3> > > > >::Type
shazam() {
   LambdaVar<1> X;
   LambdaVar<2> Y;
   LambdaVar<3> Z;
   return lambda(X,Y,Z)[ if0[equal[X,0],3,minus[Y,Z]] ];
}

int main() {
   using fcpp::negate;
   using fcpp::plus;
   using fcpp::minus;
   using fcpp::equal;

   LambdaVar<1> F;
   LambdaVar<2> G;
   //LambdaVar<3> H;
   LambdaVar<4> X;
   LambdaVar<5> Y;
   LambdaVar<6> Z;
   Fun0<int> c3 = const_(3);
   List<int> l;

   Fun1<int,double> fctd = construct1<double>();
   
   cout << lambda(X)[ X ](3) << endl;
   cout << lambda(F)[ F ](id)(3) << endl;
   cout << lambda(F)[ F[F] ](id)(3) << endl;
   cout << lambda(F)[ F[F] ](crazy)+1 << endl;
   cout << lambda(X)[ negate[X] ](-3) << endl;
   cout << lambda(X)[ negate[-3] ](5) << endl;
   // The next line is unfortunately allowable; if negate() had an effect,
   // it would happen at the 'wrong' time...
   cout << lambda(X)[ negate(-3) ](5) << endl;
   cout << lambda(F)[ F[F][F] ](id)(3) << endl;
   // The next line is unfortunately allowable, though at least g++
   // gives a warning diagnostic.  Darn comma.  Or darn [].
   //cout << lambda(F)[ F[3,3] ](id) << endl;
   // Same with line below
   //cout << lambda(F)[ F[_,_,l] ](foldl)(plus,3) << endl;
   cout << lambda()[ negate[-3] ]() << endl;
   cout << lambda()[ id ]()(3) << endl;
   cout << lambda(F,X)[ F[X] ](id,3) << endl;
   cout << lambda(F,X)[ F[X] ](id)(3) << endl;
   cout << lambda(F,X)[ F[_,X] ](minus)(2)(5) << endl;
   cout << lambda(F,X)[ F[_,F[X,1]] ](plus,1)(1) << endl;
   cout << lambda(F,X)[ F[_,F[X,1]] ](plus,1)(1) << endl;
   cout << lambda(F,X,Y)[ F[X,Y] ](plus,1,2) << endl;
   cout << lambda(F,X)[ F[F[X,1],1] ](plus,1) << endl;
   cout << lambda(F,X)[ F[1,F[X,1]] ](plus,1) << endl;
   cout << lambda(F,X)[ if0[true,F[X,2],X] ](plus,1) << endl;
   cout << lambda(F,X)[ if0[true,F[X,2],1] ](plus,1) << endl;
   cout << lambda(F,X)[ if0[true,if0[true,F[X,2],1],1] ](plus,1) << endl;
   cout << lambda(F)[ F[_*_] ](const_(3)) << endl;
   cout << lambda(F)[ F[c3[_*_],0] ](plus) << endl;
   cout << lambda(X,G)[ if0[G[X,0],0,3] ](1,equal) << endl;

   // Good error message below
   //cout << lambda(X,G)[ if0[G[X,0],0,plus] ](1,equal) << endl;
   // Good error message below
   //cout << lambda(X,X)[ X ](1,2) << endl;
   // Good error message below
   //cout << lambda(X)[ Y ](1) << endl;
   // Good error message below
   //cout << lambda(Y)[ lambda(X)[ plus[Y,Z] ] ](3)(2) << endl;
   // Good error message below
   //cout << head( map( lambda(X)[ Y ], list_with(1,2) ) ) << endl;

   cout << lambda(Y)[ lambda(X)[ Y ] ](3)(2) << endl;
   cout << lambda(Y,Z)[ lambda(X)[ minus[Y,Z] ] ](4,1)(8) << endl;
   cout << lambda(Z)[ lambda(X,Y)[ minus[Y,Z] ] ](1)(8,4) << endl;
   cout << lambda(F)[ lambda(X,Y,Z)[ minus[Y,Z] ] ](8)(8,4,1) << endl;
   cout << lambda(Y,Z)[ lambda()[ minus[Y,Z] ] ](4,1)() << endl;
   cout << lambda(X)[ lambda(F)[ F[X,X] ][ plus ] ](1)+1 << endl;

   cout << lambda()[ let[ X == 3 ].in[ X ] ]() << endl;
   cout << lambda()[ let[ X==4, Y==minus[X,3] ].in[ minus[X,Y] ] ]() << endl;

   cout << whoa()()+3 << endl;
   cout << shazam()(1)(4)(1) << endl;
   cout << head( lambda(X)[ if1[X,list_with(3,4),NIL] ](true) ) << endl;

   cout << lambda()[ letrec[   // G is (nonlazy) map
      G == lambda(F,X)[ if1[ null[X], X, cons[ F[head[X]], G[F,tail[X]] ] ] ]
      ].in[ G ] ]()( plus(2), list_with(1,2) ).head() << endl;

   cout << thunkFuncToFunc( lambda()[ lambda(X,Y)[ X%plus%Y ] ] )(2,1) << endl;
}
