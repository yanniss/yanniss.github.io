#include <iostream>
using std::cout;
using std::endl;

#include <string>

#include "prolog.h"
using namespace lcpp;

#include "prelude.h"

struct Node : public lcpp::Functor<Node,int> {
   static const char * const name;
} node;
const char * const Node::name = "node";

struct Edge : public lcpp::Functor<Edge,int,int> {
   static const char * const name;
} edge;
const char * const Edge::name = "edge";

struct Cons : public lcpp::Functor<Cons,bool,int,Cons> {
   static const char * const name;
} cons;
const char * const Cons::name = "cons";

struct Length : public lcpp::Functor<Length,Cons,int> {
   static const char * const name;
} length;
const char * const Length::name = "length";

struct Contains : public lcpp::Functor<Contains,Cons,int> {
   static const char * const name;
} contains;
const char * const Contains::name = "contains";

struct IsEmpty : public lcpp::Functor<IsEmpty,Cons> {
   static const char * const name;
} empty;
const char * const IsEmpty::name = "empty";

struct Member : public lcpp::Functor<Member,int,List<int> > {
   static const char * const name;
} member;
const char * const Member::name = "member";

struct Print {
   template <class T> struct Sig : public fcpp::FunType<T,int> {};
   template <class T>
   int operator()( const T& x ) const {
      std::cout << x;
      return 0;
   }
} print;

#define DECLARE(name,type,num) \
   typedef lcpp::LogicVariable<type,num> name ## _TYPE; \
   const name ## _TYPE& name = name ## _TYPE::instance( #name );

int main() {
   try {
      DECLARE(D,int,0);
/*
      typedef lcpp::LogicVariable<int,0> D_TYPE;
      const D_TYPE& D = D_TYPE::instance("D");
*/
      typedef lcpp::LogicVariable<int,1> X_TYPE;
      const X_TYPE& X = X_TYPE::instance("X");
      typedef lcpp::LogicVariable<Cons,7> L_TYPE;
      const L_TYPE& L = L_TYPE::instance("L");

      std::cout << "Now for the list stuff" << std::endl;
      lassert( empty( cons(false,_,_) ) );
      std::cout << "1" << std::endl;
      lassert( length(L,0) -= empty(L) );
      std::cout << "2" << std::endl;
      lassert( length(cons(true,_,L),X) -= 
                  length(L,D) && X.is(fcpp::plus,D,1) );
      std::cout << "3" << std::endl;
      lassert( contains(cons(true,X,_),X) );
      std::cout << "4" << std::endl;
      lassert( contains(cons(true,_,L),X) -= contains(L,X) );
      std::cout << "4.5" << std::endl;
      query( length( cons(false,_,_), X ) );

      std::cout << "5" << std::endl;
      typedef QRT<X_TYPE>::IE RE;
      List<RE> lre = 
         lquery( length( cons(true,1,cons(true,1,cons(false,_,_))), X ) );
      cout << "solns for X:" << endl;
      while( lre ) {
         cout << "soln for X" << endl;
         if( fcpp::head(lre)->at( X ) )
            cout << " - " << *fcpp::head(lre)->at(X) << endl;
         else
            cout << " - " << "unbound" << endl;
         lre = fcpp::tail(lre);
      }
      cout << "done" << endl;

      std::cout << "5.5" << std::endl;
      typedef QRT<X_TYPE,D_TYPE>::IE RE2;
      std::pair<RE2,List<Empty> > p2 =
        query( X==3 || X==4 && D==5 );
      List<Empty> list = p2.second;
      RE2 env2 = p2.first;
      cout << "solns for X:" << endl;
      while( list ) {
         cout << "soln for X" << endl;
         if( env2->at( X ) )
            cout << *env2->at( X ) << endl;
         else
            cout << "unbound" << endl;
         cout << "soln for d" << endl;
         if( env2->at( D ) )
            cout << *env2->at( D ) << endl;
         else
            cout << "unbound" << endl;
         list = fcpp::tail(list);
      }
      cout << "done" << endl;
      
      std::cout << "6" << std::endl;
      query( length( cons(true,1,cons(false,_,_)), X ) );
      std::cout << "7" << std::endl;
      query( length( cons(true,1,cons(true,1,cons(false,_,_))), X ) );
      std::cout << "8" << std::endl;
      query( contains( cons(true,5,cons(true,4,cons(false,_,_))), X ) );
      std::cout << "9" << std::endl;
      iquery( X==3 || X==4 && D==5 );
      std::cout << "10" << std::endl;
      iquery( X==3 || fail );
      std::cout << "11" << std::endl;
      iquery( X==3 && not_provable(X==4) );
      std::cout << "12" << std::endl;
      iquery( X==3 && not_provable(D==4) );
      std::cout << "13" << std::endl;
      iquery( X==3 && fail );
      std::cout << "14" << std::endl;
      iquery( _.is( print,std::string("hello world!\n") ) );
      std::cout << "15" << std::endl;
      iquery( X.is( fcpp::const_(2) ) );

      typedef List<int> LI;
      typedef lcpp::LogicVariable<List<int>,8> I_TYPE;
      const I_TYPE& I = I_TYPE::instance("I");
      typedef lcpp::LogicVariable<List<int>,10> J_TYPE;
      const J_TYPE& J = J_TYPE::instance("J");
      typedef lcpp::LogicVariable<bool,9> B_TYPE;
      const B_TYPE& B = B_TYPE::instance("B");
      lassert( member( X, I ) -= B.is(fcpp::null,I) && B==false
         && D.is(fcpp::head,I) && X==D );
      lassert( member( X, I ) -= B.is(fcpp::null,I) && B==false
         && J.is(fcpp::tail,I) && member(X,J) );
      std::cout << "16" << std::endl;
      iquery( member( X, LI( fcpp::enumFromTo(1,3) ) ) );

      std::cout << "17" << std::endl;
      typedef lcpp::LogicVariable<Edge,21> E_TYPE;
      const E_TYPE& E = E_TYPE::instance("E");
      typedef QRT<E_TYPE>::IE RE3;
      List<RE3> lre3 = lquery( E == edge(1,2) );
      while( lre3 ) {
         FunctorFacade<Edge,int,int> e = fcpp::head(lre3)->at(E);
         std::cout << " - E = egde(" 
            << *e->arg1() << "," << *e->arg2() << ")" << std::endl;
         lre3 = fcpp::tail(lre3);
      }
      std::cout << "18" << std::endl;
      typedef QRT<E_TYPE,X_TYPE,D_TYPE>::IE RE4;
      iquery( succeed && (X==3 || D==3) && E == edge(X,X) );
   }
   catch( const char* s ) {
      std::cout << "exception: " << s << std::endl;
   }
   catch( ... ) {
      std::cout << "exception of some kind" << std::endl;
   }
}
