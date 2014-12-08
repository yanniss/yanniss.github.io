#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#define FCPP_ENABLE_LAMBDA
#include "prelude.h"

using namespace fcpp;

typedef string S;
void print( S s ) {
   cout << s << endl;
}

int main() {
   int x = 3;
   int y = 4;
   int z = x ^plus^ y;
   cout << z << endl;

   Fun0<void> hello = curry( ptr_to_fun(&print), S("hello") );
   cout << "-------" << endl;
   ByNeed<int> xt( before( hello, const_(3) ) );
   ByNeed<int> yt( 4 );
   LambdaVar<1> X;
   LambdaVar<2> Y;
   cout << "-------" << endl;
   ByNeed<int> zt = lambda()[ xt %bind% lambda(X)[
                              yt %bind% lambda(Y)[
                              unitM<ByNeedM>()[ X %plus% Y ] ] ] ]();
   cout << "-------" << endl;
   cout << zt.force() << endl;
   cout << zt.force() << endl;
   cout << "-------" << endl;
   xt = ByNeed<int>( before( hello, const_(3) ) );
   zt = lambda()[ compM<ByNeedM>()[ X %plus% Y | X<=xt, Y<=yt ] ] ();
   cout << "-------" << endl;
   cout << zt.force() << endl;
   cout << zt.force() << endl;

   {
      cout << "-------------------" << endl;
      // expensive negate
      Fun1<int,int> exp_neg = 
         before( curry( ptr_to_fun(&print), S("expensive") ), negate );
      int z = plus( exp_neg(3), 4 );
      cout << z << endl;
   
      cout << "-------" << endl;
      ByNeed<int> bx( 3 );
      ByNeed<int> by( 4 );
      ByNeed<int> bz( liftM2<ByNeedM>()(plus)( liftM<ByNeedM>()(exp_neg)(bx),
                                               by ) );
      cout << "..." << endl;
      cout << bz.force() << endl;

      cout << "-------------------" << endl;
      bz = ByNeed<int>( before( hello, const_(0) ) );
      cout << liftM3<ByNeedM>()(ignore(plus))(bz,bx,bDelay(4)).force() 
                << endl;
   }

   {
      cout << "==========" << endl;
      Fun1<int,int> p = before( hello, plus(1) );
      zt = lambda()[ compM<ByNeedM>()[ X %plus% Y | 
                                       X<=bLift(p)[3], Y<=yt ] ] ();
                                       //X<=bDelay[p[3]], Y<=yt ] ] ();
      cout << "==========" << endl;
      cout << zt.force() << endl;
      cout << zt.force() << endl;
      cout << "==========" << endl;

      Fun2<int,int,int> p2 = before( hello, plus );
      zt = lambda()[ compM<ByNeedM>()[ X | X<=bLift(p2)[3][4] ] ] ();
      cout << "==========" << endl;
      cout << zt.force() << endl;
      cout << zt.force() << endl;
      cout << "==========" << endl;

      Fun3<int,int,int,int> p3 = before( hello, ignore(plus) );
      zt = lambda()[ compM<ByNeedM>()[ X | X<=bLift(p3)[1][3][4] ] ] ();
      cout << "==========" << endl;
      cout << zt.force() << endl;
      cout << zt.force() << endl;
      cout << "==========" << endl;

      Fun0<int> p0 = before( hello, curry2(plus,3,4) );
      zt = lambda()[ compM<ByNeedM>()[ X | X<=bLift(p0)[_*_] ] ] ();
      cout << "==========" << endl;
      cout << zt.force() << endl;
      cout << zt.force() << endl;
      cout << "==========" << endl;
   }
}
