#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include "timer.h"
#include "prelude.h"

extern "C" {
long int lrand48 (void);
}

struct Tree {
   int data;
   Tree *left;
   Tree *right;
   
   Tree( int x ) : data(x), left(0), right(0) {}
   Tree( int x, Tree* l, Tree* r ) : data(x), left(l), right(r) {}
   bool leaf() const { return (left==0) && (right==0); }
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

Tree* tree_a() {                     //   2        |
   int a[] = { 2, 1, 4, 3, 5 };      //  / \       |
   vector<int> v(a,&a[5]);           // 1   4      |
   return make_tree(v);              //    / \     |
}                                    //   3   5    |

Tree* tree_b() {                     //     4      |
   int a[] = { 4, 2, 1, 3, 5 };      //    / \     |
   vector<int> v(a,&a[5]);           //   2   5    |
   return make_tree(v);              //  / \       |
}                                    // 1   3      |

vector<int> strict_fringe( Tree* t ) {
   if( t == 0 )
      return vector<int>();
   else if( t->leaf() ) {
      vector<int> v;
      v.push_back( t->data );
      return v;
   }
   else {
      vector<int> l = strict_fringe( t->left );
      vector<int> r = strict_fringe( t->right );
      copy( r.begin(), r.end(), back_inserter( l ) );
      return l;
   }
}

struct Fringe : public CFunType<Tree*,List<int> > {
   List<int> operator()( Tree* t ) const {
      if( t==0 )
         return List<int>();  //Nil<int>();
      else if( t->leaf() ) {
//cout << "I have actually bothered to inspect leaf " << t->data << endl;
         return one_element(t->data);
      }
      else
         return cat( curry(Fringe(),t->left), curry(Fringe(),t->right) );
   }
};
Fringe fringe;
      
int main() {
   int N;
   cout << "Num nodes?" << endl;
   cin >> N;

   Timer timer;

   vector<int> v( N );
   for( int i=0; i<N; i++ )
      v[i] = i;
   random_shuffle( v.begin(), v.end() );
   Tree *t = make_tree(v);

   cout << "vector results" << endl;
   int start = timer.ms_since_start();
      vector<int> vf = strict_fringe(t);
      copy( vf.begin(), vf.end(), ostream_iterator<int>( cerr, " " ) );
      cerr << endl;
   int end = timer.ms_since_start();
   cout << "took " << end-start << " ms" << endl;
   
   cout << "lazy results" << endl;
   start = timer.ms_since_start();
      List<int> l = fringe(t);
      while( l ) {
         cerr << head(l) << " ";
         l = tail(l);
      }
      cerr << endl;
   end = timer.ms_since_start();
   cout << "took " << end-start << " ms" << endl;

   Fun1<Tree*,List<int> > mono_fringe = makeFun1( fringe );

   cout << "slow and lazy results :)" << endl;
   start = timer.ms_since_start();
      l = mono_fringe(t);
      while( l ) {
         cerr << head(l) << " ";
         l = tail(l);
      }
      cerr << endl;
   end = timer.ms_since_start();
   cout << "took " << end-start << " ms" << endl;

   return 0;
}

