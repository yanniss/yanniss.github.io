// The program here is based on an example from
//    "Designing and Implementing Combinator Languages"
// by S. Doaitse Swierstra, Pablo R. Azero Alcocer, Joao Saraiva

#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#define FCPP_ENABLE_LAMBDA
#include "prelude.h"

using std::vector;
using std::cin;
using std::cerr;
using std::cout;
using std::endl;

using namespace fcpp;

#ifndef NUM
#define NUM 1000
#endif

#ifdef REAL_TIMING
#  include "timer.h"
#else
struct Timer { int ms_since_start() { return 0; } };
#endif

int the_count;

class Tree {
   int data_;
   Tree *left_;
   Tree *right_;
   bool pr_leaf() const { return (left_==0) && (right_==0); }
public:
   Tree( int x ) : data_(x), left_(0), right_(0) {}
   Tree( Tree* l, Tree* r ) : data_(0), left_(l), right_(r) {}
   bool leaf() const { ++the_count; return pr_leaf(); }
   int data() const { if( !pr_leaf() ) throw "bad"; return data_; }
   Tree* left() const { if( pr_leaf() ) throw "bad"; return left_; }
   Tree* right() const { if( pr_leaf() ) throw "bad"; return right_; }
};

struct XMkLeaf : public CFunType<int,Tree*> {
   Tree* operator()( int x ) const { return new Tree(x); }
};
typedef Full1<XMkLeaf> MkLeaf;
MkLeaf mkLeaf;

struct XMkBin : public CFunType<Tree*,Tree*,Tree*> {
   Tree* operator()( Tree* l, Tree* r ) const { 
      return new Tree(l,r); 
   }
};
typedef Full2<XMkBin> MkBin;
MkBin mkBin;

int sum_tree( Tree* root ) {
   if( root->leaf() )
      return root->data();
   else
      return sum_tree( root->left() ) + sum_tree( root->right() );
}

//////////////////////////////////////////////////////////////////////

template <class A>
struct TreeAlgebra {
   typedef A Type;
   Fun1<int,A> leaf;
   Fun2<A,A,A> bin;
   TreeAlgebra( Fun1<int,A> a, Fun2<A,A,A> b ) : leaf(a), bin(b) {}
};

struct XCataTree {
   template <class TA, class T> struct Sig 
   : public FunType<TA,T,typename TA::Type> {};

   template <class A>
   A operator()( const TreeAlgebra<A>& alg, Tree* t ) const {
      if( t->leaf() )
         return alg.leaf( t->data() );
      else
         return alg.bin( XCataTree()(alg,t->left()),
                         XCataTree()(alg,t->right()) );
   }
};
typedef Full2<XCataTree> CataTree;
CataTree cata_tree;

// Basic Tree algebras
TreeAlgebra<int> sum_alg( id, fcpp::plus );
TreeAlgebra<int> min_alg( id, fcpp::min );

//////////////////////////////////////////////////////////////////////
// obvious solution (Listing 3)

LambdaVar<11> LFUN;
LambdaVar<12> RFUN;
LambdaVar<13> M;
LambdaVar<14> I;

TreeAlgebra<Fun1<int,Tree*> > rep_alg( ignore( mkLeaf ),
   lambda(LFUN,RFUN)[ lambda(M)[ mkBin[ LFUN[M], RFUN[M] ] ] ] );

Tree* replace_min3( Tree* t ) {
   return cata_tree(rep_alg,t)( cata_tree(min_alg,t) );
}

//////////////////////////////////////////////////////////////////////
// tupling solution (Listing 4)

struct XTupleTree {
   template <class TA, class TB>
   struct Sig : public FunType<TA,TB,
      TreeAlgebra<std::pair<typename TA::Type,typename TB::Type> > > {};

   template <class A, class B>
   TreeAlgebra<std::pair<A,B> >
   operator()( const TreeAlgebra<A>& x, const TreeAlgebra<B>& y ) const {
      LambdaVar<1> L;
      LambdaVar<2> R;
      return TreeAlgebra<std::pair<A,B> >(
        fcpp::compose2( makePair, x.leaf, y.leaf ),
        lambda(L,R)[ makePair[ x.bin[fst[L],fst[R]], y.bin[snd[L],snd[R]] ] ] );
   }
};
typedef Full2<XTupleTree> TupleTree;
TupleTree tuple_tree;

TreeAlgebra<std::pair<int,Fun1<int,Tree*> > > 
   min_tup_rep( min_alg ^tuple_tree^ rep_alg );

Tree* replace_min4( Tree* t ) {
   std::pair<int,Fun1<int,Tree*> > res = cata_tree(min_tup_rep,t);
   return res.second( res.first );
}

//////////////////////////////////////////////////////////////////////
// merging tupled functions (Listing 5)

LambdaVar<21> L;
LambdaVar<22> L_M;
LambdaVar<23> L_T;
LambdaVar<24> R;
LambdaVar<25> R_M;
LambdaVar<26> R_T;

// Lift the tree and its integers into the ByNeed monad for laziness
typedef ByNeed<int> BNI;
typedef ByNeed<Tree*> BNT;

TreeAlgebra<Fun1<BNI,std::pair<BNI,BNT> > > merged_alg( 
   lambda(I)[ lambda(M)[ makePair[I,liftM<ByNeedM>()(mkLeaf)[M]] ] ], 
   lambda(LFUN,RFUN)[ lambda(M)[ let[ L == LFUN[M], R == RFUN[M],
      L_M == fst[L], L_T == snd[L], R_M == fst[R], R_T == snd[R] ].in[
         makePair[ L_M %liftM2<ByNeedM>()(min)% R_M,
         liftM2<ByNeedM>()(mkBin)[L_T,R_T] ] ] ] ] );

Tree* replace_min5( Tree* t ) {
   // C++ doesn't do recursive definitions, so we utilize laziness to tie
   // the knot correctly:
   std::pair<BNI,BNT> p = cata_tree( merged_alg, t )
         ( BNI( lambda()[ bForce[ dereference[&p.first] ] ] ) );
   return bForce( p.second );
}

//////////////////////////////////////////////////////////////////////

int main() {
   Tree *t = new Tree( new Tree(3), new Tree( new Tree(4), new Tree(5) ) );
   t = new Tree( t, t );
   t = new Tree( t, t );
   t = new Tree( t, t );
   t = new Tree( t, t );
/* Move this comment-window around to change the tree size
   t = new Tree( t, t );
   t = new Tree( t, t );
*/
   cout << "Sum nodes is " << sum_tree(t) << endl;
   cout << "Sum nodes is " << cata_tree(sum_alg,t) << endl;

   Tree *tmp;
   Timer timer;
   int start, end;

   // warm up the cache
   tmp = replace_min3(t);
   cout << "After repl_min3, sum is " << sum_tree(tmp) << endl;

   start = timer.ms_since_start();
   for( int i=0; i<NUM; ++i ) {
      the_count = 0;
      tmp = replace_min3(t);
      cout << "Count is now " << the_count << endl;
      cout << "After repl_min3, sum is " << sum_tree(tmp) << endl;
   }
   end = timer.ms_since_start();
   cerr << "took " << end-start << " ms" << endl;

   start = timer.ms_since_start();
   for( int i=0; i<NUM; ++i ) {
      the_count = 0;
      tmp = replace_min4(t);
      cout << "Count is now " << the_count << endl;
      cout << "After repl_min4, sum is " << sum_tree(tmp) << endl;
   }
   end = timer.ms_since_start();
   cerr << "took " << end-start << " ms" << endl;

   start = timer.ms_since_start();
   for( int i=0; i<NUM; ++i ) {
      the_count = 0;
      tmp = replace_min5(t);
      cout << "Count is now " << the_count << endl;
      cout << "After repl_min5, sum is " << sum_tree(tmp) << endl;
   }
   end = timer.ms_since_start();
   cerr << "took " << end-start << " ms" << endl;

   return 0;
}

