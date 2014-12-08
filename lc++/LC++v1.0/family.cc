#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "prolog.h"
using namespace lcpp;

#include "prelude.h"
using fcpp::plus;
using fcpp::List;
using fcpp::head;
using fcpp::tail;
using fcpp::null;

FUN2(parent,string,string)
FUN2(father,string,string)
FUN2(mother,string,string)
FUN2(child,string,string)
FUN1(male,string)
FUN1(female,string)
FUN2(sibling,string,string)
FUN2(brother,string,string)
FUN2(sister,string,string)
FUN3(ancestor,string,string,int)

int main() {
   string bart="bart", lisa="lisa", maggie="maggie", marge="marge",
      homer="homer", abraham="abraham";
   try {
      DECLARE(Mom, string,0);
      DECLARE(Dad, string,1);
      DECLARE(Kid, string,2);
      DECLARE(Par, string,3);
      DECLARE(Bro, string,4);
      DECLARE(Sis, string,5);
      DECLARE(Sib, string,6);
      DECLARE(Sib2,string,7);
      DECLARE(Anc, string,8);
      DECLARE(Tmp, string,9);
      DECLARE(X   ,int,  10);
      DECLARE(Y   ,int,  11);

      lassert( male(bart) );
      lassert( male(homer) );
      lassert( male(abraham) );
      lassert( female(lisa) );
      lassert( female(maggie) );
      lassert( female(marge) );

      lassert( parent(marge,bart) );
      lassert( parent(marge,lisa) );
      lassert( parent(marge,maggie) );
      lassert( parent(homer,bart) );
      lassert( parent(homer,lisa) );
      lassert( parent(homer,maggie) );
      lassert( parent(abraham,homer) );

      lassert( mother(Mom,Kid) -= parent(Mom,Kid) && female(Mom) );
      lassert( father(Dad,Kid) -= parent(Dad,Kid) && male(Dad) );
      lassert( child(Kid,Par) -= parent(Par,Kid) );
      lassert( sibling(Sib,Sib2) -= father(Dad,Sib) && father(Dad,Sib2)
                  && mother(Mom,Sib) && mother(Mom,Sib2) 
                  && not_provable( Sib==Sib2 ) );
      lassert( brother(Bro,Sib) -= sibling(Bro,Sib) && male(Bro) );
      lassert( sister(Sis,Sib) -= sibling(Sis,Sib) && female(Sis) );
      lassert( ancestor(Par,Kid,1) -= parent(Par,Kid) );
      lassert( ancestor(Anc,Kid,X) -= parent(Anc,Tmp) &&
                  ancestor(Tmp,Kid,Y) && X.is(plus,Y,1) );
     
      iquery( father(Dad,Kid) );
      iquery( sibling(maggie,Sib2) );
      iquery( ancestor(Anc,bart,X) );

      std::cout << "---------------" << std::endl;
      typedef QRT<Anc_TYPE,X_TYPE>::IE IE;
      List<IE> l = lquery( ancestor(Anc,bart,X) );
      while( !null(l) ) {
         IE env = head(l);
         env->show();
         l = tail(l);
      }

      std::cout << "---------------" << std::endl;
      typedef QRT<Anc_TYPE,X_TYPE>::IE IE;
      std::pair<IE,List<Empty> > p = query( ancestor(Anc,bart,X) );
      IE env = p.first;
      while( !null(p.second) ) {
         std::cout << "X is " << *env->at(X) << " and Anc is " 
            << *env->at(Anc) << std::endl;
         p.second = tail(p.second);
      }
   }
   catch( const char* s ) {
      std::cout << "exception: " << s << std::endl;
   }
   catch( ... ) {
      std::cout << "exception of some kind" << std::endl;
   }
}
