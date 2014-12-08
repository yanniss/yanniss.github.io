//
// Copyright (c) 2000 Brian McNamara and Yannis Smaragdakis
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is granted without fee,
// provided that the above copyright notice and this permission notice
// appear in all source code copies and supporting documentation. The
// software is provided "as is" without any express or implied
// warranty.

#ifndef FCPP_REF_DOT_H
#define FCPP_REF_DOT_H

#ifdef FCPP_USE_NAMESPACE  
// g++ chokes for some reason
namespace fcpp {
#endif

// Ref<T> can be used in place of T* most anywhere.  Ref<T>s are garbage
// collected (using reference-couting); they are used in place of
// pointers throughout the library to get automatic memory management.
// It is notable that the library parts that use Ref (the indirect functoids
// in function.h and the list in list.h) do not allow the creation of
// "circular" (self-referencing) data structures.

// Ref<T> should work exactly as T*, except that instead of dynamic_cast
// you must use ref_dynamic_cast.


// This is a helper; it will probably be in next version of the C++ standard
template<class T, class U>
T implicit_cast( const U& x ) {
   return x;
}

// Here's the Ref class
template<class T>
class Ref;

template <class U, class T>
Ref<U> ref_dynamic_cast( const Ref<T>& r );

template<class T>
class Ref {
protected:
   T* ptr;
   unsigned int* count;
   
   void new_ref() { count = new unsigned int(1); }
   void inc()     { ++(*count); }
   bool dec()     { return 0 == --(*count); }

   template <class U> friend class Ref;

   template <class U, class V>
   friend Ref<U> ref_dynamic_cast( const Ref<V>& r );

public:
   typedef T WrappedType;

   explicit Ref(T* p=0) : ptr(p), count(0) {
      if(ptr) new_ref();
   }
   Ref(const Ref<T>& other) : ptr(other.ptr), count(0) {
      if(ptr) { count = other.count; inc(); }
   }
   ~Ref() {
      if (ptr && dec()) { delete ptr; delete count; }
   }
   Ref<T>& operator=(const Ref<T>& other) {
      T* tp = other.ptr;
      unsigned int* tc = other.count;
      if( tp ) { ++(*tc); }
      if (ptr && dec()) { delete ptr; delete count; }
      ptr = tp;
      count = tc;
      return *this;
   }

// FIX THIS: need const version too
   operator T* () const  { return ptr; }
   T* operator->() const { return ptr; }

   template <class U>
   Ref(const Ref<U>& other) 
   : ptr(implicit_cast<T*>(other.ptr)), count(0) {
      if(ptr) { count = other.count; inc(); }
   }
};

// dynamic_cast; can't overload the operator (why?!?) so we create our own

template <class U, class T>
Ref<U> ref_dynamic_cast( const Ref<T>& r ) {
   Ref<U> temp;
   temp.ptr = dynamic_cast<U*>( r.ptr );
   if( temp.ptr ) { temp.count = r.count; temp.inc(); }
   return temp;
}

#ifdef FCPP_USE_NAMESPACE  
} // end namespace fcpp 
#endif

#endif
