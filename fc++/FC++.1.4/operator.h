//
// Copyright (c) 2000,2001,2002 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_OPERATOR_DOT_H
#define FCPP_OPERATOR_DOT_H

//////////////////////////////////////////////////////////////////////
// The goal here is to provide functoids for all C++ operators (e.g.
// Plus, Greater, ...) as well as conversions between representations.
// The conversions include ptr_to_fun, for turning function pointers into
// functoids, stl_to_funN, for turning STL functoids into our functoids,
// and monomophizeN, for converting polymorphic direct functoids into
// monomorphic ones.
//
// There's also some miscellaneous at the end of this file, for lack of
// a better place to put it.
//////////////////////////////////////////////////////////////////////

#include <functional>
#include <utility>
#include <iterator>
#include "curry.h"

namespace fcpp {

//////////////////////////////////////////////////////////////////////
// operators
//////////////////////////////////////////////////////////////////////

struct MakePair {
   template <class A, class B>
   struct Sig : public FunType<A,B,std::pair<A,B> > {};

   template <class A, class B>
   std::pair<A,B> operator()( const A& a, const B& b ) const {
      return std::make_pair(a,b);
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN MakePair _makePair;
FCPP_MAYBE_EXTERN Curryable2<MakePair> makePair FCPP_MAYBE_INIT((_makePair));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Min {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::less<T>()( x, y ) ? x : y;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Min _min;
FCPP_MAYBE_EXTERN Curryable2<Min> min FCPP_MAYBE_INIT((_min));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Max {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::less<T>()( x, y ) ? y : x;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Max _max;
FCPP_MAYBE_EXTERN Curryable2<Max> max FCPP_MAYBE_INIT((_max));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Plus {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::plus<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Plus _plus;
FCPP_MAYBE_EXTERN Curryable2<Plus> plus FCPP_MAYBE_INIT((_plus));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Minus {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::minus<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Minus _minus;
FCPP_MAYBE_EXTERN Curryable2<Minus> minus FCPP_MAYBE_INIT((_minus));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Multiplies {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::multiplies<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Multiplies _multiplies;
FCPP_MAYBE_EXTERN Curryable2<Multiplies> multiplies FCPP_MAYBE_INIT((_multiplies));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Divides {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::divides<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Divides _divides;
FCPP_MAYBE_EXTERN Curryable2<Divides> divides FCPP_MAYBE_INIT((_divides));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Modulus {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::modulus<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Modulus _modulus;
FCPP_MAYBE_EXTERN Curryable2<Modulus> modulus FCPP_MAYBE_INIT((_modulus));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Negate {
   template <class T>
   struct Sig : public FunType<T,T> {};

   template <class T>
   T operator()( const T& x ) const {
      return std::negate<T>()( x );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Negate negate;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Equal {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::equal_to<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Equal _equal;
FCPP_MAYBE_EXTERN Curryable2<Equal> equal FCPP_MAYBE_INIT((_equal));
FCPP_MAYBE_NAMESPACE_CLOSE

struct NotEqual {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::not_equal_to<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN NotEqual _notEqual;
FCPP_MAYBE_EXTERN Curryable2<NotEqual> notEqual FCPP_MAYBE_INIT((_notEqual));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Greater {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::greater<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Greater _greater;
FCPP_MAYBE_EXTERN Curryable2<Greater> greater FCPP_MAYBE_INIT((_greater));
FCPP_MAYBE_NAMESPACE_CLOSE

struct Less {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::less<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Less _less;
FCPP_MAYBE_EXTERN Curryable2<Less> less FCPP_MAYBE_INIT((_less));
FCPP_MAYBE_NAMESPACE_CLOSE

struct GreaterEqual {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::greater_equal<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN GreaterEqual _greaterEqual;
FCPP_MAYBE_EXTERN Curryable2<GreaterEqual> greaterEqual FCPP_MAYBE_INIT((_greaterEqual));
FCPP_MAYBE_NAMESPACE_CLOSE

struct LessEqual {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::less_equal<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN LessEqual _lessEqual;
FCPP_MAYBE_EXTERN Curryable2<LessEqual> lessEqual FCPP_MAYBE_INIT((_lessEqual));
FCPP_MAYBE_NAMESPACE_CLOSE

struct LogicalAnd {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::logical_and<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN LogicalAnd _logicalAnd;
FCPP_MAYBE_EXTERN Curryable2<LogicalAnd> logicalAnd FCPP_MAYBE_INIT((_logicalAnd));
FCPP_MAYBE_NAMESPACE_CLOSE

struct LogicalOr {
   template <class T, class U> struct Sig;

   template <class T>
   struct Sig<T,T> : public FunType<T,T,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::logical_or<T>()( x, y );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN LogicalOr _logicalOr;
FCPP_MAYBE_EXTERN Curryable2<LogicalOr> logicalOr FCPP_MAYBE_INIT((_logicalOr));
FCPP_MAYBE_NAMESPACE_CLOSE

struct LogicalNot {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T&x ) const {
      return std::logical_not<T>()( x );
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN LogicalNot logicalNot;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Dereference {
   template <class T> struct Sig 
   : public FunType<T,typename std::iterator_traits<T>::value_type> {};

   template <class T>
   typename Sig<T>::ResultType operator()( const T& p ) const {
      return *p;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Dereference dereference;
FCPP_MAYBE_NAMESPACE_CLOSE

struct AddressOf {
   template <class T>
   struct Sig : public FunType<T,const T*> {};

   template <class T>
   const T* operator()( const T& x ) const {
      return &x;
   }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN AddressOf addressOf;
FCPP_MAYBE_NAMESPACE_CLOSE

// The other "higher-order" operators in STL are easy to do in our
// framework. For instance, instead of unary_negate (with all its
// complicated type info), we can just say compose1(logicalnot, f).


//////////////////////////////////////////////////////////////////////
// STL conversions
//////////////////////////////////////////////////////////////////////

template <class Op>
class stl1 : public CFunType<typename Op::argument_type,
                       typename Op::result_type> {
   Op f;
public:
   stl1( const Op& o ) : f(o) {}
   typename Op::result_type 
   operator()( const typename Op::argument_type& x ) const {
      return f(x);
   }
};

template <class Op>
stl1<Op> stl_to_fun1( const Op& o ) {
   return stl1<Op>(o);
}

template <class Op>
class stl2 : public CFunType<typename Op::first_argument_type,
                       typename Op::second_argument_type,
                       typename Op::result_type> {
   Op f;
public:
   stl2( const Op& o ) : f(o) {}
   typename Op::result_type 
   operator()( const typename Op::first_argument_type& x, 
               const typename Op::second_argument_type& y ) const {
      return f(x,y);
   }
};

template <class Op>
Curryable2<stl2<Op> > stl_to_fun2( const Op& o ) {
   return makeCurryable2(stl2<Op>(o));
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
Curryable3<MonomorphicWrapper3<Arg1,Arg2,Arg3,Res,F> > 
monomorphize3( const F& f ) {
   return makeCurryable3(MonomorphicWrapper3<Arg1,Arg2,Arg3,Res,F>( f ));
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
Curryable2<MonomorphicWrapper2<Arg1,Arg2,Res,F> > 
monomorphize2( const F& f ) {
   return makeCurryable2(MonomorphicWrapper2<Arg1,Arg2,Res,F>( f ));
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

template <class Res, class F>
class MonomorphicWrapper0 : public CFunType<Res> {
   F f;
public:
   MonomorphicWrapper0( const F& g ) : f(g) {}
   Res operator()() const {
      return f();
   }
};

template <class Res, class F>
MonomorphicWrapper0<Res,F> monomorphize0( const F& f ) {
   return MonomorphicWrapper0<Res,F>( f );
}

//////////////////////////////////////////////////////////////////////
// ptr_fun
//////////////////////////////////////////////////////////////////////

template <class Result>
class ptr_to_nullary_function : public CFunType<Result> {
    Result (*ptr)();
public:
    explicit ptr_to_nullary_function(Result (*x)()) : ptr(x) {}
    Result operator()() const { return ptr(); }
};

template <class Result>
inline ptr_to_nullary_function<Result>
ptr_to_fun(Result (*x)()) {
  return ptr_to_nullary_function<Result>(x);
}

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
inline Curryable2<ptr_to_binary_function<Arg1, Arg2, Result> >
ptr_to_fun(Result (*x)(Arg1, Arg2)) {
  return makeCurryable2( ptr_to_binary_function<Arg1, Arg2, Result>(x) );
}

template <class Arg1, class Arg2, class Arg3, class Result>
class ptr_to_ternary_function : public CFunType<Arg1, Arg2, Arg3, Result> {
    Result (*ptr)(Arg1, Arg2, Arg3);
public:
    explicit ptr_to_ternary_function(Result (*x)(Arg1, Arg2, Arg3)) : ptr(x) {}
    Result operator()(Arg1 x, Arg2 y, Arg3 z) const { return ptr(x,y,z); }
};

template <class Arg1, class Arg2, class Arg3, class Result>
inline Curryable3<ptr_to_ternary_function<Arg1, Arg2, Arg3, Result> >
ptr_to_fun(Result (*x)(Arg1, Arg2, Arg3)) {
  return makeCurryable3( ptr_to_ternary_function<Arg1,Arg2,Arg3,Result>(x) );
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
inline Curryable3<ptr_to_mem_binary_function<Arg1, Arg2, Arg3, Result> >
ptr_to_fun(Result (Arg1::*x)(Arg2,Arg3)) {
  return makeCurryable3(
            ptr_to_mem_binary_function<Arg1, Arg2, Arg3, Result>(x) );
}

template <class Arg1, class Arg2, class Arg3, class Result>
class ptr_to_const_mem_binary_function 
: public CFunType<Arg1*, Arg2, Arg3, Result> {
    Result (Arg1::*ptr)(Arg2,Arg3) const;
public:
    explicit ptr_to_const_mem_binary_function(
          Result (Arg1::*x)(Arg2,Arg3) const) : ptr(x) {}
    Result operator()(const Arg1* x, const Arg2& y, const Arg3& z) const 
       { return (x->*ptr)(y,z); }
};

template <class Arg1, class Arg2, class Arg3, class Result>
inline Curryable3<ptr_to_const_mem_binary_function<Arg1, Arg2, Arg3, Result> >
ptr_to_fun(Result (Arg1::*x)(Arg2,Arg3) const) {
  return makeCurryable3(
            ptr_to_const_mem_binary_function<Arg1, Arg2, Arg3, Result>(x) );
}

template <class Arg1, class Arg2, class Result>
class ptr_to_mem_unary_function : public CFunType<Arg1*, Arg2, Result> {
    Result (Arg1::*ptr)(Arg2);
public:
    explicit ptr_to_mem_unary_function(Result (Arg1::*x)(Arg2)) : ptr(x) {}
    Result operator()(Arg1* x, const Arg2& y) const { return (x->*ptr)(y); }
};

template <class Arg1, class Arg2, class Result>
inline Curryable2<ptr_to_mem_unary_function<Arg1, Arg2, Result> >
ptr_to_fun(Result (Arg1::*x)(Arg2)) {
  return makeCurryable2( ptr_to_mem_unary_function<Arg1, Arg2, Result>(x) );
}

template <class Arg1, class Arg2, class Result>
class ptr_to_const_mem_unary_function : public CFunType<Arg1*, Arg2, Result> {
    Result (Arg1::*ptr)(Arg2) const;
public:
    explicit ptr_to_const_mem_unary_function(Result (Arg1::*x)(Arg2) const) 
       : ptr(x) {}
    Result operator()(const Arg1* x, const Arg2& y) const 
       { return (x->*ptr)(y); }
};

template <class Arg1, class Arg2, class Result>
inline Curryable2<ptr_to_const_mem_unary_function<Arg1, Arg2, Result> >
ptr_to_fun(Result (Arg1::*x)(Arg2) const) {
  return makeCurryable2( 
    ptr_to_const_mem_unary_function<Arg1, Arg2, Result>(x) );
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

template <class Arg1, class Result>
class ptr_to_const_mem_nullary_function : public CFunType<Arg1*, Result> {
    Result (Arg1::*ptr)() const;
public:
    explicit ptr_to_const_mem_nullary_function(Result (Arg1::*x)() const) 
       : ptr(x) {}
    Result operator()(const Arg1* x) const { return (x->*ptr)(); }
};

template <class Arg1, class Result>
inline ptr_to_const_mem_nullary_function<Arg1, Result>
ptr_to_fun(Result (Arg1::*x)() const) {
  return ptr_to_const_mem_nullary_function<Arg1, Result>(x);
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
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Inc inc; 
FCPP_MAYBE_NAMESPACE_CLOSE

struct Always1 {
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return true; }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Always1 always1;
FCPP_MAYBE_NAMESPACE_CLOSE

struct Never1 {
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return false; }
};
FCPP_MAYBE_NAMESPACE_OPEN
FCPP_MAYBE_EXTERN Never1 never1;
FCPP_MAYBE_NAMESPACE_CLOSE

} // end namespace fcpp

#endif
