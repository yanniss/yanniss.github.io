#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::cin;

#define FCPP_ENABLE_LAMBDA
#include "prelude.h"
using namespace fcpp;



struct Animal {
   virtual void speak() = 0;
   virtual ~Animal() {}
};

struct Dog : public Animal {
   void speak() { cout << "Woof!" << endl; }
};

int main() {
   int *p = new1<int>()( 5 );
   cout << dereference( addressOf( *p ) ) << endl;
   delete_(p);   
   cout << (2 ^plus^ 3) << endl;
   cout << (6 ^minus^ 1) << endl;
   cout << (5 ^multiplies^ 1) << endl;
   cout << (10 ^divides^ 2) << endl;
   cout << (19 ^modulus^ 7) << endl;
   cout << "-----------------------------------" << endl;
   cout << (1 ^less^ 2) << endl;
   cout << (1 ^lessEqual^ 1) << endl;
   cout << (2 ^greater^ 1) << endl;
   cout << (2 ^greaterEqual^ 2) << endl;
   cout << (2 ^equal^ 2) << endl;
   cout << (2 ^notEqual^ 3) << endl;
   cout << logicalNot( false ^logicalAnd^ true ) << endl;
   cout << logicalNot( false ^logicalOr^ false ) << endl;
   cout << "-----------------------------------" << endl;
   cout << construct1<float>()(5) << endl;
   cout << "-----------------------------------" << endl;
   Animal *a = new0<Dog>()();
   ptr_to_fun( &Animal::speak )( a );
   ptr_to_fun( &Animal::speak )( dynamicCast<Dog*>()(a) );
   cout << "-----------------------------------" << endl;
   &cout ^outStream^ 5 ^outStream^ makeManip(cout)(endl);
   &cout ^outStream^ "The answer is " ^outStream^ 5 
      ^outStream^ makeManip(cout)(endl);
   const char* s = "X is ";
   LambdaVar<1> X;
   lambda(X)[ &cout %outStream% s %outStream% X 
              %outStream% makeManip(cout)(endl) ](5);
   Fun1<int,std::ostream*> f = lambda(X)[ &cout %outStream% s %outStream% X 
              %outStream% makeManip(cout)(endl) ];
   f(5);
   s = "17(hex) is ";
   &cout ^outStream^ s ^outStream^ makeManip(cout)(std::hex) 
         ^outStream^ 17 ^outStream^ makeManip(cout)(endl);
   cout << "-----------------------------------" << endl;
   int x;
   cout << "Enter a number: " << endl;
   &cin ^inStream^ &x;
   cout << "You said " << x << endl;
}
