//
// Copyright (c) 2000,2001 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_PRELUDE_DOT_H
#define FCPP_PRELUDE_DOT_H

//////////////////////////////////////////////////////////////////////
// Note that this header file includes all the other FC++ header files,
// so including this one (prelude.h) is sufficient to suck in the whole
// library. 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Here we define a bunch of functions from the Haskell Standard
// Prelude (HSP).  For documentation of their behaviors, see 
//    http://haskell.org/onlinereport/standard-prelude.html
//
// A few of the functions are not from HSP, but seemed natural/useful.
//
// The implementations #ifdef-ed out (FCPP_SIMPLE_PRELUDE) are mostly
// just here to look at, as they are often more readable than their
// optimized counterparts.
//////////////////////////////////////////////////////////////////////

#include "list.h"

namespace fcpp {

template <class F, class G>
class Compose0Helper {
   F f;
   G g;
public:
   Compose0Helper( const F& a, const G& b ) : f(a), g(b) {}

   struct Sig : public FunType<
      typename F::template Sig<typename G::Sig::ResultType>::ResultType
   > {};

   typename F::template Sig<typename G::Sig::ResultType>::ResultType
   operator()() const {
      return f( g() );
   }
};
   
struct Compose0 {
   template <class F, class G>
   struct Sig : public FunType<F,G,Compose0Helper<F,G> > {};

   template <class F, class G>
   Compose0Helper<F,G> operator()( const F& f, const G& g ) const {
      return Compose0Helper<F,G>( f, g );
   }
};

template <class F, class G>
class Compose1Helper {
   F f;
   G g;
public:
   Compose1Helper( const F& a, const G& b ) : f(a), g(b) {}

   template <class T>
   struct Sig : public FunType<
   typename G::template Sig<T>::Arg1Type,
   typename F::template Sig<typename G::template Sig<T>::ResultType>::ResultType
   > {};

   template <class T>
   typename F::template Sig<typename G::template Sig<T>::ResultType>::ResultType
   operator()( const T& x ) const {
      return f( g(x) );
   }
};
   
// Compose1 is Haskell's operator (.) for f.g where g is a one-arg
struct Compose1 {
   template <class F, class G>
   struct Sig : public FunType<F,G,Compose1Helper<F,G> > {};

   template <class F, class G>
   Compose1Helper<F,G> operator()( const F& f, const G& g ) const {
      return Compose1Helper<F,G>( f, g );
   }
};
namespace {
Compose1 compose1, compose;
}

template <class F, class G, class H>
class Compose2Helper {
  F f;
  G g;
  H h;
public:
   Compose2Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   template <class T>
   struct Sig : public FunType<
   typename G::template Sig<T>::Arg1Type,
   typename F::template Sig<typename G::template Sig<T>::ResultType, 
                            typename H::template Sig<T>::ResultType>::ResultType
   > {};

   template <class T>
   typename F::template Sig<typename G::template Sig<T>::ResultType, 
                            typename H::template Sig<T>::ResultType>::ResultType
   operator()( const T& x ) const {
      return f( g(x), h(x) );
   }
};
   
// Compose2 composes a two argument function with two one-argument
// functions (taking the same type). This is quite useful for the
// common case of binary operators
struct Compose2 {
   template <class F, class G, class H>
   struct Sig : public FunType<F,G,H,Compose2Helper<F,G,H> > {};

   template <class F, class G, class H>
   Compose2Helper<F,G,H> operator()(const F& f, const G& g, const H& h) const {
      return Compose2Helper<F,G,H>( f, g, h );
   }
};
namespace {
Compose2 _compose2;
Curryable3<Compose2> compose2(_compose2);
}

struct Until {
   template <class Pred, class Unary, class T>
   struct Sig : public FunType<Pred,Unary,T,T> {};

   template <class Pred, class Unary, class T>
   T operator()( const Pred& p, const Unary& op, T start ) const {
      while( !p(start) ) {
         T tmp( start );
         start.~T();
         new (&start) T( op(tmp) );
      }
      return start;
   }
};
namespace {
Until _until;
Curryable3<Until> until(_until);
}

struct Last {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class T>
   T operator()( List<T> l ) const {
      while( !null( tail(l) ) )
         l = tail(l);
      return head(l);
   }
};
namespace {
Last last;
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Init {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      if( null( tail( l ) ) )
         return NIL;
      else
         return cons( head(l), curry( Init(), tail(l) ) );
   }
};
#else
struct Init {
   template <class L>
   struct Sig : public FunType<L,OddList<typename L::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType> 
   operator()( const L& l, 
               Reuser1<Inv,Var,Init,List<typename L::ElementType> >
               r = NIL ) const {
      if( null( tail( l ) ) )
         return NIL;
      else
         return cons( head(l), r( Init(), tail(l) ) );
   }
};
#endif
namespace {
Init init;
}

struct Length {
   template <class L>
   struct Sig : public FunType<L,size_t> {};

   template <class T>
   size_t operator()( List<T> l ) const {
      size_t x = 0;
      while( !null(l) ) {
         l = tail(l);
         ++x;
      }
      return x;
   }
};
namespace {
Length length;
}

// At is Haskell's operator (!!)
struct At {
   template <class L, class N>
   struct Sig : public FunType<L,N,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( L l, size_t n ) const {
      while( n!=0 ) {
         l = tail(l);
         --n;
      }
      return head(l);
   }
};
namespace {
At _at;
Curryable2<At> at(_at);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Filter {
   template <class P, class LT>
   struct Sig : public FunType<P,LT,List<typename LT::ElementType> > {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) )
         return l;
      else if( p(head(l)) )
        return cons( head(l), curry2( Filter(), p, tail(l) ) );
      else
         return Filter()( p, tail(l) );
   }
};
#else
template <class P, class T>
struct FilterHelp : public Fun0Impl< OddList<T> > {
   P p;
   mutable List<T> l;
   FilterHelp( const P& pp, const List<T>& ll ) : p(pp), l(ll) {}
   OddList<T> operator()() const {
      while(1) {
         if( null(l) )
            return NIL;
         else if( p( head(l) ) ) {
            T x = head(l);
            l = tail(l);
            return cons( x, Fun0< OddList<T> >(1,this) );
         }
         else
            l = tail(l);
      }
   }
};
struct Filter {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class L>
   List<typename L::ElementType>
   operator()( const P& p, L l ) const {
      return Fun0< OddList<typename L::ElementType> >(1, 
               new FilterHelp<P,typename L::ElementType>(p,l) );
   }
};
/* For filter, the version with a Reuser is just not as good as the
hand-coded reuse version, which is why this is commented out.
struct Filter {
   template <class P, class L>
   struct Sig : public FunType<P,L,OddList<typename L::ElementType> > {};

   template <class P, class T>
   OddList<T> operator()( const P& p, List<T> l,
                            Reuser2<Inv,Inv,Var,Filter,P,List<T> > 
                            r = NIL ) const {
      while(1) {
         if( null(l) )
            return NIL;
         else if( p(head(l)) )
            return cons( head(l), r( Filter(), p, tail(l) ) );
         else
            r.iter( Filter(), p, l = tail(l) );
      }
   }
};
*/
#endif
namespace {
Filter _filter;
Curryable2<Filter> filter(_filter);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Concat {
   template <class LLT>
   struct Sig : public FunType<LLT, typename LLT::ElementType> {};

   template <class T>
   List<T> operator()( const List<List<T> >& l ) const {
      if( null(l) )
         return List<T>();
      else
         return cat( head(l), curry(Concat(),tail(l)) );
   }
};
#else
struct Concat {
   template <class LLT>
   struct Sig : public FunType<LLT, 
      OddList<typename LLT::ElementType::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType::ElementType> 
   operator()( const L& l, 
               Reuser1<Inv,Var,Concat,List<typename L::ElementType> > 
               r = NIL ) const {
      if( null(l) )
         return NIL;
      else
         return cat( head(l), r(Concat(),tail(l)) );
   }
};
#endif
namespace {
Concat concat;
}

// Note: this isn't lazy (even if 'op' is 'cons').
struct Foldr {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,E> {};

   template <class Op, class E, class T>
   E operator()( const Op& op, const E& e, const List<T>& l ) const {
      if( null(l) )
         return e;
      else 
         return op( head(l), Foldr()( op, e, tail(l) ) );
   }
};
namespace {
Foldr _foldr;
Curryable3<Foldr> foldr(_foldr);
}

struct Foldr1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::ElementType> {};

   template <class Op, class T>
   T operator()( const Op& op, const List<T>& l ) const {
      return foldr( op, head(l), tail(l) );
   }
};
namespace {
Foldr1 _foldr1;
Curryable2<Foldr1> foldr1(_foldr1);
}

struct Foldl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,E> {};

   template <class Op, class E, class T>
   E operator()( const Op& op, E e, List<T> l ) const {
      while( !null(l) ) {
         E tmp( e );
         e.~E();
         new (&e) E( op(tmp,head(l)) );
         l = tail(l);
      }
      return e;
   }
};
namespace {
Foldl _foldl;
Curryable3<Foldl> foldl(_foldl);
}

struct Foldl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::ElementType> {};

   template <class Op, class T>
   T operator()( const Op& op, const List<T>& l ) const {
      return foldl( op, head(l), tail(l) );
   }
};
namespace {
Foldl1 _foldl1;
Curryable2<Foldl1> foldl1(_foldl1);
}

struct Scanr {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,OddList<E> > {};

   template <class Op, class E, class L>
   OddList<E> operator()( const Op& op, const E& e, const L& l ) const {
      if( null(l) )
         return cons( e, NIL );
      else {
         OddList<E> temp = Scanr()( op, e, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
   }
};
namespace {
Scanr _scanr;
Curryable3<Scanr> scanr(_scanr);
}

struct Scanr1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,OddList<typename L::ElementType> > {};

   template <class Op, class L>
   OddList<typename L::ElementType> 
   operator()( const Op& op, const L& l ) const {
      if( null( tail(l) ) )
         return l;
      else {
         OddList<typename L::ElementType> temp = Scanr1()( op, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
   }
};
namespace {
Scanr1 _scanr1;
Curryable2<Scanr1> scanr1(_scanr1);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Scanl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,List<E> > {};

   template <class Op, class E, class T>
   List<E> operator()( const Op& op, const E& e, const List<T>& l ) const {
      if( null(l) )
         return cons( e, NIL );
      else
         return cons( e, curry3( Scanl(), op, op(e,head(l)), tail(l) ));
   }
};
#else
struct Scanl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,OddList<E> > {};

   template <class Op, class E, class L>
   OddList<E> operator()( const Op& op, const E& e, const L& l,
         Reuser3<Inv,Inv,Var,Var,Scanl,Op,E,List<typename L::ElementType> >
         r = NIL ) const {
      if( null(l) )
         return cons( e, NIL );
      else
         return cons( e, r( Scanl(), op, op(e,head(l)), tail(l) ) );
   }
};
#endif
namespace {
Scanl _scanl;
Curryable3<Scanl> scanl(_scanl);
}

struct Scanl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,OddList<typename L::ElementType> > {};

   template <class Op, class L>
   OddList<typename L::ElementType> 
   operator()( const Op& op, const L& l ) const {
      return scanl( op, head(l), tail(l) );
   }
};
namespace {
Scanl1 _scanl1;
Curryable2<Scanl1> scanl1(_scanl1);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Iterate {
   template <class F, class T>
   struct Sig : public FunType<F,T,List<T> > {};

   template <class F, class T>
   List<T> operator()( const F& f, const T& x ) const {
      return cons( x, curry2( Iterate(), f, f(x) ) );
   }
};
#else
struct Iterate {
   template <class F, class T>
   struct Sig : public FunType<F,T,OddList<T> > {};

   template <class F, class T>
   OddList<T> operator()( const F& f, const T& x,
                            Reuser2<Inv,Inv,Var,Iterate,F,T> r = NIL ) const {
      return cons( x, r( Iterate(), f, f(x) ) );
   }
};
#endif
namespace {
Iterate _iterate;
Curryable2<Iterate> iterate(_iterate);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Repeat {
   template <class T>
   struct Sig : public FunType<T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x ) const {
      return cons( x, curry( Repeat(), x ) );
   }
};
#else
struct Repeat {
   template <class T>
   struct Sig : public FunType<T,OddList<T> > {};

   template <class T>
   OddList<T> operator()( const T& x, 
                            Reuser1<Inv,Inv,Repeat,T> r = NIL ) const {
      return cons( x, r( Repeat(), x ) );
   }
};
#endif
namespace {
Repeat repeat;
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Map {
   template <class F, class L>
   struct Sig : public FunType<F,L,
      List<typename F::template Sig<typename L::ElementType>::ResultType> > {};

   template <class F, class T>
   List<typename F::template Sig<T>::ResultType>
   operator()( const F& f, const List<T>& l ) const {
      if( null(l) )
         return NIL;
      else
         return cons( f(head(l)), curry2( Map(), f, tail(l) ) );
   }
};
#else
struct Map {
   template <class F, class L>
   struct Sig : public FunType<F,L,
      OddList<typename RT<F,typename L::ElementType>::ResultType> > {};

   template <class F, class L>
   OddList<typename RT<F,typename L::ElementType>::ResultType> 
   operator()( const F& f, const L& l, 
               Reuser2<Inv,Inv,Var,Map,F,List<typename L::ElementType> >
               r = NIL ) const {
      if( null(l) )
         return NIL;
      else
         return cons( f(head(l)), r( Map(), f, tail(l) ) );
   }
};
#endif
namespace {
Map _map;
Curryable2<Map> map(_map);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Take {
   template <class N,class L>
   struct Sig : public FunType<N,L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( size_t n, const List<T>& l ) const {
      if( n==0 || null(l) )
         return NIL;
      else
         return cons( head(l), curry2( Take(), n-1, tail(l) ) );
   }
};
#else
struct Take {
   template <class N,class L>
   struct Sig : public FunType<N,L,OddList<typename L::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType> 
   operator()( size_t n, const L& l,
               Reuser2<Inv,Var,Var,Take,size_t,List<typename L::ElementType> > 
               r = NIL ) const {
      if( n==0 || null(l) )
         return NIL;
      else
         return cons( head(l), r( Take(), n-1, tail(l) ) );
   }
};
#endif
namespace {
Take _take;
Curryable2<Take> take(_take);
}

struct Drop {
   template <class Dummy, class L>
   struct Sig : public FunType<size_t,L,List<typename L::ElementType> > {};
   
   template <class T>
   List<T> operator()( size_t n, List<T> l ) const {
      while( n!=0 && !null(l) ) {
         --n;
         l = tail(l);
      }
      return l;
   }
};
namespace {
Drop _drop;
Curryable2<Drop> drop(_drop);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct TakeWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) || !p( head(l) ) )
         return NIL;
      else
         return cons( head(l), curry2( TakeWhile(), p, tail(l) ) );
   }
};
#else
struct TakeWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,OddList<typename L::ElementType> > {};

   template <class P, class L>
   OddList<typename L::ElementType>
   operator()( const P& p, const L& l,
               Reuser2<Inv,Inv,Var,TakeWhile,P,List<typename L::ElementType> >
               r = NIL ) const {
      if( null(l) || !p( head(l) ) )
         return NIL;
      else
         return cons( head(l), r( TakeWhile(), p, tail(l) ) );
   }
};
#endif
namespace {
TakeWhile _takeWhile; 
Curryable2<TakeWhile> takeWhile(_takeWhile);
}

struct DropWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class T>
   List<T> operator()( const P& p, List<T> l ) const {
      while( !null(l) && p( head(l) ) )
         l = tail(l);
      return l;
   }
};
namespace {
DropWhile _dropWhile;
Curryable2<DropWhile> dropWhile(_dropWhile);
}

struct Replicate {
   template <class N, class T>
   struct Sig : public FunType<N,T,OddList<T> > {};

   template <class T>
   OddList<T> operator()( size_t n, const T& x ) const {
      return take( n, repeat(x) );
   }
};
namespace {
Replicate _replicate; 
Curryable2<Replicate> replicate(_replicate);
}

#ifdef FCPP_SIMPLE_PRELUDE
struct Cycle {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return cat( l, curry( Cycle(), l ) );
   }
};
#else
struct Cycle {
   template <class L>
   struct Sig : public FunType<L,OddList<typename L::ElementType> > {};

   template <class L>
   OddList<typename L::ElementType> 
   operator()( const L& l, Reuser1<Inv,Inv,Cycle,L> r = NIL ) const {
      return cat( l, r( Cycle(), l ) );
   }
};
#endif
namespace {
Cycle cycle; 
}

struct SplitAt {
   template <class N, class L>
   struct Sig : public FunType<N,L,std::pair<List<typename
      L::ElementType>,List<typename L::ElementType> > > {};

   template <class T>
   std::pair<List<T>,List<T> > operator()( size_t n, const List<T>& l ) const {
      if( n==0 || null(l) )
         return std::make_pair( List<T>(), l );
      else {
         std::pair<List<T>,List<T> > temp = SplitAt()( n-1, tail(l) );
         List<T> tl = cons( head(l), temp.first );
         return std::make_pair( tl, temp.second );
      }
   }
};
namespace {
SplitAt _splitAt; 
Curryable2<SplitAt> splitAt(_splitAt);
}

struct Span {
   template <class P, class L>
   struct Sig : public FunType<P,L,std::pair<List<typename
      L::ElementType>,List<typename L::ElementType> > > {};

   template <class P, class T>
   std::pair<List<T>,List<T> > 
   operator()( const P& p, const List<T>& l ) const {
      if( null(l) || !p(head(l)) )
         return std::make_pair( List<T>(), l );
      else {
         std::pair<List<T>,List<T> > temp = Span()( p, tail(l) );
         List<T> tl = cons(head(l),temp.first);
         return std::make_pair( tl, temp.second );
      }
   }
};
namespace {
Span _span; 
Curryable2<Span> span(_span);
}

struct Break {
   template <class P, class L>
   struct Sig : public FunType<P,L,std::pair<List<typename
      L::ElementType>,List<typename L::ElementType> > > {};

   template <class P, class T>
   std::pair<List<T>,List<T> > 
   operator()( const P& p, const List<T>& l ) const {
      return span( Compose1()( LogicalNot(), p ), l );
   }
};
namespace {
Break _break_;
Curryable2<Break> break_(_break_);   // C++ keyword, so add trailing underscore
}

template <class Binary>
class FlipHelper {
   Binary op;
public:
   FlipHelper( const Binary& b ) : op(b) {}
   
   template <class Y, class X>
   struct Sig : public FunType<Y,X, 
      typename Binary::template Sig<X,Y>::ResultType > {};

   template <class Y, class X>
   typename Binary::template Sig<X,Y>::ResultType
   operator()( const Y& y, const X& x ) const {
      return op( x, y );
   }
};
struct Flip {
   template <class Binary>
   struct Sig : public FunType<Binary,FlipHelper<Binary> > {};

   template <class Binary>
   FlipHelper<Binary> operator()( const Binary& op ) const {
      return FlipHelper<Binary>( op );
   }
};
namespace {
Flip flip;
}

struct Reverse {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return curry3( foldl, flip(cons), List<T>(), l );
   }
};
namespace {
Reverse reverse;
}

//////////////////////////////////////////////////////////////////////
// Not HSP but close
//////////////////////////////////////////////////////////////////////

// These next two are defined as _lazy_ versions of these operators on lists
// We don't give them names because they conflict with C++ operators
struct And : public CFunType<List<bool>,bool> {
   bool operator()( const List<bool>& l ) const {
      return null(dropWhile( curry2(Equal(),true), l ));
   }
};

struct Or : public CFunType<List<bool>,bool> {
   bool operator()( const List<bool>& l ) const {
      return !null(dropWhile( curry2(Equal(),false), l ));
   }
};

//////////////////////////////////////////////////////////////////////
// Back to HSP
//////////////////////////////////////////////////////////////////////

struct All {
   template <class P, class L>
   struct Sig : public FunType<P,L,bool> {};

   template <class P, class L>
   bool operator()( const P& p, const L& l ) const {
      return And()( map( p, l ) );
   }
};
namespace {
All _all;
Curryable2<All> all(_all);
}

struct Any {
   template <class P, class L>
   struct Sig : public FunType<P,L,bool> {};

   template <class P, class L>
   bool operator()( const P& p, const L& l ) const {
      return Or()( map( p, l ) );
   }
};
namespace {
Any _any; 
Curryable2<Any> any(_any);
}

struct Elem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T, class L>
   bool operator()( const T& x, const L& l ) const {
      return any( bind2of2( Equal(), x ), l );
   }
};
namespace {
Elem _elem; 
Curryable2<Elem> elem(_elem);
}

struct NotElem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T, class L>
   bool operator()( const T& x, const L& l ) const {
      return all( bind2of2( NotEqual(), x ), l );
   }
};
namespace {
NotElem _notElem; 
Curryable2<NotElem> notElem(_notElem);
}

struct Sum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl( Plus(), 0, l );
   }
};
namespace {
Sum sum;
}

struct Product {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl( Multiplies(), 1, l );
   }
};
namespace {
Product product;
}

struct Minimum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl1( Min(), l );
   }
};
namespace {
Minimum minimum; 
}

struct Maximum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl1( Max(), l );
   }
};
namespace {
Maximum maximum; 
}

#ifdef FCPP_SIMPLE_PRELUDE
struct ZipWith {
   template <class Z, class LA, class LB>
   struct Sig : public FunType<Z,LA,LB,
   List<typename RT<typename Z::template Sig<typename LA::ElementType,
                                 typename LB::ElementType> >::ResultType> > {};

   template <class Z, class A, class B>
   List<typename RT<Z,A,B>::ResultType> 
   operator()( const Z& z, const List<A>& a, const List<B>& b) const {
      if( null(a) || null(b) )
         return List<typename RT<Z,A,B>::ResultType>();
      else
         return cons( z(head(a),head(b)),
            curry3( ZipWith(), z, tail(a), tail(b) ) );
   }
};
#else
struct ZipWith {
   template <class Z, class LA, class LB>
   struct Sig : public FunType<Z,LA,LB,
   OddList<typename RT<typename Z::template Sig<typename LA::ElementType,
                                 typename LB::ElementType> >::ResultType> > {};

   template <class Z, class LA, class LB>
   OddList<typename RT<typename Z::template Sig<typename LA::ElementType,
                                     typename LB::ElementType> >::ResultType> 
   operator()( const Z& z, const LA& a, const LB& b,
               Reuser3<Inv,Inv,Var,Var,ZipWith,Z,
                  List<typename LA::ElementType>,
                  List<typename LB::ElementType> > r = NIL ) const {
      if( null(a) || null(b) )
         return NIL;
      else
         return cons( z(head(a),head(b)),
            r( ZipWith(), z, tail(a), tail(b) ) );
   }
};
#endif
namespace {
ZipWith _zipWith; 
Curryable3<ZipWith> zipWith(_zipWith);
}

struct Zip {
   template <class LA, class LB>
   struct Sig 
   : public FunType<LA,LB,OddList<std::pair<typename LA::ElementType,
                                              typename LB::ElementType> > > {};

   template <class LA, class LB>
   OddList<std::pair<typename LA::ElementType, typename LB::ElementType> >
   operator()( const LA& a, const LB& b ) const {
      return zipWith( MakePair(), a, b );
   }
};
namespace {
Zip _zip;
Curryable2<Zip> zip(_zip);
}

struct Fst {
   template <class P>
   struct Sig : public FunType<P,typename P::first_type> {};

   template <class A, class B>
   A operator()( const std::pair<A,B>& p ) const {
      return p.first;
   }
};
namespace {
Fst fst;
}

struct Snd {
   template <class P>
   struct Sig : public FunType<P,typename P::second_type> {};

   template <class A, class B>
   B operator()( const std::pair<A,B>& p ) const {
      return p.second;
   }
};
namespace {
Snd snd;
}

struct Unzip {
   template <class LPT>
   struct Sig : public FunType<LPT,std::pair<
      List<typename LPT::ElementType::first_type>,
      List<typename LPT::ElementType::second_type> > > {};

   template <class LPT>
   std::pair<
      List<typename LPT::ElementType::first_type>,
      List<typename LPT::ElementType::second_type> >
   operator()( const LPT& l ) const {
      typedef typename LPT::ElementType::first_type F;
      typedef typename LPT::ElementType::second_type S;
      return std::make_pair( List<F>(curry2(map,fst,l)), 
                             List<S>(curry2(map,snd,l))  );
   }
};
namespace {
Unzip unzip;
}

struct GcdPrime {
   template <class T, class U> struct Sig;
   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( T x, T y ) const {
      while( y!=0 ) {
         T tmp( x%y );
         x = y;
         y = tmp;
      }
      return x;
   }
};
struct Gcd {
   template <class T, class U> struct Sig;
   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      if( x==0 && y==0 )
         throw fcpp_exception("Gcd error: x and y both 0");
      return GcdPrime()( x<0?-x:x, y<0?-y:y );
   }
};
namespace {
Gcd _gcd; 
Curryable2<Gcd> gcd(_gcd);
}

struct Odd {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==1;
   }
};
namespace {
Odd odd; 
}

struct Even {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==0;
   }
};
namespace {
Even even; 
}

//////////////////////////////////////////////////////////////////////
// Not HSP but close
//////////////////////////////////////////////////////////////////////

// For some unknown reason, g++2.95.2 (for Solaris, at least) generates
// poor code when these next two functoids are templates.  (g++3 does
// fine, regardless.)  As a result, we make them just work with ints,
// unless the user #defines the flag below.
#ifdef FCPP_TEMPLATE_ENUM
template <class T>
struct EFH : public Fun0Impl< OddList<T> > {
   mutable T x;
   EFH( const T& xx ) : x(xx) {}
   OddList<T> operator()() const {
      ++x;
      return cons( x-1, Fun0<OddList<T> >(1,this) );
   }
};
struct EnumFrom {
   template <class T>
   struct Sig : FunType<T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x ) const {
      return Fun0<OddList<T> >(1, new EFH<T>(x) );
   }
};
#else
struct EFH : public Fun0Impl< OddList<int> > {
   mutable int x;
   EFH( int xx ) : x(xx) {}
   OddList<int> operator()() const {
      ++x;
      return cons( x-1, Fun0<OddList<int> >(1,this) );
   }
};
struct EnumFrom : CFunType<int,List<int> > {
   List<int> operator()( int x ) const {
      return Fun0<OddList<int> >(1, new EFH(x) );
   }
};
#endif
namespace {
EnumFrom enumFrom; 
}

#ifdef FCPP_TEMPLATE_ENUM
template <class T>
struct EFTH : public Fun0Impl<OddList<T> > {
   mutable T x;
   T y;
   EFTH( const T& xx, const T& yy ) : x(xx), y(yy) {}
   OddList<T> operator()() const {
      if( x > y )
         return NIL;
      ++x;
      return cons( x-1, Fun0<OddList<int> >( 1, this ) );
   }
};
struct EnumFromTo {
   template <class T, class U> struct Sig;
   template <class T>
   struct Sig<T,T> : FunType<T,T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x, const T& y ) const {
      return Fun0<OddList<T> >( 1, new EFTH<T>(x,y) );
   }
};
#else
struct EFTH : public Fun0Impl<OddList<int> > {
   mutable int x;
   int y;
   EFTH( const int& xx, const int& yy ) : x(xx), y(yy) {}
   OddList<int> operator()() const {
      if( x > y )
         return NIL;
      ++x;
      return cons( x-1, Fun0<OddList<int> >( 1, this ) );
   }
};
struct EnumFromTo : CFunType<int,int,List<int> > {
   List<int> operator()( const int& x, const int& y ) const {
      return Fun0<OddList<int> >( 1, new EFTH(x,y) );
   }
};
#endif
namespace {
EnumFromTo _enumFromTo; 
Curryable2<EnumFromTo> enumFromTo(_enumFromTo);
}

// Not HSP
struct ListUntil {
   template <class P, class F, class T>
   struct Sig : public FunType<P,F,T,List<T> > {};

   template <class Pred, class Unary, class T>
   List<T> operator()( const Pred& p, const Unary& f, const T& x ) const {
      return takeWhile( fcpp::compose(logicalNot,p), iterate(f,x) );
   }
};
namespace {
ListUntil _listUntil;
Curryable3<ListUntil> listUntil(_listUntil);
}

} // end namespace fcpp

#endif
