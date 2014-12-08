#include <iostream>
#include <cmath>
#include "prelude.h"

using namespace fcpp;

using std::cin;
using std::cout;
using std::endl;

struct improve : public CFunType<double,double,double> {
   double operator()( double x, double y ) const { 
      return 0.5*(y+x/y);
   }
};

struct sqtol : public CFunType<double,bool> {
   double tolerance;
   double goal;
   sqtol( double g, double t ) : tolerance(t), goal(g) {}
   bool operator()( double try_me ) const { 
      return std::abs(try_me*try_me - goal) <= tolerance;
   }
};

int main() {
   double x;

   cout << "Enter number to find sqrt: " << endl;
   cin >> x;

   List<double> nums( listUntil( Never1(), curry2(improve(),x), 1.0));

   List<double> l = nums;
   for( int i=0; i<20; i++ ) {
      cout << head(l) << " ";
      l = tail(l);
   }
   cout << endl;

   double tol;
   cout << "Enter tolerance, and I'll show first value that meets it" << endl;
   cin >> tol;
   cout << until( sqtol(x,tol), curry2(improve(),x), 1.0) << endl;

   return 0;
}

