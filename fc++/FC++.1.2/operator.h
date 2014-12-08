//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
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
namespace {
MakePair _makePair;
Curryable2<MakePair> makePair(_makePair);
}

struct Min {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::less<T>()( x, y ) ? x : y;
   }
};
namespace {
Min _min;
Curryable2<Min> min(_min);
}

struct Max {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::less<T>()( x, y ) ? y : x;
   }
};
namespace {
Max _max;
Curryable2<Max> max(_max);
}

struct Plus {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::plus<T>()( x, y );
   }
};
namespace {
Plus _plus;
Curryable2<Plus> plus(_plus);
}

struct Minus {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::minus<T>()( x, y );
   }
};
namespace {
Minus _minus;
Curryable2<Minus> minus(_minus);
}

struct Multiplies {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::multiplies<T>()( x, y );
   }
};
namespace {
Multiplies _multiplies;
Curryable2<Multiplies> multiplies(_multiplies);
}

struct Divides {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::divides<T>()( x, y );
   }
};
namespace {
Divides _divides;
Curryable2<Divides> divides(_divides);
}

struct Modulus {
   template <class T, class U>
   struct Sig : public FunType<T,U,T> {};

   template <class T>
   T operator()( const T& x, const T& y ) const {
      return std::modulus<T>()( x, y );
   }
};
namespace {
Modulus _modulus;
Curryable2<Modulus> modulus(_modulus);
}

struct Negate {
   template <class T>
   struct Sig : public FunType<T,T> {};

   template <class T>
   T operator()( const T& x ) const {
      return std::negate<T>()( x );
   }
};
namespace {
Negate negate;
}

struct Equal {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::equal_to<T>()( x, y );
   }
};
namespace {
Equal _equal;
Curryable2<Equal> equal(_equal);
}

struct NotEqual {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::not_equal_to<T>()( x, y );
   }
};
namespace {
NotEqual _notEqual;
Curryable2<NotEqual> notEqual(_notEqual);
}

struct Greater {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::greater<T>()( x, y );
   }
};
namespace {
Greater _greater;
Curryable2<Greater> greater(_greater);
}

struct Less {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::less<T>()( x, y );
   }
};
namespace {
Less _less;
Curryable2<Less> less(_less);
}

struct GreaterEqual {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::greater_equal<T>()( x, y );
   }
};
namespace {
GreaterEqual _greaterEqual;
Curryable2<GreaterEqual> greaterEqual(_greaterEqual);
}

struct LessEqual {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::less_equal<T>()( x, y );
   }
};
namespace {
LessEqual _lessEqual;
Curryable2<LessEqual> lessEqual(_lessEqual);
}

struct LogicalAnd {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::logical_and<T>()( x, y );
   }
};
namespace {
LogicalAnd _logicalAnd;
Curryable2<LogicalAnd> logicalAnd(_logicalAnd);
}

struct LogicalOr {
   template <class Arg1, class Arg2>
   struct Sig : public FunType<Arg1,Arg2,bool> {};

   template <class T>
   bool operator()( const T&x, const T&y ) const {
      return std::logical_or<T>()( x, y );
   }
};
namespace {
LogicalOr _logicalOr;
Curryable2<LogicalOr> logicalOr(_logicalOr);
}

struct LogicalNot {
   template <class T>
   struct Sig : public FunType<T,bool> {};

   template <class T>
   bool operator()( const T&x ) const {
      return std::logical_not<T>()( x );
   }
};
namespace {
LogicalNot logicalNot;
}

// The other "higher-order" operators in STL are easy to do in our
// framework. For instance, instead of unary_negate (with all its
// complicated type info), we can just say compose1(logicalnot, f).


//////////////////////////////////////////////////////////////////////
// STL conversions
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
namespace {
Inc inc; 
}

struct Always1 {
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return true; }
};
namespace {
Always1 always1;
}

struct Never1 {
    template <class T>
    struct Sig : public FunType<T,bool> {};

    template <class T>
    bool operator()(const T&) const { return false; }
};
namespace {
Never1 never1;
}

} // end namespace fcpp

#endif
