//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_LIST_DOT_H
#define FCPP_LIST_DOT_H

#ifdef FCPP_USE_SIMPLE_LIST
#include "simple_list.h"
#else

///////////////////////////////////////////////////////////////////////////
// Here we implement (lazy) lists in the List class.  There are also a
// number of functions associated with lists:
//  - head, tail, cons, one_element, cat, null
///////////////////////////////////////////////////////////////////////////

#include <iterator>
#include "function.h"

// Please excuse this mess, but we've been trying out GC.  :)
#ifdef FCPP_USE_GARBAGE_COLLECTION
#include "gc_cpp.h"
#endif

namespace fcpp {

template <class T> class List;

struct AUniqueTypeForNIL {};
namespace {
AUniqueTypeForNIL NIL; 
}

// This can probably be deprecated now.
template <class T> struct Nil {};

// Lists are just pairs of "values" and "functions returning a pointer
// to another such pair (or null)"; this enables laziness.  LazyPair is
// the data structure.

// This should probably go in the implementation namespace; clients
// don't need to know this exists.
template <class T>
struct LazyPair 
#ifdef FCPP_USE_GARBAGE_COLLECTION
#  ifndef FCPP_LEAK_MEMORY
#    ifdef FCPP_UNSAFE_GC
         : public gc
#    else
         : public gc_cleanup
#    endif
#  endif
#endif
{
#ifdef FCPP_USE_GARBAGE_COLLECTION
   typedef LazyPair* Type;
#else
   typedef Ref<LazyPair> Type;
#endif
   typedef Fun0<Type> GenFxn;

   T first;
   GenFxn second;
   LazyPair( const T& a, const GenFxn& b ) : first(a), second(b) {}
   LazyPair( const T& a, const List<T>& b );   // impl. after List def.
   LazyPair( const T& a, AUniqueTypeForNIL ) 
     : first(a), second(makeFun0(Const()(Type(0)))) {}
   LazyPair( const T& a, Nil<T> ) 
     : first(a), second(makeFun0(Const()(Type(0)))) {}
};

// Forward declarations that List needs to know about
template <class T> class magic;
template <class T> class ListIterator;
class Cat;
class Cons;

// The List class wraps up the data structure and presents a friendly
// interface.
template <class T>
class List {
   struct Dummy : public CFunType<Ref<LazyPair<T> > > {
      Ref<LazyPair<T> > operator()() const { return Ref<LazyPair<T> >(0); }
   };
   static Fun0<Ref<LazyPair<T> > > dummy() { 
      static Fun0<Ref<LazyPair<T> > > f =
             makeFun0( Dummy() );
      return f;
   }

   typedef LazyPair<T>::Type PTR;
   typedef LazyPair<T>::GenFxn FXN;
   mutable PTR head_;
   FXN fxn;
   mutable bool fxn_takes_priority;

   void cache() const {
      if( fxn_takes_priority ) {
         head_ = fxn();
         fxn_takes_priority = false;
      }
   }
   FXN gen() const {
      if( fxn_takes_priority ) 
         return fxn;
      else
         return FXN(makeFun0(Const()(head_)));
   }

   friend class LazyPair<T>;
   friend class magic<T>;
   friend class Cat;
   friend class Cons;

   template <class U, class F>
   friend class extract_from_list;

   template <class It>
   static typename LazyPair<typename std::iterator_traits<It>::value_type>::Type
   stl_range( const It& i, const It& end ) {
      typedef typename std::iterator_traits<It>::value_type U;
      typedef typename LazyPair<U>::Type Type;
      if( i==end )
         return Type(0);
      else {
         It j = i;
         j++;
         Type (*pf)( const It&, const It& ) = &List::stl_range;
         return Type( new LazyPair<U>( *i, makeFun0( 
                         bind1and2of2(ptr_to_fun(pf),j,end) ) ) );
      }
   }

   // List::tail() and Cat use this one.
   // The dummy parameter is to distinguish from other constructors.
   List( int, FXN f ) : head_(0), fxn(f), fxn_takes_priority(1) {} 

   // Cons, Cat, and stl_range know about the underlying implementation
   // and use this constructor sometimes.
   List( PTR p ) : head_(p), fxn(dummy()), fxn_takes_priority(0) {}
public:
   typedef T ElementType;

   List() : head_(0), fxn(dummy()), fxn_takes_priority(0) {}
   List( AUniqueTypeForNIL ) : head_(0), fxn(dummy()), fxn_takes_priority(0) {}
   List( Nil<T> ) : head_(0), fxn(dummy()), fxn_takes_priority(0) {}

   template <class F>   // F is a function which returns a list
   List( const F& );

   template <class It>
   explicit List( It begin, It end ) 
   : head_(stl_range(begin,end)), fxn(dummy()), fxn_takes_priority(0) {}

   operator bool() const {              // not empty
      cache();
      return head_ != 0;
   }
   T head() const {
      cache();
      return head_->first;
   }
   List<T> tail() const {
      cache();
      return List<T>( 1, head_->second ); 
   }

   // The following helps makes List almost an STL "container"
   typedef T value_type;
   typedef ListIterator<T> const_iterator;
   typedef const_iterator iterator;         // List is immutable
   iterator begin() const;
   iterator end() const;
};

// Now that List is defined, we can finish defining LazyPair
template <class T>
LazyPair<T>::LazyPair( const T& a, const List<T>& b ) 
: first(a), second(b.gen()) {}


// We try to make List almost an STL container, and so we define
// iterators on it.
template <class T>
class ListIterator : public std::input_iterator<T,ptrdiff_t> {
   List<T> l;
   bool is_nil;
   void advance() {
      l = l.tail();
      if( !l )
         is_nil = true;
   }
public:
   ListIterator() : l(), is_nil(true) {}
   explicit ListIterator( const List<T>& ll ) : l(ll), is_nil(!ll) {}
   const T operator*() const {
      return l.head();
   }
   ListIterator<T>& operator++() {
      advance();
      return *this;
   }
   const ListIterator<T> operator++(int) {
      ListIterator<T> i( *this );
      advance();
      return i;
   }
   bool operator==( const ListIterator<T>& i ) const {
      return is_nil && i.is_nil;
   }
   bool operator!=( const ListIterator<T>& i ) const {
      return ! this->operator==(i);
   }
};

template <class T>
List<T>::iterator List<T>::begin() const {
   return ListIterator<T>( *this );
}
template <class T>
List<T>::iterator List<T>::end() const {
   return ListIterator<T>();
}



struct Head {
   template <class L>
   struct Sig : public FunType<L,typename L::ElementType> {};

   template <class T>
   T operator()( const List<T>& l ) const {
      return l.head();
   }
};
// HSP
namespace {
Head head; 
}

struct Tail {
   template <class L>
   struct Sig : public FunType<L,L> {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return l.tail();
   }
};
// HSP
namespace {
Tail tail;
}

struct Null {
   template <class L>
   struct Sig : public FunType<L,bool> {};

   template <class T>
   bool operator()( const List<T>& l ) {
      return !l;
   }
};
// HSP
namespace {
Null null; 
}

//////////////////////////////////////////////////////////////////////
// This next bit is ugly, but I struggled with representations; it seems
// like sometimes we have a list, sometimes a LazyPair, sometimes NIL,
// etc... there may be a cleaner way to do this, but this works.
//////////////////////////////////////////////////////////////////////

// A helper for "magic"; this helps turn a function-that-returns-a-list
// into a GenFxn
template <class T, class F>
struct extract_from_list : public CFunType<typename LazyPair<T>::Type> {
   F f;
   extract_from_list( const F& ff ) : f(ff) {}

   typename LazyPair<T>::Type operator()() const {
      List<T> l = f();
      l.cache();
      return l.head_;
   }
};
   
// Takes either a List or a functoid-that-returns-a-List, and creates a
// GenFxn out of it.
template <class T>
struct magic {
   template <class F>
   LazyPair<T>::GenFxn operator()( const F& f ) {
      return makeFun0( extract_from_list<T,F>(f) );
   }
   LazyPair<T>::GenFxn operator()( Nil<T> ) {
      return LazyPair<T>::GenFxn(makeFun0(Const()(LazyPair<T>::Type(0))));
   }
   LazyPair<T>::GenFxn operator()( AUniqueTypeForNIL ) {
      return LazyPair<T>::GenFxn(makeFun0(Const()(LazyPair<T>::Type(0))));
   }
   LazyPair<T>::GenFxn operator()( const List<T>& l ) {
      return l.gen();
   }
};

template <class T> 
template <class F>   // F is a function which returns a list
List<T>::List( const F& f ) : head_(0), 
   fxn( makeFun0(extract_from_list<T,F>(f)) ), fxn_takes_priority(1) {} 

//////////////////////////////////////////////////////////////////////
// Cons is much happier now.  You can say things like
//    cons( 1, cons( 2, NIL ) )
// and you can also say
//    cons( f(head(l)), bind1of1(g,tail(l)) )
// and pass it a thunk that will return a list, rather than a list, as
// the second argument; this is how to write lazy functions like "map".
//////////////////////////////////////////////////////////////////////

// Cons should take a T and, List<T>, or F where F::Sig::ResultType is List<T>
// HSP (:)
struct Cons {
   template <class T, class LLGen>
   struct Sig : public FunType<T,LLGen,List<T> > {};

   template <class T, class LLGen>
   List<T> operator()( const T& x, const LLGen& g ) const {
      typedef LazyPair<T>::Type PTR;
      typedef LazyPair<T>       PAIR;
      return PTR( new PAIR(x,magic<T>()(g)) );
   }
};
namespace {
Cons _cons;
Curryable2<Cons> cons(_cons);
}

// deprecate this, as cons works correctly now
struct OneElement {
   template <class T>
   struct Sig : public FunType<T,List<T> > {};

   template <class T>
   List<T> operator()( const T& x ) const {
      return cons(x,NIL);
   }
}; 
namespace {
OneElement one_element;
}


// Ugly, but it solves a problem...
// namely that in some cases I have either a List<T> or a thunk that
// returns a List<T>, and I want to uniformly ask what T is.
template <class F>
struct ElementTypeTraits {
   typedef typename F::Sig::ResultType::ElementType Type;
};
template <class T>
struct ElementTypeTraits<List<T> > {
   typedef T Type;
};

// This should probably be in the implementation namespace
template <class T>
class CatHelper : public CFunType< typename LazyPair<T>::Type > {
   typedef typename LazyPair<T>::GenFxn GenFxn;
   typedef typename LazyPair<T>::Type PTR;
   typedef LazyPair<T> PAIR;
   GenFxn a, b;
public:
   CatHelper( const GenFxn& aa, const GenFxn& bb ) : a(aa), b(bb) {}

   PTR operator()() const {
      PTR aa = a();
      if( aa == 0 )
         return b();
      else
         return PTR( new PAIR( aa->first,
                               makeFun0(CatHelper(aa->second,b)) ) );
   }
};

// Cat, like cons, works on lists or functions-that-return-lists
// HSP (++)
struct Cat {
   template <class LLGenA, class LLGenB>
   struct Sig : public FunType<LLGenA,LLGenB,
       List<typename ElementTypeTraits<LLGenA>::Type> > {};

   template <class LLGenA, class LLGenB>
   List<typename ElementTypeTraits<LLGenA>::Type>
   operator()( const LLGenA& a, const LLGenB& b ) const {
      typedef typename ElementTypeTraits<LLGenA>::Type T;
      typedef typename LazyPair<T>::GenFxn GenFxn;
      typedef typename LazyPair<T>::Type Type;
      GenFxn aa = magic<T>()(a);
      GenFxn bb = magic<T>()(b);
      return List<T>( 1, makeFun0(CatHelper<T>(aa,bb)) );
   }      
};
namespace {
Cat _cat;
Curryable2<Cat> cat(_cat);
}


// Here's some stuff that used to be in operator.h

//////////////////////////////////////////////////////////////////////
// List comparisons (they even work correctly now :) )
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
bool operator==( const AUniqueTypeForNIL&, const List<T>& b ) {
   return null(b);
}

template <class T>
bool operator==( const List<T>& a, const AUniqueTypeForNIL& ) {
   return null(a);
}

template <class T>
bool operator<( const List<T>& a, const List<T>& b ) {
   if( null(b) )
      return false;
   if( null(a) )
      return true;
   return (head(a) < head(b)) ||
          ((head(a) == head(b)) && (tail(a) < tail(b)));
}

template <class T>
bool operator<( const List<T>&, const AUniqueTypeForNIL& ) {
   return false;
}

template <class T>
bool operator<( const AUniqueTypeForNIL&, List<T>& b ) {
   return !null(b);
}

//////////////////////////////////////////////////////////////////////
// Handy functions for making list literals
//////////////////////////////////////////////////////////////////////
// Yes, these aren't functoids, they're just template functions.  I'm
// lazy and created these mostly to make it easily to make little lists
// in the sample code snippets that appear in papers.

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

template <class T>
List<T> 
list_with( const T& a, const T& b, const T& c, const T& d, const T& e ) {
   List<T> l;
   l = cons( e, l );
   l = cons( d, l );
   l = cons( c, l );
   l = cons( b, l );
   l = cons( a, l );
   return l;
}

//////////////////////////////////////////////////////////////////////
// List operations on lazy lists
//////////////////////////////////////////////////////////////////////
// Why is this here?  I should move this to prelude, probably.
// Oh, I guess it's not part of the HSP?
// Hmm.

struct ListUntil {
   template <class P, class F, class T>
   struct Sig : public FunType<P,F,T,List<T> > {};

   template <class Pred, class Unary, class T>
   List<T> operator()( const Pred& p, const Unary& f, const T& x ) const {
      if( p(x) )
         return NIL;
      else
         return cons(x, curry3(ListUntil(),p,f,f(x)));
   }
};
namespace {
ListUntil _listUntil;
Curryable3<ListUntil> listUntil(_listUntil);
}


} // namespace fcpp

#endif
#endif
