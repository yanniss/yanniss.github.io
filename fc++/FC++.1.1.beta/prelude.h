//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef PRELUDE_DOT_H
#define PRELUDE_DOT_H

//////////////////////////////////////////////////////////////////////
// Note that this header file includes all the other FC++ header files,
// so including this one (prelude.h) is sufficient to suck in the whole
// library.  Please excuse the namespace pollution (to be fixed in a
// future version).  :)
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Here we define a bunch of functions from the Haskell Standard
// Prelude.  For documentation of their behaviors, see 
//    http://haskell.org/onlinereport/standard-prelude.html
//////////////////////////////////////////////////////////////////////

#include "list.h"
#include "curry.h"
#include "operator.h"

//////////////////////////////////////////////////////////////////////
// Stuff from HSP
//////////////////////////////////////////////////////////////////////

struct Until {
   template <class Pred, class Unary, class T>
   struct Sig : public FunType<Pred,Unary,T,T> {};

   template <class Pred, class Unary, class T>
   T operator()( const Pred& p, const Unary& op, const T& start ) {
      if( p(start) )
         return start;
      else
         return until( p, op, op(start) );
   }
};
namespace { Until until; }

struct Last {
   template <class L>
   struct Sig : public FunType<L,typename L::WrappedType> {};

   template <class T>
   T operator()( const List<T>& l ) const {
      if( null( tail( l ) ) )
         return head(l);
      else
         return Last()(tail(l));
   }
};
namespace { Last last; }

struct Init {
   template <class L>
   struct Sig : public FunType<L,L> {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      if( null( tail( l ) ) )
         //return Nil<T>();
         return NIL;
      else
         return cons( head(l), curry( Init(), tail(l) ) );
   }
};
namespace { Init init; }

struct Length {
   template <class L>
   struct Sig : public FunType<L,size_t> {};

   template <class T>
   size_t operator()( const List<T>& l ) const {
      if( null(l) )
         return 0;
      else
         return 1 + Length()( tail(l) );
   }
};
namespace { Length length; }

// At is Haskell's operator (!!)
struct At {
   template <class L, class N>
   struct Sig : public FunType<L,N,typename L::WrappedType> {};

   template <class T>
   T operator()( const List<T>& l, size_t n ) const {
      if( n==0 )
         return head(l);
      else
         return At()( tail(l), n-1 );
   }
};
namespace { At at; }

struct Filter {
   template <class P, class LT>
   struct Sig : public FunType<P,LT,LT> {};

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
namespace { Filter filter; }

struct Concat {
   template <class LLT>
   struct Sig : public FunType<LLT, typename LLT::WrappedType> {};

   template <class T>
   List<T> operator()( const List<List<T> >& l ) const {
      if( null(l) )
         return List<T>();
      else
         return cat( Const()(head(l)), curry(Concat(),(tail(l))) );
   }
};
namespace { Concat concat; }

// Note, this isn't lazy, even if Op is
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
namespace { Foldr foldr; }

struct Foldr1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::WrappedType> {};

   template <class Op, class T>
   T operator()( const Op& op, const List<T>& l ) const {
      return foldr( op, head(l), tail(l) );
   }
};
namespace { Foldr1 foldr1; }

struct Foldl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,E> {};

   template <class Op, class E, class T>
   E operator()( const Op& op, const E& e, const List<T>& l ) const {
      if( null(l) )
         return e;
      else 
         return Foldl()( op, op(e,head(l)), tail(l) );
   }
};
namespace { Foldl foldl; }

struct Foldl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::WrappedType> {};

   template <class Op, class T>
   T operator()( const Op& op, const List<T>& l ) const {
      return foldl( op, head(l), tail(l) );
   }
};
namespace { Foldl1 foldl1; }

struct Scanr {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,List<E> > {};

   template <class Op, class E, class T>
   List<E> operator()( const Op& op, const E& e, const List<T>& l ) const {
      if( null(l) )
         return one_element( e );
      else {
         List<E> temp = Scanr()( op, e, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
   }
};
namespace { Scanr scanr; }

struct Scanr1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,L> {};

   template <class Op, class T>
   List<T> operator()( const Op& op, const List<T>& l ) const {
      if( null( tail(l) ) )
         return l;
      else {
         List<T> temp = Scanr1()( op, tail(l) );
         return cons( op( head(l), head(temp) ), temp );
      }
   }
};
namespace { Scanr1 scanr1; }

// FIX THIS: Laziness?
struct Scanl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,List<E> > {};

   template <class Op, class E, class T>
   List<E> operator()( const Op& op, const E& e, const List<T>& l ) const {
      if( null(l) )
         return cons( e, List<E>() );
      else
         return cons( e, Scanl()( op, op(e,head(l)), tail(l) ) );
   }
};
namespace { Scanl scanl; }

struct Scanl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,L> {};

   template <class Op, class T>
   List<T> operator()( const Op& op, const List<T>& l ) const {
      return scanl( op, head(l), tail(l) );
   }
};
namespace { Scanl1 scanl1; }

struct Iterate {
   template <class F, class T>
   struct Sig : public FunType<F,T,List<T> > {};

   template <class F, class T>
   List<T> operator()( const F& f, const T& x ) const {
      return cons( x, curry2( Iterate(), f, f(x) ) );
   }
};
namespace { Iterate iterate; }

struct Repeat {
   template <class T>
   struct Sig : public FunType<T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x ) const {
      return cons( x, curry( Repeat(), x ) );
   }
};
namespace { Repeat repeat; }

struct Map {
   template <class F, class L>
   struct Sig : public FunType<F,L,
      List<typename F::template Sig<typename L::WrappedType>::ResultType> > {};

   template <class F, class T>
   Sig<F, List<T> >::ResultType
   operator()( const F& f, const List<T>& l ) const {
      if( null(l) )
         return NIL;
      else
         return cons( f(head(l)), curry2( Map(), f, tail(l) ) );
   }
};
namespace { Map map; }

struct Take {
   template <class N,class L>
   struct Sig : public FunType<N,L,L> {};

   template <class T>
   List<T> operator()( size_t n, const List<T>& l ) const {
      if( n==0 || null(l) )
         //return Nil<T>();
         return NIL;
      else
         return cons( head(l), curry2( Take(), n-1, tail(l) ) );
   }
};
namespace { Take take; }

struct Drop {
   template <class L>
   struct Sig : public FunType<size_t,L,L> {};
   
   template <class T>
   List<T> operator()( size_t n, const List<T>& l ) const {
      if( n==0 || null(l) )
         return l;
      else
         return Drop()( n-1, tail(l) );
   }
};
namespace { Drop drop; }

struct TakeWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,L> {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) || !p( head(l) ) )
         //return Nil<T>();
         return NIL;
      else
         return cons( head(l), curry2( TakeWhile(), p, tail(l) ) );
   }
};
namespace { TakeWhile takeWhile; }
         
struct DropWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,L> {};

   template <class P, class T>
   List<T> operator()( const P& p, const List<T>& l ) const {
      if( null(l) || !p( head(l) ) )
         return l;
      else
         return DropWhile()( p, tail(l) );
   }
};
namespace { DropWhile dropWhile; }
         
struct Replicate {
   template <class N, class T>
   struct Sig : public FunType<N,T,List<T> > {};

   template <class T>
   List<T> operator()( size_t n, const T& x ) const {
      return take( n, repeat(x) );
   }
};
namespace { Replicate replicate; }

struct Cycle {
   template <class L>
   struct Sig : public FunType<L,L> {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return cat( Const()(l), curry( Cycle(), l ) );
   }
};
namespace { Cycle cycle; }

struct SplitAt {
   template <class N, class L>
   struct Sig : public FunType<N,L,pair<L,L> > {};

   template <class T>
   pair<List<T>,List<T> > operator()( size_t n, const List<T>& l ) const {
      if( n==0 || null(l) )
         return make_pair( List<T>(), l );
      else {
         pair<List<T>,List<T> > temp = SplitAt()( n-1, tail(l) );
         return make_pair( cons( head(l), temp.first ), temp.second );
      }
   }
};
namespace { SplitAt splitAt; }

struct Span {
   template <class P, class L>
   struct Sig : public FunType<P,L,pair<L,L> > {};

   template <class P, class T>
   pair<List<T>,List<T> > operator()( const P& p, const List<T>& l ) const {
      if( null(l) || !p(head(l)) )
         return make_pair( List<T>(), l );
      else {
         pair<List<T>,List<T> > temp = Span()( p, tail(l) );
         return make_pair( cons(head(l),temp.first), temp.second );
      }
   }
};
namespace { Span span; }

template <class F, class G>
class Compose1Helper {
   F f;
   G g;
public:
   Compose1Helper( const F& a, const G& b ) : f(a), g(b) {}

   template <class T>
   struct Sig : public FunType<
   typename G::Sig<T>::Arg1Type,
   typename F::Sig<typename G::Sig<T>::ResultType>::ResultType
   > {};

   template <class T>
   typename F::Sig<typename G::Sig<T>::ResultType>::ResultType
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
namespace { Compose1 compose; }   // compose1 exists in STL


template <class F, class G, class H>
class Compose2Helper {
  F f;
  G g;
  H h;
public:
   Compose2Helper( const F& a, const G& b, const H& c) : f(a), g(b), h(c) {}

   template <class T>
   struct Sig : public FunType<
   typename G::Sig<T>::Arg1Type,
   typename F::Sig<typename G::Sig<T>::ResultType, 
                   typename H::Sig<T>::ResultType>::ResultType
   > {};

   template <class T>
   typename F::Sig<typename G::Sig<T>::ResultType, 
                   typename H::Sig<T>::ResultType>::ResultType
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
// namespace { Compose2 compose2; } //exists in STL


struct Break {
   template <class P, class L>
   struct Sig : public FunType<P,L,pair<L,L> > {};

   template <class P, class T>
   pair<List<T>,List<T> > operator()( const P& p, const List<T>& l ) const {
      return span( Compose1()( LogicalNot(), p ), l );
   }
};
// namespace { Break break; }  -- can't; break is C++ keyword

template <class Binary>
class FlipHelper {
   Binary op;
public:
   FlipHelper( const Binary& b ) : op(b) {}
   
   template <class Y, class X>
   struct Sig : public FunType<Y,X,
   typename Binary::Sig<X,Y>::ResultType
   > {};

   template <class Y, class X>
   typename Binary::Sig<X,Y>::ResultType
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
namespace { Flip flip; }

struct Reverse {
   template <class L>
   struct Sig : public FunType<L,L> {};

   // use StrictCons instead of cons to avoid infinitely recursive template (?)
   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return foldl( flip(StrictCons()), List<T>(), l );
   }
};
//namespace { Reverse reverse; }   // reverse is a name in the STL

struct Or;
struct And;

struct All {
   template <class P, class L>
   struct Sig : public FunType<P,L,bool> {};

   template <class P, class T>
   bool operator()( const P& p, const List<T>& l ) const {
      return And()( map( p, l ) );
   }
};
namespace { All all; }

struct Any {
   template <class P, class L>
   struct Sig : public FunType<P,L,bool> {};

   template <class P, class T>
   bool operator()( const P& p, const List<T>& l ) const {
      return Or()( map( p, l ) );
   }
};
namespace { Any any; }

struct Elem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T>
   bool operator()( const T& x, const List<T>& l ) const {
      return any( bind2of2( Equal(), x ), l );
   }
};
namespace { Elem elem; }

struct NotElem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T>
   bool operator()( const T& x, const List<T>& l ) const {
      return all( bind2of2( NotEqual(), x ), l );
   }
};
namespace { NotElem notElem; }

struct Sum {
   template <class L>
   struct Sig : public FunType<L,typename L::WrappedType> {};

   template <class T>
   T operator()( const List<T>& l ) const {
      return foldl( Plus(), 0, l );
   }
};
namespace { Sum sum; }

struct Product {
   template <class L>
   struct Sig : public FunType<L,typename L::WrappedType> {};

   template <class T>
   T operator()( const List<T>& l ) const {
      return foldl( Multiplies(), 0, l );
   }
};
namespace { Product product; }

struct Minimum {
   template <class L>
   struct Sig : public FunType<L,typename L::WrappedType> {};

   template <class T>
   T operator()( const List<T>& l ) const {
      return foldl1( Min(), l );
   }
};
namespace { Minimum minimum; }
   
struct Maximum {
   template <class L>
   struct Sig : public FunType<L,typename L::WrappedType> {};

   template <class T>
   T operator()( const List<T>& l ) const {
      return foldl1( Max(), l );
   }
};
namespace { Maximum maximum; }
   
struct ZipWith {
   template <class Z, class LA, class LB>
   struct Sig : public FunType<Z,LA,LB,
   List<typename RT<typename Z::Sig<typename LA::WrappedType,
                                    typename LB::WrappedType> >::ResultType> 
   > {};

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
namespace { ZipWith zipWith; }

struct Zip {
   template <class LA, class LB>
   struct Sig 
   : public FunType<LA,LB,List<pair<typename LA::WrappedType,
                                     typename LB::WrappedType> > > {};

   template <class A, class B>
   List<pair<A,B> > operator()( const List<A>& a, const List<B>& b ) const {
      return zipWith( MakePair(), a, b );
   }
};
namespace { Zip zip; }

// FIX THIS: write unzip

struct Fst {
   template <class P>
   struct Sig : public FunType<P,typename P::first_type> {};

   template <class A, class B>
   A operator()( const pair<A,B>& p ) const {
      return p.first;
   }
};
namespace { Fst fst; }

struct Snd {
   template <class P>
   struct Sig : public FunType<P,typename P::second_type> {};

   template <class A, class B>
   B operator()( const pair<A,B>& p ) const {
      return p.second;
   }
};
namespace { Snd snd; }

struct GcdPrime {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      if( y==0 )
         return x;
      else
         return GcdPrime()( y, x%y );
   }
};

struct Gcd {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      if( x==0 && y==0 )
         throw "Gcd error: x and y both 0";
      return GcdPrime()( x<0?-x:x, y<0?-y:y );
   }
};
namespace { Gcd gcd; }

struct Odd {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==1;
   }
};
namespace { Odd odd; }

struct Even {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==0;
   }
};
namespace { Even even; }

//////////////////////////////////////////////////////////////////////
// Not HSP but close
//////////////////////////////////////////////////////////////////////

// These next two are defined as _lazy_ versions of these operators on lists
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


// FIX THIS: almost HSP, maybe templatize (rather than int) and/or use succ?
struct EnumFrom : public CFunType<int,List<int> > {
   List<int> operator()( int x ) const {
      return list_until( Never1(), Inc(), x );
   }
};
namespace { EnumFrom enumFrom; }

// FIX THIS: almost HSP, maybe templatize (rather than int) and/or use succ?
struct EnumFromTo : public CFunType<int,int,List<int> > {
   List<int> operator()( int x, int y ) const {
      return list_until( bind2of2(Greater(),y), Inc(), x );
   }
};
namespace { EnumFromTo enumFromTo; }

#endif
