//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef CURRY_DOT_H
#define CURRY_DOT_H

#include "signature.h"

//////////////////////////////////////////////////////////////////////
// This file implements currying for functoids.  Included here are
//  - bindMofN    for currying the Mth of N arguments
//  - Const       for turning a value into a constant function
//  - curryN      curries the first k arguments of an N-arg functoid, 
//                where k is the number of arguments "curryN" was 
//                called with
//
// Examples:
//   If f() has signature (T,U)->V and g() has signature (W,X,Y)->Z then
//      bind1of3( g, a_W_obj )    has signature      (X,Y)->Z
//      bind3of3( g, a_Y_obj )    has signature      (W,X)->Z
//      bind1of2( f, a_T_obj )    has signature      (U)->V
//      bind2of2( f, a_U_obj )    has signature      (T)->V
//      bind1and2of2( f, a_t_obj, a_U_obj )  has signature   ()->V
//
//   So, for instance,
//      bind2of2( And(), true )
//   yields the identity function for booleans, and
//      bind1and2of2( Plus(), 3, 4 )
//   yields the same thing as
//      Const()( 7 )
//   which is a function with arity zero that always returns 7.
//
//  REVIEW: Not all combinations are supported.
//////////////////////////////////////////////////////////////////////

template <class T>
struct ConstHelper : public CFunType<T> {
   T x;
public:
   ConstHelper( const T& a ) : x(a) {}
   T operator()() const { return x; }
};

struct Const {
   template <class T>
   struct Sig : public FunType<T,ConstHelper<T> > {};

   template <class T>
   ConstHelper<T> operator()( const T& x ) const {
      return ConstHelper<T>(x);
   }
};

template <class Unary, class Arg>
class binder1of1 
: public CFunType<typename RT<Unary,Arg>::ResultType> {
   Unary f;
   Arg a;
public:
   binder1of1( const Unary& x, const Arg& y ) : f(x), a(y) {}
   ResultType operator()() const { return f(a); }
};

struct Bind1of1 {
   template <class Unary, class Arg>
   struct Sig : public FunType< Unary, Arg, binder1of1<Unary,Arg> > {};
   
   template <class Unary, class Arg>
inline
   binder1of1<Unary,Arg> operator()( const Unary& f, const Arg& a ) const {
      return binder1of1<Unary,Arg>(f,a);
   }
};
namespace { Bind1of1 bind1of1; }

template <class Binary, class Arg1>
class binder1of2 {
   Binary f;
   Arg1 x;
public:
   binder1of2( const Binary& a, const Arg1& b ) : f(a), x(b) {}

   template <class Arg2>
   struct Sig 
   : public FunType<typename Binary::Sig<Arg1,Arg2>::Arg2Type,
                    typename Binary::Sig<Arg1,Arg2>::ResultType> {};

   template <class Arg2>
   typename Binary::Sig<Arg1,Arg2>::ResultType
   operator()( const Arg2& y ) const {
      return f(x,y);
   }
};

struct Bind1of2 {
   template <class Binary, class Arg1>
   struct Sig : public FunType<Binary,Arg1,binder1of2<Binary,Arg1> > {};

   template <class Binary, class Arg1>
   binder1of2<Binary,Arg1> 
   operator()( const Binary& f, const Arg1& x ) const {
      return binder1of2<Binary,Arg1>(f,x);
   }
};
namespace { Bind1of2 bind1of2; }

template <class Binary, class Arg2>
class binder2of2 {
   Binary f;
   Arg2 y;
public:
   binder2of2( const Binary& a, const Arg2& b ) : f(a), y(b) {}

   template <class Arg1>
   struct Sig 
   : public FunType<typename Binary::template Sig<Arg1,Arg2>::Arg1Type,
                    typename Binary::template Sig<Arg1,Arg2>::ResultType> {};

   template <class Arg1>
   typename Binary::template Sig<Arg1,Arg2>::ResultType
   operator()( const Arg1& x ) const {
      return f(x,y);
   }
};

struct Bind2of2 {
   template <class Binary, class Arg2>
   struct Sig : public FunType<Binary,Arg2,binder2of2<Binary,Arg2> > {};

   template <class Binary, class Arg2>
   binder2of2<Binary,Arg2> 
   operator()( const Binary& f, const Arg2& y ) const {
      return binder2of2<Binary,Arg2>(f,y);
   }
};
namespace { Bind2of2 bind2of2; }

template <class Binary, class Arg1, class Arg2>
class binder1and2of2 
: public CFunType<typename RT<Binary,Arg1,Arg2>::ResultType > {
   Binary f;
   Arg1 a1;
   Arg2 a2;
public:
   binder1and2of2( const Binary& x, const Arg1& y, const Arg2& z ) 
   : f(x), a1(y), a2(z) {}
   ResultType operator()() const { return f(a1,a2); }
};

struct Bind1and2of2 {
   template <class Binary, class Arg1, class Arg2>
   struct Sig 
   : public FunType< Binary, Arg1, Arg2, binder1and2of2<Binary,Arg1,Arg2> > {};

   template <class Binary, class Arg1, class Arg2>
   binder1and2of2<Binary,Arg1,Arg2> 
   operator()( const Binary& f, const Arg1& a1, const Arg2& a2 ) const {
      return binder1and2of2<Binary,Arg1,Arg2>(f,a1,a2);
   }
};
namespace { Bind1and2of2 bind1and2of2; }

template <class Ternary, class A1, class A2, class A3>
class binder1and2and3of3
: public CFunType<typename RT<Ternary,A1,A2,A3>::ResultType> {
   Ternary f;
   A1 a1;
   A2 a2;
   A3 a3;
public:
   binder1and2and3of3( const Ternary& w, const A1& x, const A2& y, const A3& z )
   : f(w), a1(x), a2(y), a3(z) {}
   ResultType operator()() const { return f(a1,a2,a3); }
};

struct Bind1and2and3of3 {
   template <class Ternary, class A1, class A2, class A3>
   struct Sig 
   : public FunType<Ternary,A1,A2,A3,
                     binder1and2and3of3<Ternary,A1,A2,A3> > {};

   template <class Ternary, class A1, class A2, class A3>
   binder1and2and3of3<Ternary,A1,A2,A3> 
   operator()( const Ternary& f, const A1& a1, 
               const A2& a2, const A3& a3 ) const {
      return binder1and2and3of3<Ternary,A1,A2,A3>(f,a1,a2,a3);
   }
};
namespace { Bind1and2and3of3 bind1and2and3of3; }

template <class Ternary, class A1, class A2>
class binder1and2of3 {
   Ternary f;
   A1 a1;
   A2 a2;
public:
   template <class Arg3>
   struct Sig 
   : public FunType<typename Ternary::Sig<Arg1,Arg2,Arg3>::Arg3Type,
                    typename Ternary::Sig<Arg1,Arg2,Arg3>::ResultType> {};

   binder1and2of3(const Ternary& w, const A1& x, const A2& y) : 
     f(w), a1(x), a2(y) {}
   template <class Arg3>
   Sig<Arg3>::ResultType operator()(const Arg3& z) const { return f(a1,a2,z); }
};

struct Bind1and2of3 {
   template <class Ternary, class A1, class A2>
   struct Sig 
   : public FunType<Ternary,A1,A2,binder1and2of3<Ternary,A1,A2> > {};

   template <class Ternary, class A1, class A2>
   binder1and2of3<Ternary,A1,A2> 
   operator()( const Ternary& f, const A1& a1, const A2& a2 ) const {
      return binder1and2of3<Ternary,A1,A2>(f,a1,a2);
   }
};
namespace { Bind1and2of3 bind1and2of3; }


template <class Ternary, class Arg1>
class binder1of3 {
   Ternary f;
   Arg1 x;
public:
   binder1of3( const Ternary& a, const Arg1& b ) : f(a), x(b) {}

   template <class Arg2, class Arg3>
   struct Sig 
   : public FunType<typename RT<Ternary,Arg1,Arg2,Arg3>::Arg2Type,
		    typename RT<Ternary,Arg1,Arg2,Arg3>::Arg3Type,
                    typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType> {};

   template <class Arg2, class Arg3>
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
   operator()( const Arg2& y, const Arg3& z ) const {
      return f(x,y,z);
   }
};

struct Bind1of3 {
   template <class Ternary, class Arg1>
   struct Sig 
   : public FunType<Ternary,Arg1,binder1of3<Ternary,Arg1> > {};

   template <class Ternary, class Arg1>
   binder1of3<Ternary,Arg1>
   operator()( const Ternary& f, const Arg1& x ) const {
      return binder1of3<Ternary,Arg1>(f,x);
   }
};
namespace { Bind1of3 bind1of3; }

template <class Ternary, class Arg2>
class binder2of3 {
   Ternary f;
   Arg2 x;
public:
   binder2of3( const Ternary& a, const Arg2& b ) : f(a), x(b) {}

   template <class Arg1, class Arg3>
   struct Sig 
   : public FunType<typename RT<Ternary,Arg1,Arg2,Arg3>::Arg1Type,
		    typename RT<Ternary,Arg1,Arg2,Arg3>::Arg3Type,
                    typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType> {};

   template <class Arg1, class Arg3>
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
   operator()( const Arg1& y, const Arg3& z ) const {
      return f(y,x,z);
   }
};

struct Bind2of3 {
   template <class Ternary, class Arg2>
   struct Sig 
   : public FunType<Ternary,Arg2,binder2of3<Ternary,Arg2> > {};

   template <class Ternary, class Arg2>
   binder2of3<Ternary,Arg2>
   operator()( const Ternary& f, const Arg2& x ) const {
      return binder2of3<Ternary,Arg2>(f,x);
   }
};
namespace { Bind2of3 bind2of3; }

template <class Ternary, class Arg3>
class binder3of3 {
   Ternary f;
   Arg3 x;
public:
   binder3of3( const Ternary& a, const Arg3& b ) : f(a), x(b) {}

   template <class Arg1, class Arg2>
   struct Sig 
   : public FunType<typename RT<Ternary,Arg1,Arg2,Arg3>::Arg1Type,
		    typename RT<Ternary,Arg1,Arg2,Arg3>::Arg2Type,
                    typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType> {};

   template <class Arg1, class Arg2>
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
   operator()( const Arg1& y, const Arg2& z ) const {
      return f(y,z,x);
   }
};

struct Bind3of3 {
   template <class Ternary, class Arg3>
   struct Sig 
   : public FunType<Ternary,Arg3,binder3of3<Ternary,Arg3> > {};

   template <class Ternary, class Arg3>
   binder3of3<Ternary,Arg3>
   operator()( const Ternary& f, const Arg3& x ) const {
      return binder3of3<Ternary,Arg3>(f,x);
   }
};
namespace { Bind3of3 bind3of3; }


//////////////////////////////////////////////////////////////////////
// "curry" versions. Note that conceptually curryN has N different
// signatures that it supports. This is done with template 
// specialization (on the Sigs).
//////////////////////////////////////////////////////////////////////

struct Curry3 {
  template <class Ternary, class A1, class A2 = void, class A3 = void>
  struct Sig 
    : public FunType<Ternary, A1, A2, A3, 
                     binder1and2and3of3<Ternary,A1,A2,A3> > {};

  template <class Ternary, class A1, class A2>
  struct Sig<Ternary, A1, A2, void> 
    : public FunType<Ternary, A1, A2, binder1and2of3<Ternary,A1,A2> > {};  

  template <class Ternary, class A1>
  struct Sig<Ternary, A1, void, void> 
    : public FunType<Ternary, A1, binder1of3<Ternary,A1> > {};  


  template <class Ternary, class A1, class A2, class A3>
  Sig<Ternary, A1, A2, A3>::ResultType
  operator()( const Ternary& f, const A1& a1, const A2& a2, 
	      const A3& a3 ) const {
    return binder1and2and3of3<Ternary,A1,A2,A3>(f,a1,a2,a3);
  }

  template <class Ternary, class A1, class A2>
  Sig<Ternary, A1, A2>::ResultType
  operator()( const Ternary& f, const A1& a1, const A2& a2 ) const {
    return binder1and2of3<Ternary,A1,A2>(f,a1,a2);
  }

  template <class Ternary, class A1>
  Sig<Ternary, A1>::ResultType
  operator()( const Ternary& f, const A1& a1 ) const {
    return binder1of3<Ternary,A1>(f,a1);
  }
};
namespace { Curry3 curry3; }


struct Curry2 {
  template <class Binary, class A1, class A2 = void>
  struct Sig 
    : public FunType<Binary, A1, A2, binder1and2of2<Binary,A1,A2> > {};

  template <class Binary, class A1>
  struct Sig<Binary, A1, void> 
    : public FunType<Binary, A1, binder1of2<Binary,A1> > {};  


  template <class Binary, class A1, class A2>
  Sig<Binary, A1, A2>::ResultType
  operator()( const Binary& f, const A1& a1, const A2& a2 ) const {
    return binder1and2of2<Binary,A1,A2>(f,a1,a2);
  }

  template <class Binary, class A1>
  Sig<Binary, A1>::ResultType
  operator()( const Binary& f, const A1& a1 ) const {
    return binder1of2<Binary,A1>(f,a1);
  }
};
namespace { Curry2 curry2; }


struct Curry1 {
  template <class Unary, class A1>
  struct Sig : public FunType<Unary, A1, binder1of1<Unary,A1> > {};  

  template <class Unary, class A1>
  Sig<Unary, A1>::ResultType
  operator()( const Unary& f, const A1& a1 ) const {
    return binder1of1<Unary,A1>(f,a1);
  }
};
namespace { Curry1 curry1, curry; }
// "curry" is the same as "curry1"

#endif
