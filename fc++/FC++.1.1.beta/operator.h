//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef OPERATOR_DOT_H
#define OPERATOR_DOT_H

//////////////////////////////////////////////////////////////////////
// The goal here is to provide functoids for all C++ operators (e.g.
// Plus, Greater, ...) as well as conversions between representations.
// The conversions include ptr_fun, for turning function pointers into
// functoids, stlN_fun, for turning STL functoids into our functoids,
// and monomophizeN, for converting polymorphic direct functoids into
// monomorphic ones.
//
// There's also some miscellaneous at the end of this file, for lack of
// a better place to put it.
//////////////////////////////////////////////////////////////////////

#include <functional>
#include "signature.h"

//////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////

struct MakePair {
   template <class A, class B>
   struct Sig : public FunType<A,B,pair<A,B> > {};

   template <class A, class B>
   pair<A,B> operator()( const A& a, const B& b ) const {
      return make_pair(a,b);
   }
};

struct Min {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return less<T>()( x, y ) ? x : y;
   }
};

struct Max {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return less<T>()( x, y ) ? y : x;
   }
};

struct Plus {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return plus<T>()( x, y );
   }
};

struct Minus {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return minus<T>()( x, y );
   }
};

struct Multiplies {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return multiplies<T>()( x, y );
   }
};

struct Divides {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return divides<T>()( x, y );
   }
};

struct Modulus {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return modulus<T>()( x, y );
   }
};

struct Negate {
   template <class T>
   struct Sig : public FunType<T,T> {};

   template <class T>
   T operator()( const T& x ) const {
      return negate<T>()( x );
   }
};

struct Equal {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return equal_to<T>()( x, y );
   }
};

struct NotEqual {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return not_equal_to<T>()( x, y );
   }
};

struct Greater {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return greater<T>()( x, y );
   }
};

struct Less {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return less<T>()( x, y );
   }
};

struct GreaterEqual {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return greater_equal<T>()( x, y );
   }
};

struct LessEqual {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return less_equal<T>()( x, y );
   }
};

struct LogicalAnd {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return logical_and<T>()( x, y );
   }
};

struct LogicalOr {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return logical_or<T>()( x, y );
   }
};

struct LogicalNot {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T&x ) const {
      return logical_not<T>()( x );
   }
};

// The other "higher-order" operators in STL are easy to do in our
// framework. For instance, instead of unary_negate (with all its
// complicated type info), we can just say compose1(logicalnot, f).


//////////////////////////////////////////////////////////////////////
// conversions
//////////////////////////////////////////////////////////////////////

template <class Op>
class stl1 : CFunType<typename Op::argument_type,
                       typename Op::result_type> {
   Op f;
public:
   stl1( const Op& o ) : f(o) {}
   ResultType operator()( const Arg1Type& x ) const {
      return f(x);
   }
};

template <class Op>
stl1<Op> stl_to_fun1( const Op& o ) {
   return stl1<Op>(o);
}

template <class Op>
class stl2 : CFunType<typename Op::first_argument_type,
                       typename Op::second_argument_type,
                       typename Op::result_type> {
   Op f;
public:
   stl2( const Op& o ) : f(o) {}
   ResultType operator()( const Arg1Type& x, const Arg2Type& y ) const {
      return f(x,y);
   }
};

template <class Op>
stl2<Op> stl_to_fun2( const Op& o ) {
   return stl2<Op>(o);
}

//////////////////////////////////////////////////////////////////////
// monomorphizing conversions
//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Arg3, class Res, class F>
class MonomorphicWrapper3 : public CFunType<Arg1,Arg2,Arg3,Res> {
   F f;
public:
   MonomorphicWrapper3( const F& g ) : f(g) {}
   Res operator()( const Arg1& x, const Arg2& y, const Arg3& z ) const {
      return f(x,y,z);
   }
};

template <class Arg1, class Arg2, class Arg3, class Res, class F>
MonomorphicWrapper3<Arg1,Arg2,Arg3,Res,F> monomorphize3( const F& f ) {
   return MonomorphicWrapper3<Arg1,Arg2,Arg3,Res,F>( f );
}

template <class Arg1, class Arg2, class Res, class F>
class MonomorphicWrapper2 : public CFunType<Arg1,Arg2,Res> {
   F f;
public:
   MonomorphicWrapper2( const F& g ) : f(g) {}
   Res operator()( const Arg1& x, const Arg2& y ) const {
      return f(x,y);
   }
};

template <class Arg1, class Arg2, class Res, class F>
MonomorphicWrapper2<Arg1,Arg2,Res,F> monomorphize2( const F& f ) {
   return MonomorphicWrapper2<Arg1,Arg2,Res,F>( f );
}

template <class Arg1, class Res, class F>
class MonomorphicWrapper1 : public CFunType<Arg1,Res> {
   F f;
public:
   MonomorphicWrapper1( const F& g ) : f(g) {}
   Res operator()( const Arg1& x ) const {
      return f(x);
   }
};

template <class Arg1, class Res, class F>
MonomorphicWrapper1<Arg1,Res,F> monomorphize1( const F& f ) {
   return MonomorphicWrapper1<Arg1,Res,F>( f );
}

//////////////////////////////////////////////////////////////////////
// ptr_fun
//////////////////////////////////////////////////////////////////////

template <class Arg, class Result>
class ptr_to_unary_function : public CFunType<Arg, Result> {
    Result (*ptr)(Arg);
public:
    explicit ptr_to_unary_function(Result (*x)(Arg)) : ptr(x) {}
    Result operator()(Arg x) const { return ptr(x); }
};

template <class Arg, class Result>
inline ptr_to_unary_function<Arg, Result>
ptr_to_fun(Result (*x)(Arg)) {
  return ptr_to_unary_function<Arg, Result>(x);
}

template <class Arg1, class Arg2, class Result>
class ptr_to_binary_function : public CFunType<Arg1, Arg2, Result> {
    Result (*ptr)(Arg1, Arg2);
public:
    explicit ptr_to_binary_function(Result (*x)(Arg1, Arg2)) : ptr(x) {}
    Result operator()(Arg1 x, Arg2 y) const { return ptr(x, y); }
};

template <class Arg1, class Arg2, class Result>
inline ptr_to_binary_function<Arg1, Arg2, Result>
ptr_to_fun(Result (*x)(Arg1, Arg2)) {
  return ptr_to_binary_function<Arg1, Arg2, Result>(x);
}

//////////////////////////////////////////////////////////////////////
// Turn member functions into normal functions which take a Receiver*
// as their first (extra) argument.  Note that we disallow reference
// parameters.
//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Arg3, class Result>
class ptr_to_mem_binary_function : public CFunType<Arg1*, Arg2, Arg3, Result> {
    Result (Arg1::*ptr)(Arg2,Arg3);
public:
    explicit ptr_to_mem_binary_function(Result (Arg1::*x)(Arg2,Arg3)) 
       : ptr(x) {}
    Result operator()(Arg1* x, const Arg2& y, const Arg3& z) const 
       { return (x->*ptr)(y,z); }
};

template <class Arg1, class Arg2, class Arg3, class Result>
inline ptr_to_mem_binary_function<Arg1, Arg2, Arg3, Result>
ptr_to_fun(Result (Arg1::*x)(Arg2,Arg3)) {
  return ptr_to_mem_binary_function<Arg1, Arg2, Arg3, Result>(x);
}

template <class Arg1, class Arg2, class Result>
class ptr_to_mem_unary_function : public CFunType<Arg1*, Arg2, Result> {
    Result (Arg1::*ptr)(Arg2);
public:
    explicit ptr_to_mem_unary_function(Result (Arg1::*x)(Arg2)) : ptr(x) {}
    Result operator()(Arg1* x, const Arg2& y) const { return (x->*ptr)(y); }
};

template <class Arg1, class Arg2, class Result>
inline ptr_to_mem_unary_function<Arg1, Arg2, Result>
ptr_to_fun(Result (Arg1::*x)(Arg2)) {
  return ptr_to_mem_unary_function<Arg1, Arg2, Result>(x);
}

template <class Arg1, class Result>
class ptr_to_mem_nullary_function : public CFunType<Arg1*, Result> {
    Result (Arg1::*ptr)();
public:
    explicit ptr_to_mem_nullary_function(Result (Arg1::*x)()) : ptr(x) {}
    Result operator()(Arg1* x) const { return (x->*ptr)(); }
};

template <class Arg1, class Result>
inline ptr_to_mem_nullary_function<Arg1, Result>
ptr_to_fun(Result (Arg1::*x)()) {
  return ptr_to_mem_nullary_function<Arg1, Result>(x);
}

//////////////////////////////////////////////////////////////////////
// Misc stuff
//////////////////////////////////////////////////////////////////////

struct Inc {
    template <class T>
    struct Sig : public FunType<T,T> {};

    template <class T>
    T operator()(const T& x) const { T y = x; return ++y; }
};
namespace { Inc inc; }

struct Always1 {
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return true; }
};

struct Never1 {
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return false; }
};

//////////////////////////////////////////////////////////////////////
// List comparisons
//////////////////////////////////////////////////////////////////////

// natural orderings
template <class T>
bool operator==( const List<T>& a, const List<T>& b ) {
   if( null(a) && null(b) )
      return true;
   if( null(a) || null(b) )
      return false;
   return (head(a)==head(b)) && (tail(a)==tail(b));
}

template <class T>
bool operator<( const List<T>& a, const List<T>& b ) {
   if( null(a) )
      return true;
   if( null(b) )
      return false;
   return (head(a) < head(b)) ||
          ((head(a) == head(b)) && (tail(a) < tail(b)));
}

// FIX THIS: Need to implement versions that take predicates

//////////////////////////////////////////////////////////////////////
// Handy functions for making list literals
//////////////////////////////////////////////////////////////////////

template <class T>
List<T> list_with( const T& a ) {
   List<T> l;
   l = cons( a, l );
   return l;
}

template <class T>
List<T> list_with( const T& a, const T& b ) {
   List<T> l;
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

template <class T>
List<T> list_with( const T& a, const T& b, const T& c ) {
   List<T> l;
   l = cons( c, l );
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

template <class T>
List<T> list_with( const T& a, const T& b, const T& c, const T& d ) {
   List<T> l;
   l = cons( d, l );
   l = cons( c, l );
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

//////////////////////////////////////////////////////////////////////
// List operations on lazy lists
//////////////////////////////////////////////////////////////////////

template <class Pred, class Unary, class T>
class ListUntil : public CFunType<F0Proxy<T> > {
   Pred p;
   Unary f;
   T x;
public:
   ListUntil( const Pred& a, const Unary& b, const T& c ) : p(a), f(b),
x(c) {}
   ResultType operator()() const {
      if( p(x) )
         return Ref<pair<T,Fun0<ResultType> > >(0);
      else
         return Ref<pair<T,Fun0<ResultType> > >
                (new pair<T,Fun0<ResultType> >(x,
                    makeFun0(ListUntil<Pred,Unary,T>(p,f,f(x)))));
   }
};

template <class Pred, class Unary, class T>
ListUntil<Pred,Unary,T>
list_until( const Pred& p, const Unary& op, const T& x ) {
   return ListUntil<Pred,Unary,T>(p,op,x);
}

#endif
