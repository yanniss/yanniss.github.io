//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_CURRY_DOT_H
#define FCPP_CURRY_DOT_H

#include "signature.h"

namespace fcpp {

//////////////////////////////////////////////////////////////////////
// This file implements currying for functoids.  Included here are
//  - bindMofN    for currying the Mth of N arguments
//  - Const       for turning a value into a constant function
//  - curryN      curries the first k arguments of an N-arg functoid, 
//                where k is the number of arguments "curryN" was 
//                called with
//  - CurryableN  new way to curry with underscores (e.g. f(_,y,_); )
//
// For more info, see
//    http://www.cc.gatech.edu/~yannis/fc++/currying.html
//////////////////////////////////////////////////////////////////////

// Important to implementation of CurryableN classes
struct AutoCurryType {};
namespace {
AutoCurryType _;   // this is a legal identifier as fcpp::_
}

//////////////////////////////////////////////////////////////////////
// Const
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

//////////////////////////////////////////////////////////////////////
// Binders (through "...of2")
//////////////////////////////////////////////////////////////////////

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
namespace {
Bind1of1 bind1of1;
//Curryable2<Bind1of1> bind1of1(_bind1of1);
}

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
namespace {
Bind1of2 bind1of2;
//Curryable2<Bind1of2> bind1of2(_bind1of2);
}

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
namespace {
Bind2of2 bind2of2;
//Curryable2<Bind2of2> bind2of2(_bind2of2);
}

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
namespace {
Bind1and2of2 bind1and2of2;
//Curryable3<Bind1and2of2> bind1and2of2(_bind1and2of2);
}

//////////////////////////////////////////////////////////////////////
// Now that bindNof2 are defined, we can define Curryable2, which then
// some of the later binders can use.
//////////////////////////////////////////////////////////////////////

template <class R, class F, class X, class Y>
struct Curryable2Helper {
   R operator()( const F& f, const X& x, const Y& y ) const {
      return f(x,y); 
   }
};

template <class R, class F, class Y>
struct Curryable2Helper<R,F,AutoCurryType,Y> {
   R operator()( const F& f, const AutoCurryType& , const Y& y ) const {
      return binder2of2<F,Y>(f,y);
   }
};

template <class R, class F, class X>
struct Curryable2Helper<R,F,X,AutoCurryType> {
   R operator()( const F& f, const X& x, const AutoCurryType& ) const {
      return binder1of2<F,X>(f,x);
   }
};

template <class F>
class Curryable2 {
   F f;
public:
   Curryable2( const F& ff ) : f(ff) {}

   template <class X, class Y=void>
   struct Sig
   : public FunType<typename F::template Sig<X,Y>::Arg1Type,
                    typename F::template Sig<X,Y>::Arg2Type,
                    typename RT<F,X,Y>::ResultType> {};
   // Note: the line below is incorrect, as Fun2 uses ANY_TYPE, for example
   //: public FunType<X,Y,typename F::template Sig<X,Y>::ResultType> {};

   template <class X>
   struct Sig<X,void> : public FunType<X,binder1of2<F,X> > {};

   template <class Y>
   struct Sig<AutoCurryType,Y> 
   : public FunType<AutoCurryType,Y,binder2of2<F,Y> > {};

   template <class X>
   struct Sig<X,AutoCurryType> 
   : public FunType<X,AutoCurryType,binder1of2<F,X> > {};

   template <class X>
   Sig<X>::ResultType operator()( const X& x ) const {
      return binder1of2<F,X>(f,x);
   }
   template <class X, class Y>
   Sig<X,Y>::ResultType operator()( const X& x, const Y& y ) const {
      // need partial specialization, so defer to a class helper
      return Curryable2Helper<Sig<X,Y>::ResultType,F,X,Y>()(f,x,y);
   }
};

template <class F>
Curryable2<F> makeCurryable2( const F& f ) {
   return Curryable2<F>( f );
}

//////////////////////////////////////////////////////////////////////
// With Curryable2 out of the way, we can go back to the 3-arg binders.
//////////////////////////////////////////////////////////////////////

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
namespace {
Bind1and2and3of3 bind1and2and3of3; 
}

template <class Ternary, class Arg1, class Arg2>
class binder1and2of3 {
   Ternary f;
   Arg1 a1;
   Arg2 a2;
public:
   template <class Arg3>
   struct Sig 
   : public FunType<typename Ternary::Sig<Arg1,Arg2,Arg3>::Arg3Type,
                    typename Ternary::Sig<Arg1,Arg2,Arg3>::ResultType> {};

   binder1and2of3(const Ternary& w, const Arg1& x, const Arg2& y) : 
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
namespace {
Bind1and2of3 bind1and2of3;
//Curryable3<Bind1and2of3> bind1and2of3(_bind1and2of3);
}

template <class Ternary, class Arg2, class Arg3>
class binder2and3of3 {
   Ternary f;
   Arg2 a2;
   Arg3 a3;
public:
   template <class Arg1>
   struct Sig 
   : public FunType<typename Ternary::Sig<Arg1,Arg2,Arg3>::Arg1Type,
                    typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType> {};
                    // need RT above due to g++ bug on line below
                    // typename Ternary::Sig<Arg1,Arg2,Arg3>::ResultType> {};

   binder2and3of3(const Ternary& w, const Arg2& y, const Arg3& z) : 
     f(w), a2(y), a3(z) {}
   template <class Arg1>
   Sig<Arg1>::ResultType operator()(const Arg1& x) const { return f(x,a2,a3); }
};

struct Bind2and3of3 {
   template <class Ternary, class A2, class A3>
   struct Sig 
   : public FunType<Ternary,A2,A3,binder2and3of3<Ternary,A2,A3> > {};

   template <class Ternary, class A2, class A3>
   binder2and3of3<Ternary,A2,A3> 
   operator()( const Ternary& f, const A2& a2, const A3& a3 ) const {
      return binder2and3of3<Ternary,A2,A3>(f,a2,a3);
   }
};
namespace {
Bind2and3of3 bind2and3of3;
//Curryable3<Bind2and3of3> bind2and3of3(_bind2and3of3);
}

template <class Ternary, class Arg1, class Arg3>
class binder1and3of3 {
   Ternary f;
   Arg1 a1;
   Arg3 a3;
public:
   template <class Arg2>
   struct Sig 
   : public FunType<typename Ternary::Sig<Arg1,Arg2,Arg3>::Arg2Type,
                    typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType> {};
                    // need RT above due to g++ bug on line below
                    // typename Ternary::Sig<Arg1,Arg2,Arg3>::ResultType> {};

   binder1and3of3(const Ternary& w, const Arg1& x, const Arg3& z) : 
     f(w), a1(x), a3(z) {}
   template <class Arg2>
   Sig<Arg2>::ResultType operator()(const Arg2& y) const { return f(a1,y,a3); }
};

struct Bind1and3of3 {
   template <class Ternary, class A1, class A3>
   struct Sig 
   : public FunType<Ternary,A1,A3,binder1and3of3<Ternary,A1,A3> > {};

   template <class Ternary, class A1, class A3>
   binder1and3of3<Ternary,A1,A3> 
   operator()( const Ternary& f, const A1& a1, const A3& a3 ) const {
      return binder1and3of3<Ternary,A1,A3>(f,a1,a3);
   }
};
namespace {
Bind1and3of3 bind1and3of3;
//Curryable3<Bind1and3of3> bind1and3of3(_bind1and3of3);
}

template <class Ternary, class Arg1>
class binder1of3 {
   Ternary f;
   Arg1 x;
public:
   binder1of3( const Ternary& a, const Arg1& b ) : f(a), x(b) {}

   template <class Arg2, class Arg3>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg2Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg3Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::ResultType> {};

   template <class Arg2, class Arg3>
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
   operator()( const Arg2& y, const Arg3& z ) const {
      return f(x,y,z);
   }
};

// FIX THIS: "BindNof3"s should really be "Curryable2"s, so that stuff
// like ternary(_,_,3)(1)(2) works... ugh.
//
// Yuck.  What should happen is that "bind1of3" should have type
// "Curryable2<Bind1of3>", but in a number of places in the library, I
// bet I have presumed that the return type of bind1of3(...) was
// going to be a "binder1of3" and then implemented based on that
// (soon-to-be-faulty) presumption.  Argh.  I should really make the
// "Binder" classes hidden inside another namespace to prevent idiots
// like me from assuming things--they are implementation details of the
// "bindNof3" interface, and I shouldn't know about them.
//
// Darn.
//
// Ok, so at some point, I need to go back and find all the
// "implementation details" and hide them in a nested namespace named
// "implementation" or something.

struct Bind1of3 {
   template <class Ternary, class Arg1>
   struct Sig 
   : public FunType<Ternary,Arg1,Curryable2<binder1of3<Ternary,Arg1> > > {};

   template <class Ternary, class Arg1>
   Sig<Ternary,Arg1>::ResultType
   operator()( const Ternary& f, const Arg1& x ) const {
      return makeCurryable2( binder1of3<Ternary,Arg1>(f,x) );
   }
};
namespace {
Bind1of3 _bind1of3;
Curryable2<Bind1of3> bind1of3(_bind1of3);
}

template <class Ternary, class Arg2>
class binder2of3 {
   Ternary f;
   Arg2 x;
public:
   binder2of3( const Ternary& a, const Arg2& b ) : f(a), x(b) {}

   template <class Arg1, class Arg3>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg1Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg3Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::ResultType> {};

   template <class Arg1, class Arg3>
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
   operator()( const Arg1& y, const Arg3& z ) const {
      return f(y,x,z);
   }
};

struct Bind2of3 {
   template <class Ternary, class Arg2>
   struct Sig 
   : public FunType<Ternary,Arg2,Curryable2<binder2of3<Ternary,Arg2> > > {};

   template <class Ternary, class Arg2>
   Sig<Ternary,Arg2>::ResultType
   operator()( const Ternary& f, const Arg2& x ) const {
      return makeCurryable2( binder2of3<Ternary,Arg2>(f,x) );
   }
};
namespace {
Bind2of3 _bind2of3;
Curryable2<Bind2of3> bind2of3(_bind2of3);
}

template <class Ternary, class Arg3>
class binder3of3 {
   Ternary f;
   Arg3 x;
public:
   binder3of3( const Ternary& a, const Arg3& b ) : f(a), x(b) {}

   template <class Arg1, class Arg2>
   struct Sig 
   : public FunType<typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg1Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::Arg2Type,
                    typename Ternary::template Sig<Arg1,Arg2,Arg3>::ResultType> {};

   template <class Arg1, class Arg2>
   typename RT<Ternary,Arg1,Arg2,Arg3>::ResultType
   operator()( const Arg1& y, const Arg2& z ) const {
      return f(y,z,x);
   }
};

struct Bind3of3 {
   template <class Ternary, class Arg3>
   struct Sig 
   : public FunType<Ternary,Arg3,Curryable2<binder3of3<Ternary,Arg3> > > {};

   template <class Ternary, class Arg3>
   Sig<Ternary,Arg3>::ResultType
   operator()( const Ternary& f, const Arg3& x ) const {
      return makeCurryable2( binder3of3<Ternary,Arg3>(f,x) );
   }
};
namespace {
Bind3of3 _bind3of3;
Curryable2<Bind3of3> bind3of3(_bind3of3);
}

//////////////////////////////////////////////////////////////////////
// "curry" versions. Now that we have auto-currying, you rarely need to
// call curry() explicitly, unless you are trying to get lazy evaulation
// in examples like
//    curry2( map, f, l ).
// It used to be the case that you might also do
//    curry2( map, f ),
// but nowadays you can say the same thing with just
//    map(f).
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
namespace {
Curry3 curry3; 
}

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
namespace {
Curry2 curry2;
}

struct Curry1 {
  template <class Unary, class A1>
  struct Sig : public FunType<Unary, A1, binder1of1<Unary,A1> > {};  

  template <class Unary, class A1>
  Sig<Unary, A1>::ResultType
  operator()( const Unary& f, const A1& a1 ) const {
    return binder1of1<Unary,A1>(f,a1);
  }
};
namespace {
Curry1 curry1, curry; // "curry" is the same as "curry1"
}

//////////////////////////////////////////////////////////////////////
// Finally, Curryable3 (what a huge beast)
//////////////////////////////////////////////////////////////////////

template <class R, class F, class X, class Y, class Z>
struct Curryable3Helper {
   R operator()( const F& f, const X& x, const Y& y, const Z& z ) const {
      return f(x,y,z); 
   }
};

template <class R, class F, class X>
struct Curryable3Helper<R,F,X,AutoCurryType,AutoCurryType> {
   R operator()( const F& f, const X& x, const AutoCurryType&, 
                 const AutoCurryType& ) const {
      return bind1of3(f,x);
   }
};

template <class R, class F, class Y>
struct Curryable3Helper<R,F,AutoCurryType,Y,AutoCurryType> {
   R operator()( const F& f, const AutoCurryType&, const Y& y, 
                 const AutoCurryType& ) const {
      return bind2of3(f,y);
   }
};

template <class R, class F, class Z>
struct Curryable3Helper<R,F,AutoCurryType,AutoCurryType,Z> {
   R operator()( const F& f, const AutoCurryType&, const AutoCurryType&,
                 const Z& z ) const {
      return bind3of3(f,z);
   }
};

template <class R, class F, class Y, class Z>
struct Curryable3Helper<R,F,AutoCurryType,Y,Z> {
   R operator()( const F& f, const AutoCurryType&, const Y& y, 
                 const Z& z ) const {
      return binder2and3of3<F,Y,Z>(f,y,z);
   }
};

template <class R, class F, class X, class Z>
struct Curryable3Helper<R,F,X,AutoCurryType,Z> {
   R operator()( const F& f, const X& x, const AutoCurryType&, 
                 const Z& z ) const {
      return binder1and3of3<F,X,Z>(f,x,z);
   }
};

template <class R, class F, class X, class Y>
struct Curryable3Helper<R,F,X,Y,AutoCurryType> {
   R operator()( const F& f, const X& x, const Y& y, 
                 const AutoCurryType& ) const {
      return binder1and2of3<F,X,Y>(f,x,y);
   }
};

template <class R, class F, class X, class Y>
struct Curryable3Helper2 {
   R operator()( const F& f, const X& x, const Y& y ) const {
      return binder1and2of3<F,X,Y>(f,x,y);
   }
};

template <class R, class F, class Y>
struct Curryable3Helper2<R,F,AutoCurryType,Y> {
   R operator()( const F& f, const AutoCurryType&, const Y& y ) const {
      return makeCurryable2(binder2of3<F,Y>(f,y));
   }
};

template <class R, class F, class X>
struct Curryable3Helper2<R,F,X,AutoCurryType> {
   R operator()( const F& f, const X& x, const AutoCurryType& ) const {
      return makeCurryable2(binder1of3<F,X>(f,x));
   }
};

template <class F>
class Curryable3 {
   F f;
public:
   Curryable3( const F& ff ) : f(ff) {}

   template <class X, class Y=void, class Z=void>
   struct Sig
   : public FunType<typename F::template Sig<X,Y,Z>::Arg1Type,
                    typename F::template Sig<X,Y,Z>::Arg2Type,
                    typename F::template Sig<X,Y,Z>::Arg3Type,
                    typename RT<F,X,Y,Z>::ResultType> {};

   template <class X,class Y>
   struct Sig<X,Y,void> : public FunType<X,Y,binder1and2of3<F,X,Y> > {};

   template <class X>
   struct Sig<X,AutoCurryType,void> 
   : public FunType<X,AutoCurryType,Curryable2<binder1of3<F,X> > > {};

   template <class Y>
   struct Sig<AutoCurryType,Y,void> 
   : public FunType<AutoCurryType,Y,Curryable2<binder2of3<F,Y> > > {};

   template <class X>
   struct Sig<X,void,void> : public FunType<X,Curryable2<binder1of3<F,X> > > {};

   template <class X>
   struct Sig<X,AutoCurryType,AutoCurryType> 
   : public FunType<X,AutoCurryType,AutoCurryType,
                    Curryable2<binder1of3<F,X> > > {};

   template <class Y>
   struct Sig<AutoCurryType,Y,AutoCurryType> 
   : public FunType<AutoCurryType,Y,AutoCurryType,
                    Curryable2<binder2of3<F,Y> > > {};

   template <class Z>
   struct Sig<AutoCurryType,AutoCurryType,Z> 
   : public FunType<AutoCurryType,AutoCurryType,Z,
                    Curryable2<binder3of3<F,Z> > > {};

   template <class X,class Z>
   struct Sig<X,AutoCurryType,Z> 
   : public FunType<X,AutoCurryType,Z,binder1and3of3<F,X,Z> > {};

   template <class Y,class Z>
   struct Sig<AutoCurryType,Y,Z> 
   : public FunType<AutoCurryType,Y,Z,binder2and3of3<F,Y,Z> > {};

   template <class X,class Y>
   struct Sig<X,Y,AutoCurryType> 
   : public FunType<X,Y,AutoCurryType,binder1and2of3<F,X,Y> > {};


   template <class X,class Y>
   Sig<X,Y>::ResultType operator()( const X& x, const Y& y ) const {
      // need partial specialization, so defer to a class helper
      return Curryable3Helper2<Sig<X,Y>::ResultType,F,X,Y>()(f,x,y);
   }
   template <class X>
   Sig<X>::ResultType operator()( const X& x ) const {
      return makeCurryable2(binder1of3<F,X>(f,x));
   }
   template <class X, class Y, class Z>
   Sig<X,Y,Z>::ResultType 
   operator()( const X& x, const Y& y, const Z& z ) const {
      // need partial specialization, so defer to a class helper
      return Curryable3Helper<Sig<X,Y,Z>::ResultType,F,X,Y,Z>()(f,x,y,z);
   }
};

template <class F>
Curryable3<F> makeCurryable3( const F& f ) {
   return Curryable3<F>( f );
}

} // end namespace fcpp

#endif
