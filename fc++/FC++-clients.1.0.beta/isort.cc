#include <iostream>
#include <vector>
#include <algorithm>
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

struct Fringe : public CFunType<Tree*,List<int> > {
   List<int> operator()( Tree* t ) const {
      if( t==0 )
         return Nil<int>();
      else if( t->leaf() ) {
cout << "I have actually bothered to inspect leaf " << t->data << endl;
         return OneElement()(t->data);
      }
      else
         return cat( curry(Fringe(),t->left), 
                     curry(Fringe(),t->right) );
   }
};

Fringe fringe;
      
struct Double : public CFunType<int,int> {
   int operator()( int x ) const {
      return 2*x;
   }
};

Double twice;

struct Insert : public CFunType<int,List<int>,List<int> > {
   List<int> operator()( int x, const List<int>& l ) const {
      if( null(l) || (x > head(l)) )
         return cons( x, l );
      else
         return cons( head(l), curry2(Insert(),x,tail(l)) );
   }
};

struct Isort : public CFunType<List<int>,List<int> > {
   List<int> operator()( const List<int>& l ) const {
      return foldr( Insert(), List<int>(), l );
   }
};

int main() {
   int N;
   cout << "Num nodes?" << endl;
//   cin >> N;
   N = 20;

   vector<int> v( N );
   for( int i=0; i<N; i++ )
      v[i] = i;
   random_shuffle( v.begin(), v.end() );
   Tree *t = make_tree(v);

   List<int> list = fringe(t);
/*
   List<int> l = map( twice, list );
   while( l ) {
      cerr << head(l) << endl;
      l = tail(l);
   }
   cerr << endl;
   cout << foldr( Plus(), 0, list ) << endl;
*/
   List<int> l = Isort()( list );
   while( l ) {
      cerr << head(l) << endl;
      l = tail(l);
   }
   cerr << endl;

   return 0;
}

