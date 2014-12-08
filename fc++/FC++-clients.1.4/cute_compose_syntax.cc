#include "prelude.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace fcpp;

// This file shows how to use syntax tricks to make
//    f _o_ g
// be the same as
//    compose(f,g)
//
// Futhermore, f and g don't even have to be functoids; they could be
// plain function pointers.

//////////////////////////////////////////////////////////////////////
// ptr_to_fun clever cheat
//////////////////////////////////////////////////////////////////////

struct PtrToFun {
   template <class F>
   struct Sig : public FunType<F,F> {};

   template <class Result>
   struct Sig<Result (*)()> 
   : public FunType<Result (*)(),ptr_to_nullary_function<Result> > {};

   template <class Arg, class Result>
   struct Sig<Result (*)(Arg)>
   : public FunType<Result (*)(Arg),ptr_to_unary_function<Arg, Result> > {};

   template <class Arg1, class Arg2, class Result>
   struct Sig<Result (*)(Arg1, Arg2)>
   : public FunType<Result (*)(Arg1, Arg2),
   Curryable2<ptr_to_binary_function<Arg1, Arg2, Result> > > {};

   template <class Arg1, class Arg2, class Arg3, class Result>
   struct Sig<Result (*)(Arg1, Arg2, Arg3)>
   : public FunType<Result (*)(Arg1, Arg2, Arg3),
   Curryable3<ptr_to_ternary_function<Arg1, Arg2, Arg3, Result> > > {};
   
   template <class Arg1, class Arg2, class Arg3, class Result>
   struct Sig<Result (Arg1::*)(Arg2,Arg3)>
   : public FunType<Result (Arg1::*)(Arg2,Arg3),
   Curryable3<ptr_to_mem_binary_function<Arg1, Arg2, Arg3, Result> > > {};
   
   template <class Arg1, class Arg2, class Result>
   struct Sig<Result (Arg1::*)(Arg2)>
   : public FunType<Result (Arg1::*)(Arg2),
   Curryable2<ptr_to_mem_unary_function<Arg1, Arg2, Result> > > {};
   
   template <class Arg1, class Result>
   struct Sig<Result (Arg1::*)()> 
   : public FunType<Result (Arg1::*)(),
   ptr_to_mem_nullary_function<Arg1, Result> > {};

   template <class F>
   inline F operator()( F f ) const { return f; }
   
   template <class Result>
   inline ptr_to_nullary_function<Result>
   operator()(Result (*x)()) const {
     return ptr_to_nullary_function<Result>(x);
   }
   
   template <class Arg, class Result>
   inline ptr_to_unary_function<Arg, Result>
   operator()(Result (*x)(Arg)) const {
     return ptr_to_unary_function<Arg, Result>(x);
   }
   
   template <class Arg1, class Arg2, class Result>
   inline Curryable2<ptr_to_binary_function<Arg1, Arg2, Result> >
   operator()(Result (*x)(Arg1, Arg2)) const {
     return makeCurryable2( ptr_to_binary_function<Arg1, Arg2, Result>(x) );
   }
   
   template <class Arg1, class Arg2, class Arg3, class Result>
   inline Curryable3<ptr_to_ternary_function<Arg1, Arg2, Arg3, Result> >
   operator()(Result (*x)(Arg1, Arg2, Arg3)) const {
     return makeCurryable3( ptr_to_ternary_function<Arg1,Arg2,Arg3,Result>(x) );
   }
   
   template <class Arg1, class Arg2, class Arg3, class Result>
   inline Curryable3<ptr_to_mem_binary_function<Arg1, Arg2, Arg3, Result> >
   operator()(Result (Arg1::*x)(Arg2,Arg3)) const {
     return makeCurryable3(
   				ptr_to_mem_binary_function<Arg1, Arg2, Arg3, Result>(x) );
   }
   
   template <class Arg1, class Arg2, class Result>
   inline Curryable2<ptr_to_mem_unary_function<Arg1, Arg2, Result> >
   operator()(Result (Arg1::*x)(Arg2)) const {
     return makeCurryable2( ptr_to_mem_unary_function<Arg1, Arg2, Result>(x) );
   }
   
   template <class Arg1, class Result>
   inline ptr_to_mem_nullary_function<Arg1, Result>
   operator()(Result (Arg1::*x)()) const {
     return ptr_to_mem_nullary_function<Arg1, Result>(x);
   }
} xptr_to_fun;
   
//////////////////////////////////////////////////////////////////////
// _o_ compose 'operator'
//////////////////////////////////////////////////////////////////////
class Dummy {} compose_dummy;

template <class F>
class Dummy2 {
   F f;
   Dummy2( const F& ff ) : f(ff) {}

   template <class FF>
   friend Dummy2<FF> operator%( FF f, Dummy );

   template <class FF, class G>
   friend typename Compose::Sig<typename PtrToFun::Sig<FF>::ResultType,
          typename PtrToFun::Sig<G>::ResultType>::ResultType
   operator%( const Dummy2<FF>& f, G g );
};

template <class F>
Dummy2<F> operator%( F f, Dummy ) { return Dummy2<F>(f); }

template <class F, class G>
typename Compose::Sig<typename PtrToFun::Sig<F>::ResultType,
          typename PtrToFun::Sig<G>::ResultType>::ResultType
operator%( const Dummy2<F>& f, G g ) {
   return compose( xptr_to_fun(f.f), xptr_to_fun(g) );
}

#define _o_   % compose_dummy %

//////////////////////////////////////////////////////////////////////
// main program
//////////////////////////////////////////////////////////////////////

int f(int x) { return 2*x + 1; }
int g(int x) { return 3*x + 1; }
int h(int x) { return 5*x + 1; }

int main() {
   cout << f(g(h(1))) << endl;   
   cout << f(g(h(2))) << endl;   
   cout << compose( ptr_to_fun(f), 
                    compose( ptr_to_fun(g), ptr_to_fun(h) ) )(1) << endl;
   cout << compose( ptr_to_fun(f), 
                    compose( ptr_to_fun(g), ptr_to_fun(h) ) )(2) << endl;
   cout << compose( compose( ptr_to_fun(f), 
                             ptr_to_fun(g)), ptr_to_fun(h) )(2) << endl;
   cout << (ptr_to_fun(f) _o_ ptr_to_fun(g) _o_ ptr_to_fun(h)) (1) << endl;
   cout << (ptr_to_fun(f) _o_ ptr_to_fun(g) _o_ ptr_to_fun(h)) (2) << endl;
   cout << (f _o_ g _o_ h) (1) << endl;
   cout << (f _o_ g _o_ h) (2) << endl;
   cout << (equal(69) _o_ f _o_ g _o_ ptr_to_fun(h)) (2) << endl;
}

