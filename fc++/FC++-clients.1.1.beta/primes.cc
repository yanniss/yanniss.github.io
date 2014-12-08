#include <iostream>
#include "prelude.h"

struct Divisible : public CFunType<int,int,bool> {
   bool operator()( int x, int y ) const { return x%y==0; }
};
Divisible divisible;

struct Factors : public CFunType<int,List<int> > {
   List<int> operator()( int x ) const {
      return filter( curry2(divisible,x), enumFromTo(1,x) );
   }
};
Factors factors;

struct Prime : public CFunType<int,bool> {
   bool operator()( int x ) const { 
      return factors(x)==list_with(1,x); 
   }
};
Prime prime;

struct Primes : public CFunType<int,List<int> > {
   List<int> operator()( int n ) const {
      return take( n, filter( prime, enumFrom(1) ) );
   }
};
Primes primes;

int main() {
   List<int> l = primes(20);
   while( l ) {
      cout << head(l) << " ";
      l = tail(l);
   }
   cout << endl;

   return 0;
}

