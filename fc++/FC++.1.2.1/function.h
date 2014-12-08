//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_FUNCTION_DOT_H
#define FCPP_FUNCTION_DOT_H

//////////////////////////////////////////////////////////////////////////
// Here is where FunN and FunNImpl classes are declared.  Also here are
//  - makeFunN    turn a monomorphic direct functoid into an indirect 
//                functoid
//  - convertN    for implicit conversions (subtype polymorphism)
//  - explicit_convertN   like convertN, but uses casts (non-implicit)
//////////////////////////////////////////////////////////////////////////

// FIX THIS: This file has gotten a little messy.  It would be nice to
// try to clean it up somehow (though there are a number of
// implementation details which will undoubtedly defy simplification).

// FIX THIS: There is no Fun3 class.

// Note: it looks like the Genx classes could be local to makeFunx()

#include "ref_count.h"
#include "operator.h"

#ifdef FCPP_USE_GARBAGE_COLLECTION
#include "gc_cpp.h"
#endif

// g++ chokes for some reason
#ifdef FCPP_USE_NAMESPACE
namespace fcpp {
#endif

// ANY_TYPE is the type to use when we don't care about the
// instantiation of a template. This usually happens when we are
// reading the signature of a monomorphic function as if it were
// polymorphic.
class ANY_TYPE {};


template <class Result>
struct Fun0Impl;

template <class Result>
struct Fun0;

template <class Rd, class Rs>   // result of dest, result of src
Fun0Impl<Rd>* convert0( const Ref<Fun0Impl<Rs> >& f );

template <class Rd, class DF>
struct Fun0Constructor;

template <class Result>
class Fun0 : public CFunType<Result> {
#ifdef FCPP_USE_GARBAGE_COLLECTION
   typedef Fun0Impl<Result>* RefImpl;
#else
   typedef Ref<Fun0Impl<Result> > RefImpl;
#endif
   RefImpl ref;
   template <class T> friend class Fun0; 
   template <class Rd, class Rs>
   friend Fun0<Rd> explicit_convert0( const Fun0<Rs>& f );

   template <class Rd, class DF>
   friend struct Fun0Constructor;
public:
   typedef Fun0Impl<Result>* Impl;
   Fun0( int, Impl i ) : ref(i) {}

   Result operator()() const { return ref->operator()(); }

   template <class DF>
   Fun0( const DF& );   // direct functoid (or subtype polymorphism)
};

template <class Result>
struct Fun0Impl : public CFunType<Result> 
#ifdef FCPP_USE_GARBAGE_COLLECTION
#  ifndef FCPP_LEAK_MEMORY
#    ifdef FCPP_UNSAFE_GC
         , public gc
#    else
         , public gc_cleanup
#    endif
#  endif
#endif
{
   virtual Result operator()() const =0;
   virtual ~Fun0Impl() {}
};

template <class Rd, class Rs>
class Fun0Converter : public Fun0Impl<Rd> {
   typedef Ref<Fun0Impl<Rs> > MyFun;
   MyFun f;
public:
   Fun0Converter( const MyFun& g ) : f(g) {}
   Rd operator()() const {
      return f->operator()();
   }
};

template <class Rd, class Rs>
Fun0Impl<Rd>* convert0( const Ref<Fun0Impl<Rs> >& f ) {
   return new Fun0Converter<Rd,Rs>( f );
}

template <class Rd, class Rs>
class Fun0ExplicitConverter : public Fun0Impl<Rd> {
   typedef Ref<Fun0Impl<Rs> > MyFun;
   MyFun f;
public:
   Fun0ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()() const {
      return static_cast<Rd>( f->operator()() );
   }
};

template <class Rd, class Rs>
Fun0<Rd> explicit_convert0( const Fun0<Rs>& f ) {
   return Fun0<Rd>( 1, new Fun0ExplicitConverter<Rd,Rs>( f.ref ) );
}

#ifdef FCPP_INSTRUMENT_GEN0S
int number_of_gen0s_alive=0;
#endif

template <class Gen>
class Gen0 : public Fun0Impl<typename Gen::Sig::ResultType> {
   Gen g;
public:
   Gen0( Gen x ) : g(x) {
#ifdef FCPP_INSTRUMENT_GEN0S
number_of_gen0s_alive++;
#endif
}
   typename Gen::Sig::ResultType operator()() const { return g(); }
#ifdef FCPP_INSTRUMENT_GEN0S
~Gen0(){number_of_gen0s_alive--; }
#endif
};
   
template <class Gen>
Fun0<typename Gen::Sig::ResultType> makeFun0( const Gen& g ) {
   return Fun0<typename Gen::Sig::ResultType>(1,new Gen0<Gen>(g));
}

template <class Nullary>
Gen0<Nullary>* makeFun0Ref( const Nullary& g ) {
   return new Gen0<Nullary>(g);
}

// Note: conversion-from-direct-functoid and subtype-polymorphism create
// ambiguity in the case when we separate them into two constructors, so
// we just have one constructor and use partial specialization in this
// class to differentiate the two.
template <class Rd, class DF>
struct Fun0Constructor {
   static Fun0Impl<Rd>* make( const DF& df ) {
      return makeFun0Ref( ::fcpp::monomorphize0<Rd>(df) );
   }
};
template <class Rd, class Rs>
struct Fun0Constructor<Rd,Fun0<Rs> > {
   static Fun0Impl<Rd>* make( const Fun0<Rs>& f ) {
      return convert0<Rd>(f.ref);
   }
};

template <class Result>
template <class DF>
Fun0<Result>::Fun0( const DF& f ) 
: ref( Fun0Constructor<Result,DF>::make(f) ) {}

//////////////////////////////////////////////////////////////////////

template <class Arg1, class Result>
struct Fun1Impl;

template <class A1d, class Rd, class A1s, class Rs> 
Fun1Impl<A1d,Rd>* convert1( const Ref<Fun1Impl<A1s,Rs> >& f );

template <class Ad, class Rd, class DF>
struct Fun1Constructor;

template <class Arg1, class Result>
class Fun1 : public CFunType<Arg1,Result> {
   typedef Ref<Fun1Impl<Arg1,Result> > RefImpl;
   RefImpl ref;
   template <class A, class B> friend class Fun1;
   
   template <class Ad, class Rd, class DF>
   friend struct Fun1Constructor;

   template <class A1d, class Rd, class A1s, class Rs> 
   friend Fun1<A1d,Rd> explicit_convert1( const Fun1<A1s,Rs>& f );
public:
   typedef Fun1Impl<Arg1,Result>* Impl;

   // int is dummy arg to differentiate from template constructor
   Fun1( int, Impl i ) : ref(i) {}
   Result operator()( const Arg1& x ) const { return ref->operator()(x); }

   template <class DF>  // direct functoid (or subtype polymorphism)
   Fun1( const DF& df );
};

template <class Arg1, class Result>
struct Fun1Impl : public CFunType<Arg1,Result> {
   virtual Result operator()( const Arg1& ) const =0;
   virtual ~Fun1Impl() {}
};

template <class A1d, class Rd, class A1s, class Rs> 
class Fun1Converter : public Fun1Impl<A1d,Rd> {
   typedef Ref<Fun1Impl<A1s,Rs> > MyFun;
   MyFun f;
public:
   Fun1Converter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x ) const {
      return f->operator()( x );
   }
};

template <class A1d, class Rd, class A1s, class Rs> 
Fun1Impl<A1d,Rd>* convert1( const Ref<Fun1Impl<A1s,Rs> >& f ) {
   return new Fun1Converter<A1d,Rd,A1s,Rs>( f );
}

template <class A1d, class Rd, class A1s, class Rs> 
class Fun1ExplicitConverter : public Fun1Impl<A1d,Rd> {
   typedef Ref<Fun1Impl<A1s,Rs> > MyFun;
   MyFun f;
public:
   Fun1ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x ) const {
      return static_cast<Rd>( f->operator()(  static_cast<A1s>(x)  ) );
   }
};

template <class A1d, class Rd, class A1s, class Rs> 
Fun1<A1d,Rd> explicit_convert1( const Fun1<A1s,Rs>& f ) {
   return Fun1<A1d,Rd>( 1, new Fun1ExplicitConverter<A1d,Rd,A1s,Rs>(f.ref) );
}


template <class Gen>
class Gen1 : public Fun1Impl<typename Gen::Sig<ANY_TYPE>::Arg1Type, 
                             typename Gen::Sig<ANY_TYPE>::ResultType> {
   Gen g;
public:
   Gen1( Gen x ) : g(x) {}
   typename Gen::Sig<ANY_TYPE>::ResultType 
   operator()( const typename Gen::Sig<ANY_TYPE>::Arg1Type& x ) const { 
      return g(x); 
   }
};
   

template <class Unary>
Fun1<typename Unary::Sig<ANY_TYPE>::Arg1Type, 
     typename Unary::Sig<ANY_TYPE>::ResultType> 
makeFun1( const Unary& g ) {
   return 
     Fun1<typename Unary::Sig<ANY_TYPE>::Arg1Type,
          typename Unary::Sig<ANY_TYPE>::ResultType> 
        (1,new Gen1<Unary>(g));
}

template <class Unary>
Gen1<Unary>* makeFun1Ref( const Unary& g ) {
   return new Gen1<Unary>(g);
}

// Note: conversion-from-direct-functoid and subtype-polymorphism create
// ambiguity in the case when we separate them into two constructors, so
// we just have one constructor and use partial specialization in this
// class to differentiate the two.
template <class Ad, class Rd, class DF>
struct Fun1Constructor {
   static Fun1Impl<Ad,Rd>* make( const DF& df ) {
      return makeFun1Ref( ::fcpp::monomorphize1<Ad,Rd>(df) );
   }
};
template <class Ad, class Rd, class As, class Rs>
struct Fun1Constructor<Ad,Rd,Fun1<As,Rs> > {
   static Fun1Impl<Ad,Rd>* make( const Fun1<As,Rs>& f ) {
      return convert1<Ad,Rd>(f.ref);
   }
};

template <class Arg1, class Result>
template <class DF>  // direct functoid
Fun1<Arg1,Result>::Fun1( const DF& df )
: ref( Fun1Constructor<Arg1,Result,DF>::make(df) ) {}

//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Result>
struct Fun2Impl;

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
Fun2Impl<A1d,A2d,Rd>* convert2( const Ref<Fun2Impl<A1s,A2s,Rs> >& f );

template <class A1d, class A2d, class Rd, class DF>
struct Fun2Constructor;

// Note that this class has two signatures: it can be used either as
// a two argument function or as a single argument function (currying).
template <class Arg1, class Arg2, class Result>
class Fun2 {
   typedef Ref<Fun2Impl<Arg1, Arg2, Result> > RefImpl;
   RefImpl ref;
   template <class A1, class A2, class R> friend class Fun2;
   template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
   friend Fun2<A1d,A2d,Rd> explicit_convert2( const Fun2<A1s,A2s,Rs>& f );

   template <class A1d, class A2d, class Rd, class DF>
   friend struct Fun2Constructor;

   // kludge while this file not in namepsace
   typedef fcpp::AutoCurryType AutoCurryType;   
public:
   // Signature for normal use of the functoid (2 args)
   template <class P1, class P2 = void>
   struct Sig : public FunType<Arg1, Arg2, Result> {};

   // Signature for using this function with automatic currying (1-arg)
   template <class P1>
   struct Sig<P1,void> : public FunType<Arg1, Fun1<Arg2, Result> > {};

   // Signatures for using this function with underscore currying (1-arg)
   template <class P2>
   struct Sig<AutoCurryType,P2> 
   : public FunType<AutoCurryType, P2, Fun1<Arg1, Result> > {};
   template <class P1>
   struct Sig<P1,AutoCurryType> 
   : public FunType<P1, AutoCurryType, Fun1<Arg2, Result> > {};

   typedef Fun2Impl<Arg1,Arg2,Result>* Impl;
   Fun2( int, Impl i ) : ref(i) {}
   
   template <class DF>  // direct functoid (or subtype polymorphism)
   Fun2( const DF& df );

   // normal call
   Result operator()( const Arg1& x, const Arg2& y ) const { 
      return ref->operator()(x,y); 
   }
  
   // inheritable underscore currying!
   Fun1<Arg1, Result> operator()( const AutoCurryType&, const Arg2& y ) const { 
      return makeFun1(bind2of2(*this, y));
   }
   Fun1<Arg2, Result> operator()( const Arg1& x, const AutoCurryType& ) const { 
      return makeFun1(bind1of2(*this, x));
   }
  

   // REVIEW: Note that this could return a direct functoid, too, which
   // might be more efficient.  Same with other currying calls.

   // inheritable automatic currying!
   Fun1<Arg2,Result> operator()( const Arg1& x) const {
      return makeFun1(bind1of2(*this, x));
   }
};

template <class Arg1, class Arg2, class Result>
struct Fun2Impl : public CFunType<Arg1,Arg2,Result> {
   virtual Result operator()( const Arg1&, const Arg2& ) const =0;
   virtual ~Fun2Impl() {}
};

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
class Fun2Converter : public Fun2Impl<A1d,A2d,Rd> {
   typedef Ref<Fun2Impl<A1s,A2s,Rs> > MyFun;
   MyFun f;
public:
   Fun2Converter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y ) const {
      return f->operator()( x, y );
   }
};

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
Fun2Impl<A1d,A2d,Rd>* convert2( const Ref<Fun2Impl<A1s,A2s,Rs> >& f ) {
   return new Fun2Converter<A1d,A2d,Rd,A1s,A2s,Rs>( f );
}

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
class Fun2ExplicitConverter : public Fun2Impl<A1d,A2d,Rd> {
   typedef Ref<Fun2Impl<A1s,A2s,Rs> > MyFun;
   MyFun f;
public:
   Fun2ExplicitConverter( const MyFun& g ) : f(g) {}
   Rd operator()( const A1d& x, const A2d& y ) const {
      return static_cast<Rd>( f->operator()( static_cast<A1s>(x), 
                                             static_cast<A2s>(y) ) );
   }
};

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
Fun2<A1d,A2d,Rd> explicit_convert2( const Fun2<A1s,A2s,Rs>& f ) {
   return Fun2<A1d,A2d,Rd>( 1, 
      new Fun2ExplicitConverter<A1d,A2d,Rd,A1s,A2s,Rs>(f.ref) );
}

template <class Gen>
class Gen2 : public Fun2Impl<typename Gen::Sig<ANY_TYPE, ANY_TYPE>::Arg1Type, 
                             typename Gen::Sig<ANY_TYPE, ANY_TYPE>::Arg2Type,
                             typename Gen::Sig<ANY_TYPE, ANY_TYPE>::ResultType>
{
   Gen g;
public:
   Gen2( Gen x ) : g(x) {}
   typename Gen::Sig<ANY_TYPE, ANY_TYPE>::ResultType 
   operator()( const typename Gen::Sig<ANY_TYPE,ANY_TYPE>::Arg1Type& x,
               const typename Gen::Sig<ANY_TYPE,ANY_TYPE>::Arg2Type& y ) const { 
      return g(x,y); 
   }
};
   
template <class Binary>
Fun2<typename Binary::Sig<ANY_TYPE, ANY_TYPE>::Arg1Type, 
     typename Binary::Sig<ANY_TYPE, ANY_TYPE>::Arg2Type,
     typename Binary::Sig<ANY_TYPE, ANY_TYPE>::ResultType> 
makeFun2( const Binary& g ) {
   return Fun2<typename Binary::Sig<ANY_TYPE, ANY_TYPE>::Arg1Type,
               typename Binary::Sig<ANY_TYPE, ANY_TYPE>::Arg2Type,
               typename Binary::Sig<ANY_TYPE, ANY_TYPE>::ResultType> 
     (1,new Gen2<Binary>(g));
}

template <class Binary>
Gen2<Binary>* makeFun2Ref( const Binary& g ) {
   return new Gen2<Binary>(g);
}

// Note: conversion-from-direct-functoid and subtype-polymorphism create
// ambiguity in the case when we separate them into two constructors, so
// we just have one constructor and use partial specialization in this
// class to differentiate the two.
template <class A1d, class A2d, class Rd, class DF>
struct Fun2Constructor {
   static Fun2Impl<A1d,A2d,Rd>* make( const DF& df ) {
      return makeFun2Ref( ::fcpp::monomorphize2<A1d,A2d,Rd>(df) );
   }
};
template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
struct Fun2Constructor<A1d,A2d,Rd,Fun2<A1s,A2s,Rs> > {
   static Fun2Impl<A1d,A2d,Rd>* make( const Fun2<A1s,A2s,Rs>& f ) {
      return convert2<A1d,A2d,Rd>(f.ref);
   }
};

template <class Arg1, class Arg2, class Result>
template <class DF>  // direct functoid
Fun2<Arg1,Arg2,Result>::Fun2( const DF& df )
: ref( Fun2Constructor<Arg1,Arg2,Result,DF>::make(df) ) {}


#ifdef FCPP_USE_NAMESPACE
} // end namespace fcpp
#endif

#endif
