//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_SIGNATURE_DOT_H
#define FCPP_SIGNATURE_DOT_H

#ifdef FCPP_USE_NAMESPACE  
// g++ chokes for some reason
namespace fcpp {
#endif

//////////////////////////////////////////////////////////////////////
// Here are the classes with "nested typedefs" which just help us use
// our own type system; these classes are just inherited.
//
// Note that although the sigs support a large number of arguments, most 
// of the rest of the library only supports functions of 0-3 arguments.
//////////////////////////////////////////////////////////////////////

// Handy helper 'nothing' class.
struct Void {};


// This set names functoid arguments and results

template <class A1, class A2 = Void, class A3 = Void, class A4 = Void, 
                    class A5 = Void, class A6 = Void, class R = Void>
struct FunType {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
   typedef A5 Arg5Type;
   typedef A6 Arg6Type;
};

template <class A1, class A2, class A3, class A4, class A5, class R>
struct FunType<A1, A2, A3, A4, A5, R, Void> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
   typedef A5 Arg5Type;
};

template <class A1, class A2, class A3, class A4, class R>
struct FunType<A1, A2, A3, A4, R, Void, Void> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
   typedef A4 Arg4Type;
};

template <class A1, class A2, class A3, class R>
struct FunType<A1, A2, A3, R, Void, Void, Void> {
   typedef R ResultType;
   typedef A1 Arg1Type;
   typedef A2 Arg2Type;
   typedef A3 Arg3Type;
};

template <class A1, class A2, class R>
struct FunType<A1, A2, R, Void, Void, Void, Void> {
  typedef R ResultType;
  typedef A1 Arg1Type;
  typedef A2 Arg2Type;
};

template <class A1, class R>
struct FunType<A1, R, Void, Void, Void, Void, Void> {
  typedef R ResultType;
  typedef A1 Arg1Type;
};

template <class R>
struct FunType<R, Void, Void, Void, Void, Void, Void> {
  typedef R ResultType;
};

//////////////////////////////////////////////////////////////////////
// Concrete versions
//////////////////////////////////////////////////////////////////////
// This set is used for monomorphic direct functoids; the type names
// are inherited as-is, and also a template-Sig is defined so that
// monomorphic direct functoids can mix freely with polymorphic functoids 
// since the latter require a template-Sig member

template < class A1, class A2 = Void, class A3 = Void, class A4 = Void, 
                     class A5 = Void, class A6 = Void, class R = Void >
struct CFunType : public FunType<A1,A2,A3,A4,A5,A6,R> {
   template <class P1, class P2, class P3, class P4, class P5, class P6>
   struct Sig : public FunType<A1,A2,A3,A4,A5,A6,R> {};
};

template <class A1, class A2, class A3, class A4, class A5, class R>
struct CFunType<A1, A2, A3, A4, A5, R, Void> : 
  public FunType<A1,A2,A3,A4,A5,R> {
   template <class P1, class P2, class P3, class P4, class P5>
   struct Sig : public FunType<A1,A2,A3,A4,A5,R> {};
};

template <class A1, class A2, class A3, class A4, class R>
struct CFunType<A1, A2, A3, A4, R, Void, Void> :
  public FunType<A1,A2,A3,A4,R> {
  template <class P1, class P2, class P3, class P4>
  struct Sig : public FunType<A1,A2,A3,A4,R> {};
};

template <class A1, class A2, class A3, class R>
struct CFunType<A1, A2, A3, R, Void, Void, Void> :
  public FunType<A1,A2,A3,R> {
  template <class P1, class P2, class P3>
  struct Sig : public FunType<A1,A2,A3,R> {};
};

template <class A1, class A2, class R>
struct CFunType<A1, A2, R, Void, Void, Void, Void> :
  public FunType<A1,A2,R> {
  template <class P1, class P2>
  struct Sig : public FunType<A1,A2,R> {};
};

template <class A1, class R>
struct CFunType<A1, R, Void, Void, Void, Void, Void> :
  public FunType<A1,R> {
  template <class P1>
  struct Sig : public FunType<A1,R> {};
};

template <class R>
struct CFunType<R, Void, Void, Void, Void, Void, Void> :
  public FunType<R> {
  struct Sig : public FunType<R> {};
};

//////////////////////////////////////////////////////////////////////
// Icky helpers
//////////////////////////////////////////////////////////////////////
// These are strictly unnecessary, but they aVoid a bug in the g++
// compiler and also make some things shorter to type.  
// RT<T, args> means "return type of T when passed argument types <args>"

template <class T, class A1 = Void, class A2 = Void, class A3 = Void,
                   class A4 = Void, class A5 = Void, class A6 = Void>
struct RT {
   typedef typename T::template Sig<A1, A2, A3, A4, A5, A6>::ResultType
     ResultType;
};

template <class T, class A1, class A2, class A3, class A4, class A5>
struct RT<T, A1, A2, A3, A4, A5, Void> {
   typedef typename T::template Sig<A1, A2, A3, A4, A5>::ResultType
     ResultType;
};

template <class T, class A1, class A2, class A3, class A4>
struct RT<T, A1, A2, A3, A4, Void, Void> {
   typedef typename T::template Sig<A1, A2, A3, A4>::ResultType
     ResultType;
};

template <class T, class A1, class A2, class A3>
struct RT<T, A1, A2, A3, Void, Void, Void> {
   typedef typename T::template Sig<A1, A2, A3>::ResultType ResultType;
};

template <class T, class A1, class A2>
struct RT<T, A1, A2, Void, Void, Void, Void> {
   typedef typename T::template Sig<A1, A2>::ResultType ResultType;
};

template <class T, class A1>
struct RT<T, A1, Void, Void, Void, Void, Void> {
   typedef typename T::template Sig<A1>::ResultType ResultType;
};

template <class T>
struct RT<T, Void, Void, Void, Void, Void, Void> {
   typedef typename T::ResultType ResultType;
};

#ifdef FCPP_USE_NAMESPACE  
} // end namespace fcpp
#endif

#endif
