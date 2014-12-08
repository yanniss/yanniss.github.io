#include <iostream>
#include "prelude.h"

using namespace fcpp;
using std::cout;
using std::endl;

/* A functoid to test the fixpoint operator. Implements the Fibonacci
   function. */
template <class X>
struct FibGenHelper : public CFunType<int, int> {
  X x;

  FibGenHelper (const X& in_x) : x(in_x) {}
  
  int operator () (const int& a) const {
    if (a == 0 || a == 1)
      return 1;
    else
      return x(a - 1) + x(a - 2);
  }
};

struct FibGen {
  template <class X>
  struct Sig : public FunType<X, FibGenHelper<X> > {};

  template <class X>
  FibGenHelper<X> operator () (const X& x) const {
    return FibGenHelper<X>(x);
  }
} fibgen;

// An example of a polymorphic function, to show that Y isn't limited to
// working on monomorphic functions.
template <class Self>
struct MapGenHelper {
   Self self;
   MapGenHelper( const Self& s ) : self(s) {}

   template <class F, class L> struct Sig : public FunType<F,L,
      OddList<typename RT<F,typename L::ElementType>::ResultType> > {};

   template <class F, class L>
   typename Sig<F,L>::ResultType
   operator()( const F& f, const L& l ) const {
      if( null(l) )
         return NIL;
      else
         return cons( f(head(l)), curry2(self,f,tail(l)) );
   }
};

struct MapGen {
   template <class Self>
   struct Sig : public FunType<Self,MapGenHelper<Self> > {};

   template <class Self>
   MapGenHelper<Self> operator()( const Self& s ) const
   { return MapGenHelper<Self>(s); }
} mapgen;

template <class F>
class YHelper {
  F function;
public:
  YHelper(const F& f) : function(f) {}

  template <class A>
  struct Sig : public FunType<A, 
    typename RT<typename RT<F,YHelper<F> >::ResultType,A>::ResultType> {};

  template <class A> 
  typename Sig<A>::ResultType operator () (const A& a) const {
    return function(*this)(a);
  }
};

struct Y {
  template <class F>
  struct Sig : public FunType<F, YHelper<F> > {};

  template <class F>
  typename Sig<F>::ResultType operator () (const F& f) const {
    return YHelper<F>(f);
  }
} y;

// This is kinda shoddy.  It is not too hard to make a single Y
// combinator which can be applied to any functoid, regardless of the
// number of arguments it expects.  Here we just define two versions:
// "y" which works for one-args and "y2" which works for two-args.  If Y
// is ever move into the library, I'll do it right; as an example file,
// I don't care enough to bother.  :)
template <class F>
class Y2Helper {
  F function;
public:
  Y2Helper(const F& f) : function(f) {}

  template <class A, class B>
  struct Sig : public FunType<A, B,
    typename RT<typename RT<F,Y2Helper<F> >::ResultType,A,B>::ResultType> {};

  template <class A, class B> 
  typename Sig<A,B>::ResultType 
  operator()( const A& a, const B& b ) const {
    return function(*this)(a,b);
  }
};

struct Y2 {
  template <class F>
  struct Sig : public FunType<F, Y2Helper<F> > {};

  template <class F>
  typename Sig<F>::ResultType operator () (const F& f) const {
    return Y2Helper<F>(f);
  }
} y2;

int main () {
  cout <<
    "Testing the Y combinator." << endl <<
    "Should return a finonacci sequence, actually returned: ";
  for (int i = 0; i < 10; i++)
    cout << y(fibgen)(i) << " ";
  cout << endl << endl;

   cout << "Y test to show it works for polymorphic functions" << endl;
   cout << "Should return list of ints starting at 4..." << endl;
   List<int> l = y2(mapgen)( fcpp::plus(3), enumFrom(1) );
   l = take(10,l);
   while( !null(l) ) {
      cout << head(l) << " ";
      l = tail(l);
   }
   cout << endl;
}
