//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef LIST_DOT_H
#define LIST_DOT_H

///////////////////////////////////////////////////////////////////////////
// Here we implement (lazy) lists in the List class.  There are also a
// number of functions associated with lists:
//  - head, tail, cons, one_element, cat, null
///////////////////////////////////////////////////////////////////////////

#include <utility>
#include <iterator>
#include "function.h"
#include "curry.h"

template <class T> class List;

struct Tail;

template <class T>
struct F0Proxy {
   typedef T WrappedType;
   typedef Ref<pair<T,Fun0<F0Proxy> > > Result;
   Result r;
   F0Proxy( const Result& rr ) : r(rr) {}
   operator Result() { return r; }
};

template <class T>
struct Nil : public CFunType<F0Proxy<T> > {
   F0Proxy<T> operator()() const {
      return Ref<pair<T,Fun0<F0Proxy<T> > > >(0);
   }
};

struct AUniqueTypeForNIL {};
namespace { AUniqueTypeForNIL NIL; }

// This class is key; it takes a function that returns a List<T>, and
// wraps it into a function that returns an F0Proxy<T>.
template <class T, class LLGen>
class ListGen : public CFunType<F0Proxy<T> > {
   LLGen g;
public:
   ListGen( const LLGen& x ) : g(x) {}
   F0Proxy<T> operator()() const;
};

template <class T, class LLGen>
class ConsHelper : public CFunType<F0Proxy<T> > {
   T data;
   typedef Fun0<F0Proxy<T> > GenFxn;
   LLGen tail;
public:
   ConsHelper( const T& x, const LLGen& y ) : data(x), tail(y) {}
   F0Proxy<T> operator()() const {
      return Ref<pair<T,GenFxn> >( new pair<T,GenFxn>( data, 
                makeFun0( ListGen<T,LLGen>(tail) ) ));
   }
};

struct Cons;

template <class T>
class ConsHelper<T,List<T> > : public CFunType<F0Proxy<T> > {
   T data;
   List<T> list;
public:
   ConsHelper( const T& x, const List<T>& y ) : data(x), list(y) {}
   F0Proxy<T> operator()() const {
      typedef Fun0<F0Proxy<T> > GenFxn;
      List<T> l = Cons()(data,Const()(list)); 
      return ListGen<T,ConstHelper<List<T> > >( Const()(l) )(); 
   }
};

// HSP (:)
struct Cons {
   template <class T, class LLGen>
   struct Sig : public FunType<T,LLGen,ConsHelper<T,LLGen> > {};

   template <class T, class LLGen>
   ConsHelper<T,LLGen> operator()( const T& x, const LLGen& g ) const {
      return ConsHelper<T,LLGen>( x, g );
   }
};
namespace { Cons cons; }

struct StrictCons {
   template <class T, class LT>
   struct Sig : public FunType<T,LT,LT> {};

   template <class T>
   List<T> operator()( const T& x, const List<T>& g ) const {
      return cons( x, g );
   }
};

template <class T>
class OneElementHelper : public CFunType<F0Proxy<T> > {
   T data;
   typedef Fun0<F0Proxy<T> > GenFxn;
   typedef pair<T,GenFxn> Pair;
public:
   OneElementHelper( const T& x ) : data(x) {}
   F0Proxy<T> operator()() const {
      return Ref<Pair>( new Pair( data, makeFun0(Nil<T>()) ) );
   }
};

struct OneElement {
   template <class T>
   struct Sig : public FunType<T,OneElementHelper<T> > {};

   template <class T>
   OneElementHelper<T> operator()( const T& x ) const {
      return OneElementHelper<T>( x );
   }
};
namespace { OneElement one_element; }

template <class LLGenA, class LLGenB>
class CatHelper 
: public CFunType<F0Proxy<typename LLGenA::Sig::ResultType::WrappedType> > {
   LLGenA a;
   LLGenB b;
public:
   typedef typename LLGenA::Sig::ResultType TheList;
   typedef typename TheList::WrappedType WrappedType;
   typedef WrappedType T;
   CatHelper( const LLGenA& x, const LLGenB& y ) : a(x), b(y) {}

   F0Proxy<T> operator()() const;
};

struct Cat; 

template <class T>
class CatHelper< List<T>, List<T> >     // explicit template specialization
: public CFunType<F0Proxy<T> > {
   List<T> aa, bb;
public:
   CatHelper( const List<T>& a, const List<T>& b ) : aa(a), bb(b) {}
   F0Proxy<T> operator()() const {
      typedef Fun0<F0Proxy<T> > GenFxn;
      List<T> l = Cat()(Const()(aa),Const()(bb)); 
      return ListGen<T,ConstHelper<List<T> > >( Const()(l) )(); 
   }
};

// HSP (++)
// Note: this will work on two list-generators or on two plain old lists.  
struct Cat {
   template <class LLGenA, class LLGenB>
   struct Sig : public FunType<LLGenA,LLGenB,CatHelper<LLGenA,LLGenB> > {};

   template <class LLGenA, class LLGenB>
   CatHelper<LLGenA,LLGenB> 
   operator()( const LLGenA& a, const LLGenB& b ) const {
      return CatHelper<LLGenA,LLGenB>(a,b);
   }
};
namespace { Cat cat; }

template <class T>
class ListIterator;

// FIX THIS: should have second templ param for GenFxn (with default), 
// so can make lazy lists without virtual function calls if we desire
template <class T>
class List {
   typedef Fun0<F0Proxy<T> > GenFxn;
   GenFxn g;
   mutable Ref<pair<T,GenFxn> > temp;
   void cache() const {
      //if( temp.get() == 0 )
      if( temp == 0 )
         temp = g();   
   }
   template <class U, class LLGen> friend class ListGen;
   template <class U, class V>     friend class CatHelper;
   //friend F0Proxy<U> ListGen<U,LLGen>::operator()() const;

   template <class It>
   class stl_range 
   : public CFunType<F0Proxy<iterator_traits<It>::value_type> > {
      typedef iterator_traits<It>::value_type U;
      It i, end;
   public:
      stl_range( const It& a, const It& b ) : i(a), end(b) {}
      F0Proxy<U> operator()() const {
         if( i==end )
            return Ref<pair<U,Fun0<F0Proxy<U> > > >(0);
         else {
            It j = i;
            j++;
            return Ref<pair<U,Fun0<F0Proxy<U> > > >
                (new pair<U,Fun0<F0Proxy<U> > >( *i, 
                 makeFun0( stl_range(j,end) ) ) );
         }
      }
   };

public:
   typedef T WrappedType;

   explicit List() : g(makeFun0(Nil<T>())) {}
   List( GenFxn f ) : g(f) {}

   List( AUniqueTypeForNIL ) : g(makeFun0(Nil<T>())) {}

   template <class Gen>
   List( Gen f ) : g(makeFun0(f)) {}

   template <class It>
   List( It begin, It end ) : g(makeFun0(stl_range<It>(begin,end))) {}

   operator bool() const {              // not empty
      cache();
      //return temp.get() != 0;
      return temp != 0;
   }
   T head() const {
      cache();
      return temp->first;
   }
   List<T> tail() const {
      cache();
      return List<T>( temp->second );
   }

   // The following helps makes List almost an STL "container"
   typedef T value_type;
   typedef ListIterator<T> const_iterator;
   typedef const_iterator iterator;         // List is immutable
   iterator begin() const;
   iterator end() const;
};

template <class T>
class ListIterator : public input_iterator<T,ptrdiff_t> {
   List<T> l;
   bool is_nil;
   void advance() {
      l = l.tail();
      if( !l )
         is_nil = true;
   }
public:
   ListIterator( const List<T>& ll ) : l(ll), is_nil(!ll) {}
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
   return ListIterator<T>( Nil<T>() );
}


struct Head {
   template <class L>
   struct Sig : public FunType<L,typename L::WrappedType> {};

   template <class T>
   T operator()( const List<T>& l ) const {
      return l.head();
   }
};
// HSP
namespace { Head head; }

struct Tail {
   template <class L>
   struct Sig : public FunType<L,L> {};

   template <class T>
   List<T> operator()( const List<T>& l ) const {
      return l.tail();
   }
};
// HSP
 namespace { Tail tail; }

struct Null {
   template <class L>
   struct Sig : public FunType<L,bool> {};

   template <class T>
   bool operator()( const List<T>& l ) {
      return !l;
   }
};
// HSP
 namespace { Null null; }

template <class T, class LLGen>
F0Proxy<T> ListGen<T,LLGen>::operator()() const {
   return g().g();
}

template <class LLGenA, class LLGenB>
F0Proxy<typename CatHelper<LLGenA,LLGenB>::WrappedType> 
CatHelper<LLGenA,LLGenB>::operator()() const {
   typedef Fun0<F0Proxy<T> > GenFxn;
   typedef pair<T,GenFxn> Pair;
   typedef Ref<Pair> Result;
   List<T> ax = a();
   if( !ax )
      return ListGen<T,LLGenB>( b )();
   else {
      return Result( new Pair( head(ax), 
             makeFun0( cat( bind1of1(tail,ax), b ) ) ) );
   }
}

#endif
