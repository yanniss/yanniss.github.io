#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include "timer.h"
#include "prelude.h"

extern "C" {
long int lrand48 (void);
}

template <class T>
struct Tree {
   typedef T WrappedType;
   T data;
   Ref<Tree<T> > left, right;
   
   Tree( T x ) : data(x), left(0), right(0) {}
   bool leaf() const { return (left==0) && (right==0); }
};

// add to binary tree
void insert( Ref<Tree<int> >& root, int data ) {
   if( root == 0 )
      root = Ref<Tree<int> >( new Tree<int>( data ) );
   else if( root->data < data )
      insert( root->right, data );
   else
      insert( root->left, data );
}

// make a tree out of a list of numbers
Ref<Tree<int> > make_tree( const vector<int>& v ) {
   Ref<Tree<int> > t(0);
   for( vector<int>::const_iterator i = v.begin(); i != v.end(); i++ )
      insert( t, *i );
   return t;
}


template <class T>
struct Fringe : public CFunType<Ref<Tree<T> >,List<T> > {
   List<T> operator()( Ref<Tree<T> > t ) const {
      if( t==0 )
         return List<T>(); 
      else if( t->leaf() ) {
         return one_element(t->data);
      }
      else
         return cat( curry(Fringe(),t->left), curry(Fringe(),t->right) );
   }
};
      
int main() {
   int N;
   cout << "Num nodes?" << endl;
   cin >> N;

   Timer timer;

   vector<int> v( N );
   for( int i=0; i<N; i++ )
      v[i] = i;
   random_shuffle( v.begin(), v.end() );
   Ref<Tree<int> > t = make_tree(v);

   cout << "lazy results" << endl;
   int start = timer.ms_since_start();
      List<int> l = Fringe<int>()(t);
      while( l ) {
         cerr << head(l) << " ";
         l = tail(l);
      }
      cerr << endl;
   int end = timer.ms_since_start();
   cout << "took " << end-start << " ms" << endl;

   return 0;
}

