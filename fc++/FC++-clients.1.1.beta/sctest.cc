#include <iostream>
#include <utility>
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
         return Nil<int>();
      else if( t->leaf() ) {
//cout << "I have actually bothered to inspect leaf " << t->data << endl;
         return one_element(t->data);
      }
      else
         return cat( curry(Fringe(),t->left), curry(Fringe(),t->right) );
   }
};
      
template <class T>
ostream& operator<<( ostream& o, List<T> l ) {
   while( l ) {
      o << head(l) << " ";
      l = tail(l);
   }
   return o;
}

ostream& operator<<( ostream& o, pair<int,int> p ) {
   o << "(" << p.first << "," << p.second << ")";
   return o;
}

struct Double : public CFunType<int,int> {
   int operator()( int x ) const {
      return 2*x;
   }
};

int main() {
   int N = 20;
   vector<int> v( N );
   for( int i=0; i<N; i++ )
      v[i] = i;
   random_shuffle( v.begin(), v.end() );
   Tree *t = make_tree(v);

   Plus plus;
   Greater greater;
   And and;
   Or or;

   List<int> l = Fringe()(t);
   cout << "Original   :" << l << endl;
   List<List<int> > ll;
   ll = cons( l, ll );
   ll = cons( l, ll );
   cout << "Concat     :" << concat(ll) << endl;
   cout << "Init       :" << init(l) << endl;
   cout << "Last       :" << last(l) << endl;
   cout << "Length     :" << length(l) << endl;
   cout << "At(2)      :" << at(l,2) << endl;
   cout << "Filter >10 :" << filter( bind2of2( greater, 10 ), l) << endl;
   cout << "Foldr +    :" << foldr(plus,0,l) << endl;
   cout << "Foldr1 +   :" << foldr1(plus,l) << endl;
   cout << "Foldl +    :" << foldl(plus,0,l) << endl;
   cout << "Foldl1 +   :" << foldl1(plus,l) << endl;
   cout << "Scanr +    :" << scanr(plus,0,l) << endl;
   cout << "Scanr1 +   :" << scanr1(plus,l) << endl;
   cout << "Scanl +    :" << scanl(plus,0,l) << endl;
   cout << "Scanl1 +   :" << scanl1(plus,l) << endl;
   cout << "Drop 5     :" << drop(5,l) << endl;

   cout << "Take 5 of Iterate double 1:" << take(5,iterate(Double(),1)) << endl;
   cout << "Take 5 of repeat 1        :" << take(5,repeat(1)) << endl;

   pair<List<int>,List<int> > p = splitAt( 5, l );
   cout << "SplitAt 5 a:" << p.first << endl;
   cout << "SplitAt 5 b:" << p.second << endl;

   p = span( bind2of2( greater, 10 ), l );
   cout << "Span >10  a:" << p.first << endl;
   cout << "Span >10  b:" << p.second << endl;

   p = Break()( bind2of2( greater, 10 ), l );
   cout << "Break>10  a:" << p.first << endl;
   cout << "Break>10  b:" << p.second << endl;
   cout << "Reverse    :" << Reverse()(l) << endl;
   List<bool> b = map( Odd(), l );
   cout << "And Odd    :" << and(b) << endl;
   cout << "Or  Odd    :" << or(b) << endl;
   cout << "And Odd 0  :" << and(take(1,b)) << endl;
   cout << "Or  Odd 0  :" << or(take(1,b)) << endl;
   cout << "Any >10    :" << any( bind2of2(greater,10), l) << endl;
   cout << "All >10    :" << all( bind2of2(greater,10), l) << endl;
   cout << "Any >-10   :" << any( bind2of2(greater,-10), l) << endl;
   cout << "All >-10   :" << all( bind2of2(greater,-10), l) << endl;
   cout << "Elem 10    :" << elem(10,l) << endl;
   cout << "NotElem 10 :" << notElem(10,l) << endl;
   cout << "Sum        :" << sum(l) << endl;
   cout << "Product    :" << product(l) << endl;
   cout << "Maximum    :" << maximum(l) << endl;
   cout << "Minimum    :" << minimum(l) << endl;
   cout << "Zip w/ 1s  :" << zip(l,repeat(1)) << endl;
   cout << "Zip w/ +1s :" << zipWith(plus,l,repeat(1)) << endl;

   return 0;
}
   
