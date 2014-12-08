//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FUNCTION_DOT_H
#define FUNCTION_DOT_H

//////////////////////////////////////////////////////////////////////////
// Here is where FunN and FunNImpl classes are declared.  Also here are
//  - makeFunN    turn a monomorphic direct functoid into an indirect 
//                functoid
//  - convertN    for implicit conversions (subtype polymorphism)
//  - explicit_convertN   like convertN, but uses casts (non-implicit)
//////////////////////////////////////////////////////////////////////////

// Note: it looks like the Genx classes could be local to makeFunx()
// Note: conversions on fun2 are untested

#include "ref_count.h"
#include "signature.h"

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

template <class Result>
class Fun0 : public CFunType<Result> {
   typedef Ref<Fun0Impl<Result> > RefImpl;
   RefImpl ref;
   template <class T> friend class Fun0; 
   template <class Rd, class Rs>
   friend Fun0<Rd> explicit_convert0( const Fun0<Rs>& f );
public:
   typedef Fun0Impl<Result>* Impl;
   Fun0( Impl i ) : ref(i) {}
   Result operator()() const { return ref->operator()(); }

   // for subtype polymorphism
   template <class Rs>
   Fun0( const Fun0<Rs>& f ) : ref(convert0<Result>(f.ref)) {}
};

template <class Result>
struct Fun0Impl : public CFunType<Result> {
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
   return Fun0<Rd>( new Fun0ExplicitConverter<Rd,Rs>( f.ref ) );
}

template <class Gen>
class Gen0 : public Fun0Impl<typename Gen::Sig::ResultType> {
   Gen g;
public:
   Gen0( Gen x ) : g(x) {}
   ResultType operator()() const { return g(); }
};
   
template <class Gen>
Fun0<typename Gen::Sig::ResultType> makeFun0( const Gen& g ) {
   return Fun0<typename Gen::Sig::ResultType>(new Gen0<Gen>(g));
}

//////////////////////////////////////////////////////////////////////

template <class Arg1, class Result>
struct Fun1Impl;

template <class A1d, class Rd, class A1s, class Rs> 
Fun1Impl<A1d,Rd>* convert1( const Ref<Fun1Impl<A1s,Rs> >& f );

template <class Arg1, class Result>
class Fun1 : public CFunType<Arg1,Result> {
   typedef Ref<Fun1Impl<Arg1,Result> > RefImpl;
   RefImpl ref;
   template <class A, class B> friend class Fun1;
   
   template <class A1d, class Rd, class A1s, class Rs> 
   friend Fun1<A1d,Rd> explicit_convert1( const Fun1<A1s,Rs>& f );
public:
   typedef Fun1Impl<Arg1,Result>* Impl;
   Fun1( Impl i ) : ref(i) {}
   Result operator()( const Arg1& x ) const { return ref->operator()(x); }

   // for subtype polymorphism
   template <class A1s,class Rs>
   Fun1( const Fun1<A1s,Rs>& f ) : ref(convert1<Arg1,Result>(f.ref)) {}
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
   return Fun1<A1d,Rd>( new Fun1ExplicitConverter<A1d,Rd,A1s,Rs>(f.ref) );
}


template <class Gen>
class Gen1 : public Fun1Impl<typename Gen::Sig<ANY_TYPE>::Arg1Type, 
                             typename Gen::Sig<ANY_TYPE>::ResultType> {
   Gen g;
public:
   Gen1( Gen x ) : g(x) {}
   ResultType operator()( const Arg1Type& x ) const { return g(x); }
};
   

template <class Unary>
Fun1<typename Unary::Sig<ANY_TYPE>::Arg1Type, 
     typename Unary::Sig<ANY_TYPE>::ResultType> 
makeFun1( const Unary& g ) {
   return 
     Fun1<typename Unary::Sig<ANY_TYPE>::Arg1Type,
          typename Unary::Sig<ANY_TYPE>::ResultType> 
     (new Gen1<Unary>(g));
}


//////////////////////////////////////////////////////////////////////

template <class Arg1, class Arg2, class Result>
struct Fun2Impl;

template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
Fun2Impl<A1d,A2d,Rd>* convert2( Ref<Fun2Impl<A1s,A2s,Rs> >& f );

template <class Arg1, class Arg2, class Result>
class Fun2 : public CFunType<Arg1,Arg2,Result> {
   typedef Ref<Fun2Impl<Arg1, Arg2, Result> > RefImpl;
   RefImpl ref;
   template <class A1, class A2, class R> friend class Fun2;
   template <class A1d, class A2d, class Rd, class A1s, class A2s, class Rs>
   friend Fun2<A1d,A2d,Rd> explicit_convert2( const Fun2<A1s,A2s,Rs>& f );
public:
   typedef Fun2Impl<Arg1,Arg2,Result>* Impl;
   Fun2( Impl i ) : ref(i) {}
   Result operator()( const Arg1& x, const Arg2& y ) const { 
      return ref->operator()(x,y); 
   }
   template <class A1s, class A2s, class Rs>
   Fun2( const Fun2<A1s,A2s,Rs>& f ) : ref(convert2<A1d,A2d,Rd>(f.ref)) {}
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
Fun2Impl<A1d,A2d,Rd>* convert2( Ref<Fun2Impl<A1s,A2s,Rs> >& f ) {
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
   return Fun2<A1d,A2d,Rd>(
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
   ResultType operator()( const Arg1Type& x,
                          const Arg2Type& y ) const { 
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
     (new Gen2<Binary>(g));
}

#endif
