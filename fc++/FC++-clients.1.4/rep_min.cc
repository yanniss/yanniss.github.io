// The program here is based on an example from
//    "Designing and Implementing Combinator Languages"
// by S. Doaitse Swierstra, Pablo R. Azero Alcocer, Joao Saraiva

#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include "prelude.h"

using std::vector;
using std::cin;
using std::cerr;
using std::cout;
using std::endl;

using namespace fcpp;

#ifndef N
#define N 1000
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

struct CataTree {
   template <class TA, class T> struct Sig 
   : public FunType<TA,T,typename TA::Type> {};

   template <class A>
   A operator()( const TreeAlgebra<A>& alg, Tree* t ) const {
      if( t->leaf() )
         return alg.leaf( t->data() );
      else
         return alg.bin( CataTree()(alg,t->left()),
                         CataTree()(alg,t->right()) );
   }
} cata_tree_;
Curryable2<CataTree> cata_tree(cata_tree_);

//////////////////////////////////////////////////////////////////////
// obvious solution (Listing 3)

TreeAlgebra<int> sum_alg( id, fcpp::plus );
TreeAlgebra<int> min_alg( id, fcpp::min );
TreeAlgebra<Fun1<int,Tree*> > rep_alg( ignore(New1<Tree>()),
                                       fcpp::compose2(New2<Tree>()) );

Tree* replace_min1( Tree* t ) {
   return cata_tree(rep_alg,t)( cata_tree(min_alg,t) );
}

//////////////////////////////////////////////////////////////////////
// tupling solution (Listing 4)

struct TupleTree {
   template <class A, class B>
   struct Helper {
      TreeAlgebra<A> x;
      TreeAlgebra<B> y;
      Helper( const TreeAlgebra<A>& xx, const TreeAlgebra<B>& yy ) 
      : x(xx), y(yy) {}
      template <class L, class R>
      struct Sig : public FunType<L,R,
         std::pair<typename L::first_type,typename L::second_type> > {};
      template <class L, class R>
      typename Sig<L,R>::ResultType
      operator()( const L& l, const R& r ) const {
         return makePair(x.bin(fst(l),fst(r)),y.bin(snd(l),snd(r)));
      }
   };
   template <class TA, class TB>
   struct Sig : public FunType<TA,TB,
      TreeAlgebra<std::pair<typename TA::Type,typename TB::Type> > > {};

   template <class A, class B>
   TreeAlgebra<std::pair<A,B> >
   operator()( const TreeAlgebra<A>& x, const TreeAlgebra<B>& y ) const {
      return TreeAlgebra<std::pair<A,B> >(
         fcpp::compose2( makePair, x.leaf, y.leaf ),
         makeCurryable2( Helper<A,B>(x,y) ) );
   }
} tuple_tree_;
Curryable2<TupleTree> tuple_tree(tuple_tree_);

TreeAlgebra<std::pair<int,Fun1<int,Tree*> > > 
   min_tup_rep( tuple_tree(min_alg,rep_alg) );

Tree* replace_min2( Tree* t ) {
   std::pair<int,Fun1<int,Tree*> > res = cata_tree(min_tup_rep,t);
   return res.second( res.first );
}

//////////////////////////////////////////////////////////////////////
// merging tupled functions (Listing 5)

// In comments here is the typechecking-but-failing solution
/*
struct Yadda : public 
CFunType<Fun1<int, std::pair<int, Tree*> >,
         Fun1<int, std::pair<int, Tree*> >,int,std::pair<int,Tree*> > {
   std::pair<int,Tree*>
   operator()( Fun1<int, std::pair<int, Tree*> > lfun, 
               Fun1<int, std::pair<int, Tree*> > rfun, int m ) const {
      std::pair<int, Tree*> lres( lfun(m) );
      std::pair<int, Tree*> rres( rfun(m) );
      return makePair( fcpp::min(lres.first,rres.first), 
                       new Tree(lres.second,rres.second) );
   }
} yadda_;
Curryable3<Yadda> yadda(yadda_);

TreeAlgebra<Fun1<int,std::pair<int,Tree*> > > 
   merged_alg( flip(uncurry(compose(flip(makePair),New1<Tree>()))),
               yadda );

// FIX THIS: Circularity below, using uninitialized "res.first"
Tree* replace_min3( Tree* t ) {
   std::pair<int,Tree*> res = cata_tree(merged_alg,t)( res.first );
   return res.second;
}
*/

// The real solution requires "full laziness" and was quite the
// pain-in-the-neck to get working.  :)
template <class A>
struct XTreeAlgebra {
   typedef A Type;
   Fun1<Fun0<int>,A> leaf;
   Fun2<A,A,A> bin;
   XTreeAlgebra( Fun1<Fun0<int>,A> a, Fun2<A,A,A> b ) : leaf(a), bin(b) {}
};

struct XCataTree {
   template <class TA, class T> struct Sig 
   : public FunType<TA,T,typename TA::Type> {};

   template <class A>
   A operator()( const XTreeAlgebra<A>& alg, Tree* t ) const {
      if( t->leaf() )
         return alg.leaf( const_(t->data()) );
      else
         return alg.bin( XCataTree()(alg,t->left()),
                         XCataTree()(alg,t->right()) );
   }
} xcata_tree_;
Curryable2<XCataTree> xcata_tree(xcata_tree_);

template <class F>
class Wisher {
   F f;
public:
   Wisher( const F& ff ) : f(ff) {}
   template <class X, class Y>
   struct Sig : public FunType<X,Y,typename RT<F,typename RT<X>::ResultType,
      typename RT<Y>::ResultType>::ResultType> {};
   template <class X, class Y>
   typename Sig<X,Y>::ResultType
   operator()( const X& x, const Y& y ) const {
      return f( x(), y() );
   }
};
template <class F>
Wisher<F> wish( const F& f ) { return Wisher<F>(f); }

struct Yadda : public 
CFunType<Fun1<Fun0<int>, std::pair<Fun0<int>, Fun0<Tree*> > >,
         Fun1<Fun0<int>, std::pair<Fun0<int>, Fun0<Tree*> > >,
         Fun0<int>,std::pair<Fun0<int>,Fun0<Tree*> > > {
   std::pair<Fun0<int>,Fun0<Tree*> >
   operator()( Fun1<Fun0<int>,std::pair<Fun0<int>,Fun0<Tree*> > > lfun, 
               Fun1<Fun0<int>,std::pair<Fun0<int>,Fun0<Tree*> > > rfun, 
               Fun0<int> m ) const {
      std::pair<Fun0<int>, Fun0<Tree*> > lres( lfun(m) );
      std::pair<Fun0<int>, Fun0<Tree*> > rres( rfun(m) );
      return makePair( curry2(wish(fcpp::min),lres.first,rres.first),
                       curry2(wish(New2<Tree>()),lres.second,rres.second) );
   }
} yadda_;
Curryable3<Yadda> yadda(yadda_);

struct Blah 
: public CFunType<Fun0<int>,Fun0<int>,std::pair<Fun0<int>,Fun0<Tree*> > > {
   std::pair<Fun0<int>,Fun0<Tree*> >
   operator()( Fun0<int> i, Fun0<int> m ) const {
      return makePair( i, Compose0()(New1<Tree>(),m) );
   }
} blah_;
Curryable2<Blah> blah(blah_);

XTreeAlgebra<Fun1<Fun0<int>,std::pair<Fun0<int>,Fun0<Tree*> > > > 
   merged_alg( blah, yadda );

template <class F>
class Invoker
: public CFunType<typename RT<typename RT<F>::ResultType>::ResultType> {
   F thunk;
public:
   Invoker( const F& f ) : thunk(f) {}
   typename RT<typename RT<F>::ResultType>::ResultType operator()() const {
      return thunk()();
   }
};
template <class F>
Invoker<F> invoke( const F& f ) { return Invoker<F>(f); }
   
class TheResult : public CFunType<std::pair<Fun0<int>,Fun0<Tree*> > > {
   Tree* t;
public:
   TheResult( Tree* tt ) : t(tt) {}
   std::pair<Fun0<int>,Fun0<Tree*> > operator()() const {
      static std::pair<Fun0<int>,Fun0<Tree*> > res = 
         xcata_tree(merged_alg,t)( invoke( Compose0()(fst,*this) ) );
      return res;
   }
};

Tree* replace_min3( Tree* t ) {
   return TheResult(t)().second();
}

//////////////////////////////////////////////////////////////////////

int main() {
   Tree *t = new Tree( new Tree(3), new Tree( new Tree(4), new Tree(5) ) );
   t = new Tree( t, t );
   t = new Tree( t, t );
   t = new Tree( t, t );
   t = new Tree( t, t );
/* Move this window around to change the tree size
   t = new Tree( t, t );
   t = new Tree( t, t );
*/
   cout << "Sum nodes is " << sum_tree(t) << endl;
   cout << "Sum nodes is " << cata_tree(sum_alg,t) << endl;

   Tree *tmp;
   Timer timer;
   int start, end;

   // warm up the cache
   tmp = replace_min1(t);
   cout << "After repl_min1, sum is " << sum_tree(tmp) << endl;

   start = timer.ms_since_start();
   for( int i=0; i<N; ++i ) {
      the_count = 0;
      tmp = replace_min1(t);
      cout << "Count is now " << the_count << endl;
      cout << "After repl_min1, sum is " << sum_tree(tmp) << endl;
   }
   end = timer.ms_since_start();
   cerr << "took " << end-start << " ms" << endl;

   start = timer.ms_since_start();
   for( int i=0; i<N; ++i ) {
      the_count = 0;
      tmp = replace_min2(t);
      cout << "Count is now " << the_count << endl;
      cout << "After repl_min2, sum is " << sum_tree(tmp) << endl;
   }
   end = timer.ms_since_start();
   cerr << "took " << end-start << " ms" << endl;

   start = timer.ms_since_start();
   for( int i=0; i<N; ++i ) {
      the_count = 0;
      tmp = replace_min3(t);
      cout << "Count is now " << the_count << endl;
      cout << "After repl_min3, sum is " << sum_tree(tmp) << endl;
   }
   end = timer.ms_since_start();
   cerr << "took " << end-start << " ms" << endl;

   return 0;
}

