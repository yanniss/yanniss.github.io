//
// Copyright (c) 2000,2001,2002 Brian McNamara and Yannis Smaragdakis
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
// A number of the functions are not from HSP, but seemed natural/useful.
//
// The implementations #ifdef-ed out (FCPP_SIMPLE_PRELUDE) are mostly
// just here to look at, as they are often more readable than their
// optimized counterparts.
//////////////////////////////////////////////////////////////////////

#include "list.h"

namespace fcpp {

struct Id {
   template <class T> struct Sig : public FunType<T,T> {};

   template <class T>
   T operator()( const T& x ) const {
      return x;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Id id;
FCPP_MAYBE_NAMESPACE_CLOSE

// Plain "compose" makes Compose0 no longer necessary, but g++2.95.2 has
// bugs that prevent compose() from working on 0-arg functoids, so we
// keep this around for now.  Consider Compose0 deprecated.
template <class F, class G>
class Compose0Helper : public CFunType<
typename F::template Sig<typename RT<G>::ResultType>::ResultType> {
   F f;
   G g;
public:
   Compose0Helper( const F& a, const G& b ) : f(a), g(b) {}

   typename F::template Sig<typename RT<G>::ResultType>::ResultType
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
class ComposerBase {
protected:
   F f;
   G g;
   ComposerBase( const F& ff, const G& gg ) : f(ff), g(gg) {}
};
template <class F, class G, bool>
class ComposerMixin : public CallableWithoutArguments, public
ComposerBase<F,G> {
protected:
   using ComposerBase<F,G>::f;
   using ComposerBase<F,G>::g;
   ComposerMixin( const F& ff, const G& gg ) : ComposerBase<F,G>(ff,gg) {}
public:
   typename RT<F,typename RT<G>::ResultType>::ResultType
   operator()() const { return f( g() ); }
};
template <class F, class G>
class ComposerMixin<F,G,false> : public ComposerBase<F,G> {
protected:
   using ComposerBase<F,G>::f;
   using ComposerBase<F,G>::g;
   ComposerMixin( const F& ff, const G& gg ) : ComposerBase<F,G>(ff,gg) {}
   template <class T> struct Impossible {};
public:
   template <class T> void operator()( Impossible<T>& ) {}
};
template <class F, class G>
class Composer 
: public ComposerMixin<F,G,Inherits<G,CallableWithoutArguments>::value> {
   typedef ComposerMixin<F,G,Inherits<G,CallableWithoutArguments>::value>
      Super;
   using Super::f;
   using Super::g;
public:
#ifndef FCPP_NO_USING_DECLS
   using Super::operator();
#endif
   Composer( const F& ff, const G& gg ) : Super(ff,gg) {}

   template <class X=Void, class Y=Void, class Z=Void, class Dummy=Void>
   struct Sig : FunType<typename RT<G,X,Y,Z>::Arg1Type,
                        typename RT<G,X,Y,Z>::Arg2Type,
                        typename RT<G,X,Y,Z>::Arg3Type,
          typename RT<F,typename RT<G,X,Y,Z>::ResultType>::ResultType> {};
   template <class X, class Y, class Dummy>
   struct Sig<X,Y,Void,Dummy> : FunType<typename RT<G,X,Y>::Arg1Type,
                                        typename RT<G,X,Y>::Arg2Type,
          typename RT<F,typename RT<G,X,Y>::ResultType>::ResultType> {};
   template <class X, class Dummy>
   struct Sig<X,Void,Void,Dummy> : FunType<typename RT<G,X>::Arg1Type,
          typename RT<F,typename RT<G,X>::ResultType>::ResultType> {};
   template <class Dummy>
   struct Sig<Void,Void,Void,Dummy> : FunType<
          typename RT<F,typename RT<G>::ResultType>::ResultType> {};

   template <class X>
   typename Sig<X>::ResultType 
   operator()( const X& x ) const { return f( g(x) ); }

   template <class X, class Y>
   typename Sig<X,Y>::ResultType 
   operator()( const X& x, const Y& y ) const { return f( g(x,y) ); }

   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const 
   { return f( g(x,y,z) ); }
};

// Compose is Haskell's operator (.) for f.g where g is a one-arg
struct Compose {
   template <class F, class G>
   struct Sig : public FunType<F,G,Composer<F,G> > {};

   template <class F, class G>
   Composer<F,G> operator()( const F& f, const G& g ) const {
      return Composer<F,G>( f, g );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Compose compose_;
FCPP_MAYBE_EXTERN Curryable2<Compose> compose FCPP_MAYBE_INIT((compose_));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Compose2 _compose2;
FCPP_MAYBE_EXTERN Curryable3<Compose2> compose2 FCPP_MAYBE_INIT((_compose2));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Until _until;
FCPP_MAYBE_EXTERN Curryable3<Until> until FCPP_MAYBE_INIT((_until));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Last {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( !null( tail(l) ) )
         l = tail(l);
      return head(l);
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Last last;
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Init init;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Length {
   template <class L>
   struct Sig : public FunType<L,size_t> {};

   template <class L>
   size_t operator()( const L& ll ) const {
      List<typename L::ElementType> l = ll;
      size_t x = 0;
      while( !null(l) ) {
         l = tail(l);
         ++x;
      }
      return x;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Length length;
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN At _at;
FCPP_MAYBE_EXTERN Curryable2<At> at FCPP_MAYBE_INIT((_at));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Filter _filter;
FCPP_MAYBE_EXTERN Curryable2<Filter> filter FCPP_MAYBE_INIT((_filter));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Concat concat;
FCPP_MAYBE_NAMESPACE_CLOSE

// Note: this isn't lazy (even if 'op' is 'cons').
struct Foldr {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,E> {};

   template <class Op, class E, class L>
   E operator()( const Op& op, const E& e, const L& l ) const {
      if( null(l) )
         return e;
      else 
         return op( head(l), Foldr()( op, e, tail(l) ) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldr _foldr;
FCPP_MAYBE_EXTERN Curryable3<Foldr> foldr FCPP_MAYBE_INIT((_foldr));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Foldr1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::ElementType> {};

   template <class Op, class L>
   typename L::ElementType operator()( const Op& op, const L& l ) const {
      return foldr( op, head(l), tail(l) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldr1 _foldr1;
FCPP_MAYBE_EXTERN Curryable2<Foldr1> foldr1 FCPP_MAYBE_INIT((_foldr1));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Foldl {
   template <class Op, class E, class L>
   struct Sig : public FunType<Op,E,L,E> {};

   template <class Op, class E, class L>
   E operator()( const Op& op, E e, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( !null(l) ) {
         E tmp( e );
         e.~E();
         new (&e) E( op(tmp,head(l)) );
         l = tail(l);
      }
      return e;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldl _foldl;
FCPP_MAYBE_EXTERN Curryable3<Foldl> foldl FCPP_MAYBE_INIT((_foldl));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Foldl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,typename L::ElementType> {};

   template <class Op, class L>
   typename L::ElementType operator()( const Op& op, const L& l ) const {
      return foldl( op, head(l), tail(l) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Foldl1 _foldl1;
FCPP_MAYBE_EXTERN Curryable2<Foldl1> foldl1 FCPP_MAYBE_INIT((_foldl1));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanr _scanr;
FCPP_MAYBE_EXTERN Curryable3<Scanr> scanr FCPP_MAYBE_INIT((_scanr));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanr1 _scanr1;
FCPP_MAYBE_EXTERN Curryable2<Scanr1> scanr1 FCPP_MAYBE_INIT((_scanr1));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanl _scanl;
FCPP_MAYBE_EXTERN Curryable3<Scanl> scanl FCPP_MAYBE_INIT((_scanl));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Scanl1 {
   template <class Op, class L>
   struct Sig : public FunType<Op,L,OddList<typename L::ElementType> > {};

   template <class Op, class L>
   OddList<typename L::ElementType> 
   operator()( const Op& op, const L& l ) const {
      return scanl( op, head(l), tail(l) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Scanl1 _scanl1;
FCPP_MAYBE_EXTERN Curryable2<Scanl1> scanl1 FCPP_MAYBE_INIT((_scanl1));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Iterate _iterate;
FCPP_MAYBE_EXTERN Curryable2<Iterate> iterate FCPP_MAYBE_INIT((_iterate));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Repeat repeat;
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Map _map;
FCPP_MAYBE_EXTERN Curryable2<Map> map FCPP_MAYBE_INIT((_map));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Take _take;
FCPP_MAYBE_EXTERN Curryable2<Take> take FCPP_MAYBE_INIT((_take));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Drop {
   template <class Dummy, class L>
   struct Sig : public FunType<size_t,L,List<typename L::ElementType> > {};
   
   template <class L>
   List<typename L::ElementType> operator()( size_t n, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( n!=0 && !null(l) ) {
         --n;
         l = tail(l);
      }
      return l;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Drop _drop;
FCPP_MAYBE_EXTERN Curryable2<Drop> drop FCPP_MAYBE_INIT((_drop));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN TakeWhile _takeWhile; 
FCPP_MAYBE_EXTERN Curryable2<TakeWhile> takeWhile FCPP_MAYBE_INIT((_takeWhile));
FCPP_MAYBE_NAMESPACE_CLOSE

struct DropWhile {
   template <class P, class L>
   struct Sig : public FunType<P,L,List<typename L::ElementType> > {};

   template <class P, class L>
   List<typename L::ElementType> operator()( const P& p, const L& ll ) const {
      List<typename L::ElementType> l = ll;
      while( !null(l) && p( head(l) ) )
         l = tail(l);
      return l;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN DropWhile _dropWhile;
FCPP_MAYBE_EXTERN Curryable2<DropWhile> dropWhile FCPP_MAYBE_INIT((_dropWhile));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Replicate {
   template <class N, class T>
   struct Sig : public FunType<N,T,OddList<T> > {};

   template <class T>
   OddList<T> operator()( size_t n, const T& x ) const {
      return take( n, repeat(x) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Replicate _replicate; 
FCPP_MAYBE_EXTERN Curryable2<Replicate> replicate FCPP_MAYBE_INIT((_replicate));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Cycle cycle; 
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN SplitAt _splitAt; 
FCPP_MAYBE_EXTERN Curryable2<SplitAt> splitAt FCPP_MAYBE_INIT((_splitAt));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Span _span; 
FCPP_MAYBE_EXTERN Curryable2<Span> span FCPP_MAYBE_INIT((_span));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Break {
   template <class P, class L>
   struct Sig : public FunType<P,L,std::pair<List<typename
      L::ElementType>,List<typename L::ElementType> > > {};

   template <class P, class T>
   std::pair<List<T>,List<T> > 
   operator()( const P& p, const List<T>& l ) const {
      return span( Compose()( LogicalNot(), p ), l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Break _break_;
FCPP_MAYBE_EXTERN Curryable2<Break> break_ FCPP_MAYBE_INIT((_break_));   
   // C++ keyword, so add trailing underscore
FCPP_MAYBE_NAMESPACE_CLOSE

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
   struct Sig : public FunType<Binary,Curryable2<FlipHelper<Binary> > > {};

   template <class Binary>
   Curryable2<FlipHelper<Binary> > operator()( const Binary& op ) const {
      return Curryable2<FlipHelper<Binary> >( FlipHelper<Binary>( op ) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Flip flip;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Reverse {
   template <class L>
   struct Sig : public FunType<L,List<typename L::ElementType> > {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return curry3( foldl, flip(cons), List<T>(), l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Reverse reverse;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// Not HSP but close
//////////////////////////////////////////////////////////////////////

// These next two are defined as _lazy_ versions of these operators on lists
struct And : public CFunType<List<bool>,bool> {
   bool operator()( const List<bool>& l ) const {
      return null(dropWhile( curry2(Equal(),true), l ));
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN And and_;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Or : public CFunType<List<bool>,bool> {
   bool operator()( const List<bool>& l ) const {
      return !null(dropWhile( curry2(Equal(),false), l ));
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Or or_;
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN All _all;
FCPP_MAYBE_EXTERN Curryable2<All> all FCPP_MAYBE_INIT((_all));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Any {
   template <class P, class L>
   struct Sig : public FunType<P,L,bool> {};

   template <class P, class L>
   bool operator()( const P& p, const L& l ) const {
      return Or()( map( p, l ) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Any _any; 
FCPP_MAYBE_EXTERN Curryable2<Any> any FCPP_MAYBE_INIT((_any));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Elem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T, class L>
   bool operator()( const T& x, const L& l ) const {
      return any( bind2of2( Equal(), x ), l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Elem _elem; 
FCPP_MAYBE_EXTERN Curryable2<Elem> elem FCPP_MAYBE_INIT((_elem));
FCPP_MAYBE_NAMESPACE_CLOSE

struct NotElem {
   template <class T, class L>
   struct Sig : public FunType<T,L,bool> {};

   template <class T, class L>
   bool operator()( const T& x, const L& l ) const {
      return all( bind2of2( NotEqual(), x ), l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN NotElem _notElem; 
FCPP_MAYBE_EXTERN Curryable2<NotElem> notElem FCPP_MAYBE_INIT((_notElem));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Sum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl( Plus(), 0, l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Sum sum;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Product {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl( Multiplies(), 1, l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Product product;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Minimum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl1( Min(), l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Minimum minimum; 
FCPP_MAYBE_NAMESPACE_CLOSE

struct Maximum {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class L>
   typename L::ElementType operator()( const L& l ) const {
      return foldl1( Max(), l );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Maximum maximum; 
FCPP_MAYBE_NAMESPACE_CLOSE

#ifdef FCPP_SIMPLE_PRELUDE
struct ZipWith {
   template <class Z, class LA, class LB>
   struct Sig : public FunType<Z,LA,LB,
   List<typename Z::template Sig<typename LA::ElementType,
                                 typename LB::ElementType>::ResultType> > {};

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
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType>::ResultType> > {};

   template <class Z, class LA, class LB>
   OddList<typename RT<Z,typename LA::ElementType,
                         typename LB::ElementType>::ResultType>
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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN ZipWith _zipWith; 
FCPP_MAYBE_EXTERN Curryable3<ZipWith> zipWith FCPP_MAYBE_INIT((_zipWith));
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Zip _zip;
FCPP_MAYBE_EXTERN Curryable2<Zip> zip FCPP_MAYBE_INIT((_zip));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Fst {
   template <class P>
   struct Sig : public FunType<P,typename P::first_type> {};

   template <class A, class B>
   A operator()( const std::pair<A,B>& p ) const {
      return p.first;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Fst fst;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Snd {
   template <class P>
   struct Sig : public FunType<P,typename P::second_type> {};

   template <class A, class B>
   B operator()( const std::pair<A,B>& p ) const {
      return p.second;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Snd snd;
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Unzip unzip;
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Gcd _gcd; 
FCPP_MAYBE_EXTERN Curryable2<Gcd> gcd FCPP_MAYBE_INIT((_gcd));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Odd {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==1;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Odd odd; 
FCPP_MAYBE_NAMESPACE_CLOSE

struct Even {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T& x ) const {
      return x%2==0;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Even even; 
FCPP_MAYBE_NAMESPACE_CLOSE

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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN EnumFrom enumFrom; 
FCPP_MAYBE_NAMESPACE_CLOSE

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
      return cons( x-1, Fun0<OddList<T> >( 1, this ) );
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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN EnumFromTo _enumFromTo; 
FCPP_MAYBE_EXTERN Curryable2<EnumFromTo> enumFromTo FCPP_MAYBE_INIT((_enumFromTo));
FCPP_MAYBE_NAMESPACE_CLOSE

// Not HSP
struct ListUntil {
   template <class P, class F, class T>
   struct Sig : public FunType<P,F,T,List<T> > {};

   template <class Pred, class Unary, class T>
   List<T> operator()( const Pred& p, const Unary& f, const T& x ) const {
      return takeWhile( Compose()(logicalNot,p), iterate(f,x) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN ListUntil _listUntil;
FCPP_MAYBE_EXTERN Curryable3<ListUntil> listUntil FCPP_MAYBE_INIT((_listUntil));
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// The "Maybe" type, from Haskell
//////////////////////////////////////////////////////////////////////

template <class T>
class Maybe {
   OddList<T> rep;
public:
   Maybe() {}                                    // the Nothing constructor
   Maybe( const T& x ) : rep( cons(x,NIL) ) {}   // the Just constructor

   bool is_nothing() const { return !rep; }
   T value() const { return head(rep); }
};

// That's the end of the Haskell stuff; on to made-just-for-FC++

//////////////////////////////////////////////////////////////////////
// Useful effect combinators
//////////////////////////////////////////////////////////////////////
// Here we define some combinators for statement sequencing:
//    before(f,g)(args) = { f(); return g(args); }
//    after(f,g)(args)  = { r = f(args); g(); return r; }
// That is, before() prepends a thunk onto a functoid, and after()
// appends the thunk onto the back of a functoid.  Finally, NoOp()
// results in a thunk that does nothing, and serves as the left/right
// identity element for before/after thusly:
//    f  =  before( NoOp(), f )  =  after( f, NoOp() )

struct NoOp : public CFunType<void> {
   void operator()() const {}
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN NoOp noOp;
FCPP_MAYBE_NAMESPACE_CLOSE

template <class F, class G>
class BeforerBase {
protected:
   F f;
   G g;
   BeforerBase( const F& ff, const G& gg ) : f(ff), g(gg) {}
};
template <class F, class G, bool>
class BeforerMixin : public CallableWithoutArguments, public BeforerBase<F,G> {
protected:
   using BeforerBase<F,G>::f;
   using BeforerBase<F,G>::g;
   BeforerMixin( const F& ff, const G& gg ) : BeforerBase<F,G>(ff,gg) {}
public:
   typename RT<G>::ResultType 
   operator()() const { f(); return g(); }
};
template <class F, class G>
class BeforerMixin<F,G,false> : public BeforerBase<F,G> {
protected:
   using BeforerBase<F,G>::f;
   using BeforerBase<F,G>::g;
   BeforerMixin( const F& ff, const G& gg ) : BeforerBase<F,G>(ff,gg) {}
   template <class T> struct Impossible {};
public:
   template <class T> void operator()( Impossible<T>& ) {}
};
template <class F, class G>
class Beforer 
: public BeforerMixin<F,G,Inherits<G,CallableWithoutArguments>::value> {
   typedef BeforerMixin<F,G,Inherits<G,CallableWithoutArguments>::value> Super;
   using Super::f;
   using Super::g;
public:
#ifndef FCPP_NO_USING_DECLS
   using Super::operator();
#endif
   Beforer( const F& ff, const G& gg ) : Super(ff,gg) {}

   template <class X=Void, class Y=Void, class Z=Void, class Dummy=Void>
   struct Sig : FunType<typename RT<G,X,Y,Z>::Arg1Type,
                        typename RT<G,X,Y,Z>::Arg2Type,
                        typename RT<G,X,Y,Z>::Arg3Type,
                        typename RT<G,X,Y,Z>::ResultType> {};
   template <class X, class Y, class Dummy>
   struct Sig<X,Y,Void,Dummy> : FunType<typename RT<G,X,Y>::Arg1Type,
                                        typename RT<G,X,Y>::Arg2Type,
                                        typename RT<G,X,Y>::ResultType> {};
   template <class X, class Dummy>
   struct Sig<X,Void,Void,Dummy> : FunType<typename RT<G,X>::Arg1Type,
                                           typename RT<G,X>::ResultType> {};
   template <class Dummy>
   struct Sig<Void,Void,Void,Dummy> : FunType<typename RT<G>::ResultType> {};

   template <class X>
   typename Sig<X>::ResultType 
   operator()( const X& x ) const { f(); return g(x); }

   template <class X, class Y>
   typename Sig<X,Y>::ResultType 
   operator()( const X& x, const Y& y ) const { f(); return g(x,y); }

   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const 
   { f(); return g(x,y,z); }
};

struct Before {
   template <class F, class G>
   struct Sig : public FunType<F,G,Beforer<F,G> > {};

   template <class F, class G>
   Beforer<F,G> operator()( const F& f, const G& g ) const {
      return Beforer<F,G>( f, g );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Before before_;
FCPP_MAYBE_EXTERN Curryable2<Before> before FCPP_MAYBE_INIT((before_));
FCPP_MAYBE_NAMESPACE_CLOSE

template <class F, class G>
class AftererBase {
protected:
   F f;
   G g;
   AftererBase( const F& ff, const G& gg ) : f(ff), g(gg) {}
};
template <class F, class G, bool>
class AftererMixin : public CallableWithoutArguments, public AftererBase<F,G> {
protected:
   using AftererBase<F,G>::f;
   using AftererBase<F,G>::g;
   AftererMixin( const F& ff, const G& gg ) : AftererBase<F,G>(ff,gg) {}
public:
   typename RT<F>::ResultType 
   operator()() const { 
      typename RT<F>::ResultType res( f() );
      g();
      return res;
   }
};
template <class F, class G>
class AftererMixin<F,G,false> : public AftererBase<F,G> {
protected:
   using AftererBase<F,G>::f;
   using AftererBase<F,G>::g;
   AftererMixin( const F& ff, const G& gg ) : AftererBase<F,G>(ff,gg) {}
   template <class T> struct Impossible {};
public:
   template <class T> void operator()( Impossible<T>& ) {}
};
template <class F, class G>
class Afterer 
: public AftererMixin<F,G,Inherits<F,CallableWithoutArguments>::value> {
   typedef AftererMixin<F,G,Inherits<F,CallableWithoutArguments>::value> Super;
   using Super::f;
   using Super::g;
public:
#ifndef FCPP_NO_USING_DECLS
   using Super::operator();
#endif
   Afterer( const F& ff, const G& gg ) : Super(ff,gg) {}

   template <class X=Void, class Y=Void, class Z=Void, class Dummy=Void>
   struct Sig : FunType<typename RT<F,X,Y,Z>::Arg1Type,
                        typename RT<F,X,Y,Z>::Arg2Type,
                        typename RT<F,X,Y,Z>::Arg3Type,
                        typename RT<F,X,Y,Z>::ResultType> {};
   template <class X, class Y, class Dummy>
   struct Sig<X,Y,Void,Dummy> : FunType<typename RT<F,X,Y>::Arg1Type,
                                        typename RT<F,X,Y>::Arg2Type,
                                        typename RT<F,X,Y>::ResultType> {};
   template <class X, class Dummy>
   struct Sig<X,Void,Void,Dummy> : FunType<typename RT<F,X>::Arg1Type,
                                           typename RT<F,X>::ResultType> {};
   template <class Dummy>
   struct Sig<Void,Void,Void,Dummy> : FunType<typename RT<F>::ResultType> {};

   template <class X>
   typename Sig<X>::ResultType 
   operator()( const X& x ) const { 
      typename RT<F,X>::ResultType res( f(x) );
      g();
      return res;
   }

   template <class X, class Y>
   typename Sig<X,Y>::ResultType 
   operator()( const X& x, const Y& y ) const { 
      typename RT<F,X,Y>::ResultType res( f(x,y) );
      g();
      return res;
   }

   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const {
      typename RT<F,X,Y,Z>::ResultType res( f(x,y,z) );
      g();
      return res;
   }
};

struct After {
   template <class F, class G>
   struct Sig : public FunType<F,G,Afterer<F,G> > {};

   template <class F, class G>
   Afterer<F,G> operator()( const F& f, const G& g ) const {
      return Afterer<F,G>( f, g );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN After after_;
FCPP_MAYBE_EXTERN Curryable2<After> after FCPP_MAYBE_INIT((after_));
FCPP_MAYBE_NAMESPACE_CLOSE
   
//////////////////////////////////////////////////////////////////////
// uncurry
//////////////////////////////////////////////////////////////////////
// Sometimes FC++ expressions result in functoids where, for example,
//    f(x)(y)
// is legal but
//    f(x,y)
// is not, owing to the fact that (in the example) f() is a one-argument
// functoid the returns another one-argument functoid, which is 
// different from a two-argument functoid.  (In Haskell, the two types
// are identical.)  uncurry() wraps a functoid in a magical cloak which
// splits up its arguments, so that, for example,
//    uncurry(f)(x,y,z) = f(x)(y)(z)
// It rarely arises that you need this, but when you do, you can't live
// without it.
//
// Note that uncurry() (as well as curryN()) means something different
// in FC++ than what it does in Haskell.

template <class F>
class UncurryableBase {
protected:
   F f;
   UncurryableBase( const F& ff ) : f(ff) {}
};
template <class F, bool>
class UncurryableMixin 
: public CallableWithoutArguments, public UncurryableBase<F> {
protected:
   using UncurryableBase<F>::f;
   UncurryableMixin ( const F& ff ) : UncurryableBase<F>(ff) {}
public:
   typename RT<F>::ResultType
   operator()() const { return f(); }
};
template <class F>
class UncurryableMixin<F,false> : public UncurryableBase<F> {
protected:
   using UncurryableBase<F>::f;
   UncurryableMixin ( const F& ff ) : UncurryableBase<F>(ff) {}
   template <class T> struct Impossible {};
public:
   template <class T> void operator()( Impossible<T>& ) {}
};
template <class F>
class Uncurryable 
: public UncurryableMixin<F,Inherits<F,CallableWithoutArguments>::value> {
   typedef UncurryableMixin<F,Inherits<F,CallableWithoutArguments>::value> 
           Super;
   using Super::f;
public:
#ifndef FCPP_NO_USING_DECLS
   using Super::operator();
#endif
   Uncurryable( const F& ff ) : Super(ff) {}

   template <class X=Void, class Y=Void, class Z=Void, class Dummy=Void>
   struct Sig : public FunType<typename RT<F,X>::Arg1Type,
            typename RT<typename RT<F,X>::ResultType,Y>::Arg1Type,
typename RT<typename RT<typename RT<F,X>::ResultType,Y>::ResultType,Z>
   ::Arg1Type,
typename RT<typename RT<typename RT<F,X>::ResultType,Y>::ResultType,Z>
   ::ResultType> {};

   template <class X, class Y, class Dummy>
   struct Sig<X,Y,Void,Dummy> : public FunType<typename RT<F,X>::Arg1Type,
            typename RT<typename RT<F,X>::ResultType,Y>::Arg1Type,
            typename RT<typename RT<F,X>::ResultType,Y>::ResultType> {};

   template <class X, class Dummy>
   struct Sig<X,Void,Void,Dummy> : public FunType<typename RT<F,X>::Arg1Type,
            typename RT<F,X>::ResultType> {};

   template <class Dummy>
   struct Sig<Void,Void,Void,Dummy> : FunType<typename RT<F>::ResultType> {};

   template <class X, class Y, class Z>
   typename Sig<X,Y,Z>::ResultType
   operator()( const X& x, const Y& y, const Z& z ) const { return f(x)(y)(z); }

   template <class X, class Y>
   typename Sig<X,Y>::ResultType
   operator()( const X& x, const Y& y ) const { return f(x)(y); }

   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& x ) const { return f(x); }
};

struct Uncurry {
   template <class F>
   struct Sig : FunType<F,Uncurryable<F> > {};

   template <class F>
   Uncurryable<F> operator()( const F& f ) const { return Uncurryable<F>(f); }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Uncurry uncurry;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// duplicate() and ignore()
//////////////////////////////////////////////////////////////////////
// duplicate() duplicates the first argument of a functoid, whereas
// ignore() ignores it:
//    duplicate(f)(x)    = f(x)(x)
//    ignore(f)(x)(args) = f(args)

template <class F>
class Duplicater {
   F f;
public:
   Duplicater( const F& ff ) : f(ff) {}

   template <class X>
   struct Sig : public FunType<X,typename RT<typename RT<F,X>::ResultType,
                                             X>::ResultType> {};
   template <class X>
   typename Sig<X>::ResultType
   operator()( const X& x ) const {
      return f(x)(x);
   }
};
struct Duplicate {
   template <class F>
   struct Sig : public FunType<F,Duplicater<F> > {};

   template <class F>
   Duplicater<F> operator()( const F& f ) const {
      return Duplicater<F>(f);
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Duplicate duplicate;
FCPP_MAYBE_NAMESPACE_CLOSE

template <class F, bool>
class Ignorer {
   F f;
public:
   Ignorer( const F& ff ) : f(ff) {}

   template <class X> struct Sig : public FunType<X,F> {};

   template <class X>
   F operator()( const X& ) const {
      return f;
   }
};
template <class F>
class Ignorer<F,true> {
   F f;
public:
   Ignorer( const F& ff ) : f(ff) {}

   template <class X> 
   struct Sig : public FunType<X,typename RT<F>::ResultType> {};

   template <class X>
   typename RT<F>::ResultType operator()( const X& ) const {
      return f();
   }
};
struct Ignore {
   template <class F>
   struct Sig : public FunType<F,Uncurryable<Ignorer<F,
      Inherits<F,CallableWithoutArguments>::value> > > {};

   template <class F>
   typename Sig<F>::ResultType operator()( const F& f ) const {
      return uncurry( Ignorer<F,
         Inherits<F,CallableWithoutArguments>::value>(f) );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Ignore ignore;
FCPP_MAYBE_NAMESPACE_CLOSE

//////////////////////////////////////////////////////////////////////
// ConstructN
//////////////////////////////////////////////////////////////////////
// C++ constructors are not functions, and thus cannot easily be turned
// into functoids.  So we write these helpers.  For example,
//    construct2<Foo>()(x,y) = Foo(x,y)    // Foo is a type name
// Note also that construct1 also serves the role of an explicit
// converter; if Foos (or any type) can be converted into Bars, then we 
// could use a construct1 functoid to capture the conversion function:
//    construct1<Bar>()       // functoid that converts arg into a Bar
//    construct1<Bar>()(x) = Bar(x)

template <class T>
struct Construct0 : public CFunType<T> {
   T operator()() const { return T(); }
};
template <class T> 
Construct0<T> construct0() { return Construct0<T>(); }

template <class T>
struct Construct1 {
   template <class X> struct Sig : FunType<X,T> {};
   template <class X>
   T operator()( const X& x ) const { return T(x); }
};
template <class T> 
Construct1<T> construct1() { return Construct1<T>(); }
   
template <class T>
struct Construct2 {
   template <class X, class Y> struct Sig : FunType<X,Y,T> {};
   template <class X, class Y>
   T operator()( const X& x, const Y& y ) const { return T(x,y); }
};
template <class T> 
Curryable2<Construct2<T> > construct2() 
{ return makeCurryable2( Construct2<T>() ); }
   
template <class T>
struct Construct3 {
   template <class X, class Y, class Z> struct Sig : FunType<X,Y,Z,T> {};
   template <class X, class Y, class Z>
   T operator()( const X& x, const Y& y, const Z& z ) const { return T(x,y,z); }
};
template <class T> 
Curryable3<Construct3<T> > construct3() 
{ return makeCurryable3( Construct3<T>() ); }
   
// NewN works like ConstructN but "new"s it and returns the ptr
template <class T>
struct New0 : public CFunType<T*> {
   T* operator()() const { return new T(); }
};
template <class T> New0<T> new0() { return New0<T>(); }

template <class T>
struct New1 {
   template <class X> struct Sig : FunType<X,T*> {};
   template <class X>
   T* operator()( const X& x ) const { return new T(x); }
};
template <class T> New1<T> new1() { return New1<T>(); }
   
template <class T>
struct New2 {
   template <class X, class Y> struct Sig : FunType<X,Y,T*> {};
   template <class X, class Y>
   T* operator()( const X& x, const Y& y ) const { return new T(x,y); }
};
template <class T> 
Curryable2<New2<T> > new2() { return makeCurryable2( New2<T>() ); }
   
template <class T>
struct New3 {
   template <class X, class Y, class Z> struct Sig : FunType<X,Y,Z,T*> {};
   template <class X, class Y, class Z>
   T* operator()( const X& x, const Y& y, const Z& z ) const 
   { return new T(x,y,z); }
};
template <class T> 
Curryable3<New3<T> > new3() { return makeCurryable3( New3<T>() ); }
   

} // end namespace fcpp

#endif
