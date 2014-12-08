#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include "prelude.h"

#ifdef REAL_TIMING
#  include "timer.h"
#else
struct Timer { int ms_since_start() { return 0; } };
#endif

using std::vector;
using std::cin;
using std::cerr;
using std::cout;
using std::endl;

#if __GNUC__ == 2
   extern "C" {
   long int lrand48 (void);
   }
#else
#  include <stdlib.h>
#endif

#ifndef NUM
#define NUM 160000
#endif

using namespace fcpp;

struct Tree {
   int data;
   Tree *left;
   Tree *right;

   Tree( int x ) : data(x), left(0), right(0) {}
   Tree( int x, Tree* l, Tree* r )
      : data(x), left(l), right(r) {}
   bool leaf() const
   { return (left==0) && (right==0); }
};

// add to binary tree
void insert( Tree*& root, int data ) {
   if( root == 0 )
      root = new Tree( data );
   else if( root->data < data )
      insert( root->right, data );
   else
      insert( root->left, data );
}

// make a tree out of a list of numbers
Tree* make_tree( const vector<int>& v ) {
   Tree *t = 0;
   for( vector<int>::const_iterator i = v.begin(); i != v.end(); i++ )
      insert( t, *i );
   return t;
}

struct Fringe : public CFunType<Tree*,OddList<int> > {
   OddList<int> operator()( Tree* t ) const {
      if( t==0 )
         return NIL;
      else if( t->leaf() )
         return cons(t->data,NIL);
      else
         return cat( Fringe()(t->left),
                     curry(Fringe(),t->right) );
   }
};
Fringe fringe;

extern "C" 
void bodyx( Tree* t ) {
      List<int> l = fringe(t);
cout << length(l) << endl;
/*
      l = filter( fcpp::equal(13), l );
      while( !null(l) ) {
         cout << head(l) << endl;
         l = tail(l);
      }
*/
}

extern "C" 
void body( Tree* t ) {
   bodyx(t);
}

int main() {
   int N = NUM;

   Timer timer;

   vector<int> v( N );
   for( int i=0; i<N; i++ )
      v[i] = i;
   std::random_shuffle( v.begin(), v.end() );
   Tree *t = make_tree(v);

   cout << "Num nodes is " << N << endl;

   int start = timer.ms_since_start();
      List<int> l = fringe(t);
/*
cout << length(l) << endl;
*/
//      l = filter( fcpp::equal(13), l );
      l = curry2( filter, fcpp::equal(13), l );
      while( !null(l) ) {
         cout << head(l) << endl;
         l = tail(l);
      }
   int end = timer.ms_since_start();
   cout << "took " << end-start << " ms" << endl;

   return 0;
}
