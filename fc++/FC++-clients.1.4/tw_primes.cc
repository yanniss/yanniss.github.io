#include <iostream>
#include "prelude.h"
using std::cout; using std::endl;
using namespace fcpp;

#ifndef NUM
#define NUM 1000
#endif


struct Divisible : public CFunType<int,int,bool> {
   bool operator()( int x, int y ) const
   { return x%y==0; }
} divisible;

struct Factors : public CFunType<int,OddList<int> > {
   OddList<int> operator()( int x ) const {
      return filter( curry2(divisible,x),
                     enumFromTo(1,x) );
   }
} factors;

struct Prime : public CFunType<int,bool> {
   bool operator()( int x ) const {
      return factors(x)==cons(1,cons(x,NIL));
   }
} prime;

struct Primes : public CFunType<int,OddList<int> > {
   OddList<int> operator()( int n ) const {
      return take( n, filter( prime, enumFrom(1) ) );
   }
} primes;


#ifdef REAL_TIMING
#  include "timer.h"
#else
struct Timer { int ms_since_start() { return 0; } };
#endif

int main() {
   Timer timer;
   cout << "Num primes = " << NUM << endl;
   int start = timer.ms_since_start();
      List<int> l = primes(NUM);
      cout << at( l, NUM-1 ) << endl;
   int end = timer.ms_since_start();
   cout << "took " << end-start << " ms" << endl;
   cout << endl;
}

