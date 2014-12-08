#include <vector>
#include <iostream>
#define FCPP_ENABLE_LAMBDA
#include "prelude.h"
namespace TL = fcpp::fcpp_lambda;  // Typelists

#include <typeinfo>
using fcpp::IRef;
using fcpp::List;

//////////////////////////////////////////////////////////////////////
// Macros which are helpful to our clients

#define FUN1(functor,arg1t) \
   struct functor : public lcpp::Functor<functor,arg1t> { \
      static const char * const name; \
   } functor; \
   const char * const functor::name = #functor;
#define FUN2(functor,arg1t,arg2t) \
   struct functor : public lcpp::Functor<functor,arg1t,arg2t> { \
      static const char * const name; \
   } functor; \
   const char * const functor::name = #functor;
#define FUN3(functor,arg1t,arg2t,arg3t) \
   struct functor : public lcpp::Functor<functor,arg1t,arg2t,arg3t> { \
      static const char * const name; \
   } functor; \
   const char * const functor::name = #functor;

#define DECLARE(name,type,num) \
   typedef lcpp::LogicVariable<type,num> name ## _TYPE; \
   const name ## _TYPE& name = name ## _TYPE::instance( #name );

namespace lcpp {

namespace impl {

//////////////////////////////////////////////////////////////////////
// Set RTDEBUG to the name of a stream (like std::cout) to print 
// run-time debugging info (similar to prolog's trace command).  Every 
// run() method (and also ImplicationStorage::go()) are instrumented 
// with RTDEBUGn calls.

int indent = 0;
void RTDEBUG1( const char *s ) { 
   (void) s;
#ifdef RTDEBUG
   for(int i=0;i<indent;i++) 
      if( (i+1)%3==0 )
         RTDEBUG << "|"; 
      else
         RTDEBUG << " "; 
   RTDEBUG << "running " << s << std::endl; 
   indent++; 
#endif
}
void RTDEBUG3( const char *s ) { 
   (void) s;
#ifdef RTDEBUG
   for(int i=0;i<indent;i++) 
      if( (i+1)%3==0 )
         RTDEBUG << "|"; 
      else
         RTDEBUG << " "; 
   RTDEBUG << "        " << s << std::endl; 
#endif
}
template <class T>
const T& RTDEBUG2( const T& x ) {
#ifdef RTDEBUG
   indent--;
#endif
   return x;
}

//////////////////////////////////////////////////////////////////////
// Set DEBUGENV to a stream (like std::cout) to get debugging on
// environment creation.

void DEBUGENV1( const char* s ) {
   (void) s;
#ifdef DEBUGENV
   DEBUGENV << s << std::endl;
#endif
}

//////////////////////////////////////////////////////////////////////
// Inheritance tests

struct Funky {};    // "Functor"ness
struct HasLV {};    // Reps which may contain LogicVariables inside them
struct RepShape {}; // Reps which are Shapes

//////////////////////////////////////////////////////////////////////

struct DontCare;   // For _ (underscore)
struct Empty {        // For "unused" 2nd/3rd slots in a functor
   bool operator==( Empty ) const { return true; }
};

//////////////////////////////////////////////////////////////////////

template <class T>
struct HavingShapeAtom;
template <class T>
struct AtomImpl;
template <class F, class X, class Y=Empty, class Z=Empty>
struct HavingShapeFunctor;  // Term, Unifiable
template <class F, class X, class Y=Empty, class Z=Empty>
struct FunctorImpl; // HS, unify(), argN(), 
template <class F, class X, class Y=Empty, class Z=Empty>
struct Functor; // Funky, operator() for clients
struct Term;
template <class LHSS> struct Database;
template <class T> class AtomFacade;
template <class F, class X, class Y=Empty, class Z=Empty>
class FunctorFacade;
template <class Self, class A1, class A2, class A3, 
          class BB1, class BB2, class BB3> struct FunctorHelper;

//////////////////////////////////////////////////////////////////////
// Various useful type computers

template <class DS, bool is_funky>
struct LVHelper {
   typedef TL::NIL LVs;
};
template <class DS>
struct LVHelper<DS,true> {
   typedef typename DS::LVs LVs;
};
template <class DS>
struct LV {
   typedef typename LVHelper<DS,fcpp::Inherits<DS,HasLV>::value>::LVs LVs;
};

template <class DS, bool is_funky>
struct ForceHelper {
   typedef DS Type;
};
template <class DS>
struct ForceHelper<DS,true> {
   typedef typename DS::Shape Type;
};
template <class DS>
struct Force {
   typedef typename ForceHelper<DS,fcpp::Inherits<DS,Funky>::value>::Type Shape;
};

template <class T>
struct HavingShapeHelper { 
   typedef HavingShapeAtom<T> Type; 
   typedef AtomImpl<T> ImplType; 
   typedef AtomFacade<T> FacadeType; 
};
template <class F, class X, class Y, class Z>
struct HavingShapeHelper<Functor<F,X,Y,Z> > { 
   typedef HavingShapeFunctor<F,X,Y,Z> Type; 
   typedef FunctorImpl<F,X,Y,Z> ImplType; 
   typedef FunctorFacade<F,X,Y,Z> FacadeType; 
};
template <class DS>
struct HavingShape {
   typedef typename Force<DS>::Shape Shape;
   typedef typename HavingShapeHelper<Shape>::Type Type; 
   typedef typename HavingShapeHelper<Shape>::ImplType ImplType; 
   typedef typename HavingShapeHelper<Shape>::FacadeType FacadeType; 
};

template <class Rep, bool is_funky>
struct UnRepHelper {
   typedef Rep DS;
};
template <class Rep>
struct UnRepHelper<Rep,true> {
   typedef typename Rep::DS DS;
};
template <class Rep>
struct UnRep {
   typedef typename UnRepHelper<Rep,fcpp::Inherits<Rep,RepShape>::value>::DS DS;
};

template <class L> struct Environment;
namespace Rep {
   template <class Env, class Rep, int N> struct AddEnv;
}

template <class SomeRep>
struct RepToImpl {
   typedef IRef<Environment<TL::NIL> > IE;
   typedef typename Rep::AddEnv<IE,SomeRep,0>::Result IRef_of_Impl;
   typedef typename IRef_of_Impl::WrappedType Result;
};

template <class Q> struct QueryResultType;

//////////////////////////////////////////////////////////////////////
// Compile-time error checking

template <class A, class B> struct EnsureSameShapeHelper;
template <class A> struct EnsureSameShapeHelper<A,A> { static void go() {} };

template <class A, class B>
struct EnsureSameShape {
   typedef typename HavingShape<typename UnRep<A>::DS>::Shape AA;
   typedef typename HavingShape<typename UnRep<B>::DS>::Shape BB;
   static void go() { EnsureSameShapeHelper<AA,BB>::go(); }
};

template <bool b, class U> struct EnsureTermHelper;
template <class U> struct EnsureTermHelper<true,U> { static void go() {} };
template <class U> struct EnsureTermHelper<false,U> {};

template <class T>
struct EnsureTerm {
   typedef typename RepToImpl<T>::Result U;
   static void go() { EnsureTermHelper<fcpp::Inherits<U,Term>::value,U>::go(); }
};

template <bool b> struct EnsureFunctorHelper;
template <> struct EnsureFunctorHelper<true> { static void go() {} };

template <class Rep>
struct EnsureFunctor {
   typedef typename UnRep<Rep>::DS T;
   static void go() 
   { EnsureFunctorHelper<fcpp::Inherits<T,Funky>::value>::go(); }
};

//////////////////////////////////////////////////////////////////////

/* 
In the initial expression template tree, we use the FooRep classes to
represent LC++ structures.  Primitive types (like "int") and the
LogicVariable type are the only types whose name doesn't end in "Rep"
that should appear in such a structure.  The Reps live in their own
namespace to facilitate their constructors.  == and && and || and not()
and -= and Functor::operator() are the constructors for the Rep classes
that clients use to construct queries.

Later, we collect a whole LC++ term, create an environment, and call
addEnv() to transform the "Rep" into an "Impl".  That is, FooReps
become FooImpls, primitives (e.g. "int") become AtomImpls, and
LogicVariables become Bindings.  These Impl classes have methods like
unify() and run() and refer to each other via IRefs instead of member
variables.
*/

//////////////////////////////////////////////////////////////////////

// Forward decls of Impl classes
template <class T> class AtomImpl;
template <class S> class Binding;
template <class LHSS> class UnificationTermImpl;
class SucceedImpl;
class DisjunctImpl;
class ConjunctImpl;
class NotProvableImpl;
template <class LHSS> class ImplicationImpl;
template <class LHSS, class RHSS, class X, class Y> struct IsTermImpl;

// Forward decl of the magic "Expr" class used by LogicVariable::is()
template <class F, class X=Empty, class Y=Empty> struct Expr;
template <class F, class X=Empty, class Y=Empty> struct ExprResult;

namespace Rep {

template <class S, int i> struct LogicVariable;
template <class S, class A1, class A2=Empty, class A3=Empty> struct FunctorRep;
template <class LR, class FR> struct IsTermRep;
template <class LHS, class RHS> struct UnificationTermRep;
template <class LHS, class RHS> struct DisjunctRep;
template <class LHS, class RHS> struct ConjunctRep;
template <class X> struct NotProvableRep;
template <class LHS, class RHS> struct ImplicationRep;

template <class S>
struct DontCareLVRep : public RepShape {
   typedef S DS;
   typedef TL::NIL LVs;
   DontCareLVRep( const DontCare& ) {}
};
template <class S, int i> class LogicVariable : public HasLV, public RepShape {
public:
   typedef S DS;
   typedef TL::CONS<LogicVariable,TL::NIL> LVs;
// FIX THIS: private
   LogicVariable( const LogicVariable& x ) : name_(x.name_) {}
private:
   char *name_;
   LogicVariable( char *n ) : name_(n) {}
   // make all the Reps be friends so they can copy LVs
   template <class Se, class A1, class A2, class A3> friend struct FunctorRep;
   template <class LR, class FR> friend struct IsTermRep;
   template <class LHS, class RHS> friend struct UnificationTermRep;
   template <class LHS, class RHS> friend struct DisjunctRep;
   template <class LHS, class RHS> friend struct ConjunctRep;
   template <class X> friend struct NotProvableRep;
   template <class LHS, class RHS> friend struct ImplicationRep;
   template <class Self, class A1, class A2, class A3, 
             class BB1, class BB2, class BB3> friend struct FunctorHelper;
public:
   static LogicVariable instance( char* n ) {
      return LogicVariable(n);
   }
   char *name() const { return name_; }
   template <class F, class X, class Y>
   IsTermRep<LogicVariable<S,i>,typename ExprResult<F,X,Y>::Type>
   is( const F&, const X&, const Y& ) const;
   template <class F, class X>
   IsTermRep<LogicVariable<S,i>,typename ExprResult<F,X>::Type>
   is( const F&, const X& ) const;
   template <class F>
   IsTermRep<LogicVariable<S,i>,typename ExprResult<F>::Type>
   is( const F& ) const;
};
struct SucceedRep {};
struct FailRep {};
template <class LHS, class RHS> struct UnificationTermRep : public HasLV {
   typedef typename TL::AppendList<typename LV<LHS>::LVs,
                                   typename LV<RHS>::LVs>::Result LVs;
   LHS lhs;
   RHS rhs;
   UnificationTermRep( const LHS& l, const RHS& r ) : lhs(l), rhs(r) {}
};
template <class Self, class A1, class A2, class A3> 
struct FunctorRep : public HasLV, public RepShape {
   typedef Self DS;
   typedef typename TL::AppendList<typename LV<A1>::LVs,
           typename TL::AppendList<typename LV<A2>::LVs,
           typename LV<A3>::LVs>::Result>::Result LVs;
   A1 a1;
   A2 a2;
   A3 a3;
   FunctorRep( const A1& b1, const A2& b2, const A3& b3 )
   : a1(b1), a2(b2), a3(b3) {}
};
template <class LHS, class RHS> struct DisjunctRep : public HasLV {
   typedef typename TL::AppendList<typename LV<LHS>::LVs,
                                   typename LV<RHS>::LVs>::Result LVs;
   LHS lhs;
   RHS rhs;
   DisjunctRep( const LHS& l, const RHS& r ) : lhs(l), rhs(r) {}
};
template <class LHS, class RHS> struct ConjunctRep : public HasLV {
   typedef typename TL::AppendList<typename LV<LHS>::LVs,
                                   typename LV<RHS>::LVs>::Result LVs;
   LHS lhs;
   RHS rhs;
   ConjunctRep( const LHS& l, const RHS& r ) : lhs(l), rhs(r) {}
};
template <class X> struct NotProvableRep : public HasLV {
   typedef typename LV<X>::LVs LVs;
   X x;
   NotProvableRep( const X& xx ) : x(xx) {}
};
template <class LHS, class RHS> struct ImplicationRep : public HasLV {
   typedef typename TL::AppendList<typename LV<LHS>::LVs,
                                   typename LV<RHS>::LVs>::Result LVs;
   LHS lhs;
   RHS rhs;
   ImplicationRep( const LHS& l, const RHS& r ) : lhs(l), rhs(r) {}
};

template <class A, class B>
UnificationTermRep<A,B> operator==( const A& a, const B& b ) {
   EnsureSameShape<A,B>::go();
   return UnificationTermRep<A,B>(a,b);
}
template <class A, class B>
ConjunctRep<A,B> operator&&( const A& a, const B& b ) {
   EnsureTerm<A>::go();
   EnsureTerm<B>::go();
   return ConjunctRep<A,B>(a,b);
}
template <class A, class B>
DisjunctRep<A,B> operator||( const A& a, const B& b ) {
   EnsureTerm<A>::go();
   EnsureTerm<B>::go();
   return DisjunctRep<A,B>(a,b);
}
template <class A, class B>
ImplicationRep<A,B> operator-=( const A& a, const B& b ) {
   EnsureFunctor<A>::go();
   EnsureTerm<B>::go();
   return ImplicationRep<A,B>(a,b);
}
template <class X>
NotProvableRep<X> not_provable( const X& x ) {
   EnsureTerm<X>::go();
   return NotProvableRep<X>(x);
}

}  // end namespace Rep
using Rep::LogicVariable;

struct DontCare {
   template <class F, class X, class Y>
   Rep::IsTermRep<Rep::DontCareLVRep<typename ExprResult<F,X,Y>::RT>,
                                     typename ExprResult<F,X,Y>::Type>
   is( const F&, const X&, const Y& ) const;
   template <class F, class X>
   Rep::IsTermRep<Rep::DontCareLVRep<typename ExprResult<F,X>::RT>,
                                     typename ExprResult<F,X>::Type>
   is( const F&, const X& ) const;
   template <class F>
   Rep::IsTermRep<Rep::DontCareLVRep<typename ExprResult<F>::RT>,
                                     typename ExprResult<F>::Type>
   is( const F& ) const;
}; 
   
//////////////////////////////////////////////////////////////////////

// Note: can probably make this more efficient by inheriting "rest".
template <class S, int i, class T> 
class Environment<TL::CONS<LogicVariable<S,i>,T> > {
   typedef LogicVariable<S,i> H;
   mutable fcpp::RefCountType refC_;
   IRef<Binding<S> > b;
   char *name;
   Environment<T> rest;
public:
   void incref() const { ++refC_; }
   void decref() const { if (!--refC_) delete this; }
   Environment() : refC_(0), b( new Binding<S>(1) ), name("") {}

   void set_name_at( const H&, char* n ) { name = n; }
   template <class LV>
   void set_name_at( const LV& l, char* n ) { rest.set_name_at(l,n); }

   typename HavingShape<S>::FacadeType at( const H& ) { return b->facade(); }
   template <class LV>
   typename HavingShape<typename UnRep<LV>::DS>::FacadeType at( const LV& l ) 
   { return rest.at(l); }

   IRef<Binding<S> > basic_at( const H& ) { return b; }
   template <class LV>
   IRef<Binding<typename UnRep<LV>::DS> > basic_at( const LV& l ) 
   { return rest.basic_at(l); }

   void show();
};
template <>
struct Environment<TL::NIL> {
   mutable fcpp::RefCountType refC_;
public:
   void incref() const { ++refC_; }
   void decref() const { if (!--refC_) delete this; }
   void show() {}
};

template <class L>
IRef<Environment<L> > make_env() 
{ return IRef<Environment<L> >( new Environment<L> ); }

//////////////////////////////////////////////////////////////////////

struct UnRes {     // Unification Result
   bool ok;
   fcpp::Fun0<void> undo;
   UnRes( fcpp::Fun0<void> f ) : ok(true), undo(f) {}
   UnRes( bool b ) : ok(b), undo( fcpp::NoOp() ) {}
};

struct Term {
   mutable fcpp::RefCountType refC_;
   Term() : refC_(0) {}
   void incref() const { ++refC_; }
   void decref() const { if (!--refC_) delete this; }

   virtual List<Empty> run( IRef<Term> future ) = 0;  // RTDEBUG
   virtual ~Term() {}
};

template <class DS>
struct Unifiable {
   mutable fcpp::RefCountType refC_;
   Unifiable() : refC_(0) {}
   void incref() const { ++refC_; }
   void decref() const { if (!--refC_) delete this; }

   typedef typename HavingShape<DS>::Type HS;
   typedef typename HavingShape<DS>::ImplType HSI;
   virtual UnRes unify( IRef<HS> other ) = 0;
   // for double-dispatch
   virtual UnRes unify2( Binding<DS>* it ) = 0;
   virtual UnRes unify2( HSI* it ) = 0;

   virtual ~Unifiable() {}
};

//////////////////////////////////////////////////////////////////////

template <class T>
struct HavingShapeAtom : public Unifiable<T> { 
   typedef HavingShapeAtom Type;
   typedef AtomImpl<T> ImplType;
   virtual bool bound() = 0;
   virtual IRef<ImplType> value() = 0;
   virtual AtomFacade<T> facade() = 0;
};

template <class F, class X, class Y, class Z>
struct HavingShapeFunctor : public Term, public Unifiable<F> { 
   typedef typename HavingShape<F>::Type Type;
   typedef typename HavingShape<F>::ImplType ImplType;
   typedef X Arg1;
   typedef Y Arg2;
   typedef Z Arg3;
   using Term::incref;   // inherited from both parents
   using Term::decref;
   virtual bool bound() = 0;
   virtual IRef<ImplType> value() = 0;
   virtual FunctorFacade<F,X,Y,Z> facade() = 0;
};

//////////////////////////////////////////////////////////////////////

// Forward decl
template <class F>
IRef<Term> makeTermFromFunctoid( const F& f );

struct FailImpl : public Term {
   List<Empty> run( IRef<Term> ) {
      RTDEBUG1("FailImpl");
      return RTDEBUG2(fcpp::NIL);
   }
};
extern IRef<Term> it_fail;    // IRef<Term> "fail"

struct EndOfQuery : public Term {
   List<Empty> run( IRef<Term> ) {
      RTDEBUG1("EndOfQuery");
      return RTDEBUG2(fcpp::cons(Empty(),fcpp::NIL));
   }
};
extern IRef<Term> it_end_of_query;  // IRef<Term> "end_of_query"

template <class T> class AtomImpl : public HavingShapeAtom<T> {
   T x;
public:
   typedef typename HavingShape<T>::Type HS;
   typedef typename HavingShape<T>::ImplType HSI;
   AtomImpl( const T& xx ) : x(xx) {}
   UnRes unify( IRef<HS> other ) {
      return other->unify2(this);
   }
   UnRes unify2( HSI* other ) {
      return UnRes( x==other->x );
   }
   UnRes unify2( Binding<T>* other ) {
      return other->unify2(this);
   }
   bool bound() { return true; }
   IRef<HSI> value() { return IRef<HSI>(this); }
   T data() { return x; }
   AtomFacade<T> facade() { return AtomFacade<T>( this ); }
};

template <class DS> class Binding : public HavingShape<DS>::Type {
   typedef typename HavingShape<DS>::Type Super;
public:
   typedef typename HavingShape<DS>::Type HS;
private:
   using Super::incref;
   using Super::decref;

   bool from_lv;

   typedef typename HavingShape<DS>::ImplType HSI;
   IRef<Binding> next; 
   IRef<HSI> data; 
   // !next && !data  --> unbound
   // !next && data --> bound        next --> linked
   
   IRef<Binding> bottom() {
      IRef<Binding> tmp = IRef<Binding>(this);
      while( tmp->next )
         tmp = tmp->next;
      return tmp;
   } 
private:
   void unbind( bool f_lv ) { 
      (void) f_lv;
      //if( f_lv ) {
      //std::cout << "undoing a binding" << std::endl;
      //}
      data = IRef<HSI>(); 
      next = IRef<Binding>(); 
   }
   UnRes bind( IRef<Binding> r ) {
      if( bound() )
         throw "trying to bound already-bound ref!";
      //if( from_lv ) {
      //std::cout << "making a binding" << std::endl;
      //}
      IRef<Binding> tmp = bottom();
      tmp->next = r;
      fcpp::Fun0<void> fun = 
         fcpp::curry2(fcpp::ptr_to_fun(&Binding::unbind),tmp,from_lv);
      return UnRes( fun );
   }
   UnRes bind( IRef<HSI> r ) {
      if( bound() )
         throw "trying to bind already-bound Binding!";
      //if( from_lv ) {
      //std::cout << "making a binding" << std::endl;
      //}
      IRef<Binding> tmp = bottom();
      tmp->data = r;
      fcpp::Fun0<void> fun = 
         fcpp::curry2(fcpp::ptr_to_fun(&Binding::unbind),tmp,from_lv);
      return UnRes( fun );
   }
public:
   Binding() : from_lv(false), next(0), data(0) {}
   Binding(int) : from_lv(true), next(0), data(0) {}

   bool bound() {
      return bottom()->data;
   }
   UnRes unify( IRef<HS> other ) {
      return other->unify2(this);
   }
   UnRes unify2( Binding<DS>* other ) {
      if( !other->bound() ) {
         return other->bind( IRef<Binding<DS> >(this) );
      }
      else if( !bound() ) {
         return bind( IRef<Binding<DS> >(other) );
      }
      else {
         return bottom()->data->unify2( &*(other->bottom()->data) );
      }
   }
   UnRes unify2( HSI* other ) {
      if( !bound() ) {
         return bind( IRef<HSI>(other) );
      }
      else {
         return bottom()->data->unify2( other );
      }
   }

   // only applies if functor shape
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("binding");
      if( !bound() ) 
         throw "cannot use unbound LV as term in query";
      return RTDEBUG2(bottom()->data->run(future));
   }
   IRef<HSI> value() { 
      if( !bound() ) throw "unbound!!!"; 
      return bottom()->data; 
   }
   void* name() { return &*bottom(); }
   typename HavingShape<DS>::FacadeType facade() {
      typedef typename HavingShape<DS>::FacadeType FT;
      return FT( this );
   }
};

struct DummyTerm : public Term {
   List<Empty> run( IRef<Term> ) {
      throw "I should never get run";
   }
};

template <class LHSS>
struct UnificationTermImpl : public Term {
   IRef<LHSS> lhs, rhs;
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("UnificationTermImpl, with lhs and rhs types:");
      RTDEBUG3(typeid(*lhs).name());
      RTDEBUG3(typeid(*rhs).name());
      UnRes ur = lhs->unify(rhs);
      if( ur.ok ) {
// FIX THIS: check all run() logic; I think maybe fixed now, but needs
// testing
         using fcpp::cat;
         using fcpp::before;
         using fcpp::Const;
         using fcpp::OddList;
         return RTDEBUG2( cat( future->run( IRef<Term>(new DummyTerm) ),
            before(ur.undo,Const()(OddList<Empty>()))));
      }
      else
         return RTDEBUG2(fcpp::NIL);
   }
   UnificationTermImpl( IRef<LHSS> l, IRef<LHSS> r ) : lhs(l), rhs(r) {}
};

class SucceedImpl : public Term {
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("SucceedImpl");
      return RTDEBUG2(future->run( IRef<Term>(new DummyTerm) )); 
   }
};

struct DisjunctImpl : public Term {
   IRef<Term> lhs, rhs;
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("DisjunctImpl");
// FIX THIS: isn't lazy, or something.  I should probably be returning
// OddLists throughout, or otherwise delaying some calls.  Hmm.
      return RTDEBUG2(fcpp::cat( lhs->run(future), 
                        fcpp::curry2(fcpp::ptr_to_fun(&Term::run),rhs,future)));
   }
   DisjunctImpl( IRef<Term> l, IRef<Term> r ) : lhs(l), rhs(r) {}
};

struct ConjunctImpl : public Term {
   IRef<Term> lhs, rhs;
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("ConjunctImpl");
      future = IRef<Term>( new ConjunctImpl( rhs, future ) );
      return RTDEBUG2(lhs->run( future ));
   }
   ConjunctImpl( IRef<Term> l, IRef<Term> r ) : lhs(l), rhs(r) {}
};

template <class Self, class A1, class A2, class A3> 
class FunctorImpl : public HavingShapeFunctor<Self,A1,A2,A3> {
   IRef<Binding<A1> > a1;
   IRef<Binding<A2> > a2;
   IRef<Binding<A3> > a3;
public:
   typedef typename HavingShape<Self>::Type HS;
   typedef typename HavingShape<Self>::ImplType HSI;
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1(Self::name);
      return RTDEBUG2(Database<HS>::makeTerm( IRef<HS>(this))->run(future));
   }
   UnRes unify( IRef<HS> other ) {
      return other->unify2(this);
   }
   UnRes unify2( Binding<Self>* other ) {
      return other->unify2(this);
   }
   UnRes unify2( HSI* other ) {
      typedef IRef<typename HavingShape<A1>::Type> AA;
      typedef IRef<typename HavingShape<A2>::Type> BB;
      typedef IRef<typename HavingShape<A3>::Type> CC;
      UnRes x = other->a1->unify( AA(a1) );
      if( x.ok ) {
         UnRes y = other->a2->unify( BB(a2) );
         if( y.ok ) {
            UnRes z = other->a3->unify( CC(a3) );
            if( z.ok ) {
               return UnRes( fcpp::before(z.undo,
                                fcpp::before(y.undo,x.undo)) );
            }
            else {
               y.undo();
               x.undo();
            }
         }
         else {
            x.undo();
         }
      }
      return UnRes(false); 
   }
   typedef typename HavingShape<A1>::Type B1;
   typedef typename HavingShape<A2>::Type B2;
   typedef typename HavingShape<A3>::Type B3;
   FunctorImpl( IRef<B1> x, IRef<B2> y, IRef<B3> z ) 
   : a1( new Binding<A1> ), a2( new Binding<A2> ), a3( new Binding<A3> ) {
      a1->unify(x);
      a2->unify(y);
      a3->unify(z);
   }
   bool bound() { return true; }
   IRef<HSI> value() { return IRef<HSI>(this); }
   IRef<Binding<A1> > arg1() { return a1; }
   IRef<Binding<A2> > arg2() { return a2; }
   IRef<Binding<A3> > arg3() { return a3; }
   FunctorFacade<Self,A1,A2,A3> facade() { 
      return FunctorFacade<Self,A1,A2,A3>( this ); 
   }
};

struct NotProvableImpl : public Term {
   IRef<Term> x;
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("NotProvableImpl");
      List<Empty> l = x->run( it_end_of_query );
      if( l==fcpp::NIL )
         return RTDEBUG2(future->run( IRef<Term>(new DummyTerm) ));
      while(l)
         l = fcpp::tail(l);
      return RTDEBUG2(fcpp::NIL);
   }
   NotProvableImpl( IRef<Term> l ) : x(l) {}
};

template <class LHSS> class ImplicationImpl {
   mutable fcpp::RefCountType refC_;
public:
   void incref() const { ++refC_; }
   void decref() const { if (!--refC_) delete this; }
   IRef<LHSS> lhs;
   IRef<Term> rhs;
   ImplicationImpl( IRef<LHSS> l, IRef<Term> r ) : lhs(l), rhs(r) {}
};

//////////////////////////////////////////////////////////////////////

namespace Rep {

template <int N, class Env, class Rep>  // N explicit: call addEnv<N>(e,r)
typename AddEnv<Env,Rep,N>::Result addEnv( const Env& e, const Rep& r ) {
   return AddEnv<Env,Rep,N>::go(e,r);
}

// N is iNcoming next-avail-LV-#, O is Outgoing next-avail-LV-#
template <class Env, class Rep, int N>
struct AddEnv {
   static const int O = N;
   typedef IRef<HavingShapeAtom<Rep> > Result;
   static Result go( const Env&, const Rep& r ) {
      DEBUGENV1("AddEnv Atom");
      Result res = Result( new AtomImpl<Rep>(r) );
      DEBUGENV1("AddEnv Atom end");
      return res;
   }
};
template <class Env, class S, int N>
struct AddEnv<Env,DontCareLVRep<S>,N> {
   static const int O = N-1;
   typedef DontCareLVRep<S> Rep;
   typedef IRef<Binding<S> > Result;
   static Result go( const Env&, const Rep& ) {
      DEBUGENV1("AddEnv DontCareLVRep");
      Result res = Result( new Binding<S> );
      DEBUGENV1("AddEnv DontCareLVRep end");
      return res;
   }
};
template <class Env, int N>
struct AddEnv<Env,SucceedRep,N> {
   static const int O = N;
   typedef SucceedRep Rep;
   typedef IRef<SucceedImpl> Result;
   static Result go( const Env&, const Rep& ) {
      DEBUGENV1("AddEnv SucceedImpl");
      Result res = Result( new SucceedImpl );
      DEBUGENV1("AddEnv SucceedImpl end");
      return res;
   }
};
template <class Env, int N>
struct AddEnv<Env,FailRep,N> {
   static const int O = N;
   typedef FailRep Rep;
   typedef IRef<FailImpl> Result;
   static Result go( const Env&, const Rep& ) {
      DEBUGENV1("AddEnv FailImpl");
      Result res = Result( new FailImpl );
      DEBUGENV1("AddEnv FailImpl end");
      return res;
   }
};
template <class Env, class S, int i, int N>
struct AddEnv<Env,LogicVariable<S,i>,N > {
   static const int O = N;
   typedef LogicVariable<S,i> Rep;
   typedef IRef<Binding<S> > Result;
   static Result go( const Env& e, const Rep& r ) {
      DEBUGENV1("AddEnv LV");
      Result res = e->basic_at(r);
      e->set_name_at(r,r.name());
      DEBUGENV1("AddEnv LV end");
      return res;
   }
};
template <class Env, class LHSR, class RHSR, int N>
struct AddEnv<Env,UnificationTermRep<LHSR,RHSR>,N > {
   static const int tmp = AddEnv<Env,LHSR,N>::O;
   static const int O = AddEnv<Env,RHSR,tmp>::O;
   typedef UnificationTermRep<LHSR,RHSR> Rep;
   typedef IRef<Term> Result;
   typedef typename AddEnv<Env,LHSR,N>::Result::WrappedType LHSI;
   typedef typename LHSI::HS LHSS;
   static Result go( const Env& e, const Rep& r ) {
      DEBUGENV1("AddEnv UnificationTermImpl");
      Result res = Result( 
         new UnificationTermImpl<LHSS>( IRef<LHSS>( addEnv<N>(e,r.lhs) ),
                                        IRef<LHSS>( addEnv<tmp>(e,r.rhs) )) );
      DEBUGENV1("AddEnv UnificationTermImpl end");
      return res;
   }
};
template <class Env, class Self, class A1, class A2, class A3, int N> 
struct AddEnv<Env,FunctorRep<Self,A1,A2,A3>,N > {
   static const int t1 = AddEnv<Env,A1,N >::O;
   static const int t2 = AddEnv<Env,A2,t1>::O;
   static const int O  = AddEnv<Env,A3,t2>::O;
   typedef FunctorRep<Self,A1,A2,A3> Rep;
   typedef typename UnRep<A1>::DS AA1;
   typedef typename UnRep<A2>::DS AA2;
   typedef typename UnRep<A3>::DS AA3;
   typedef typename HavingShape<AA1>::Shape B1;
   typedef typename HavingShape<AA2>::Shape B2;
   typedef typename HavingShape<AA3>::Shape B3;
   typedef IRef<FunctorImpl<Self,AA1,AA2,AA3> > Result;
   static Result go( const Env& e, const Rep& r ) {
      DEBUGENV1("AddEnv FunctorImpl");
      IRef<typename HavingShape<typename UnRep<B1>::DS>::Type> 
         b1( addEnv<N>(e,r.a1) );
      IRef<typename HavingShape<typename UnRep<B2>::DS>::Type> 
         b2( addEnv<t1>(e,r.a2) );
      IRef<typename HavingShape<typename UnRep<B3>::DS>::Type> 
         b3( addEnv<t2>(e,r.a3) );
      Result res = Result( new FunctorImpl<Self,AA1,AA2,AA3>(b1,b2,b3) );
      DEBUGENV1("AddEnv FunctorImpl end");
      return res;
   }
};
template <class Env, class LHS, class RHS, int N>
struct AddEnv<Env,ConjunctRep<LHS,RHS>,N > {
   static const int tmp = AddEnv<Env,LHS,N>::O;
   static const int O = AddEnv<Env,RHS,tmp>::O;
   typedef ConjunctRep<LHS,RHS> Rep;
   typedef IRef<Term> Result;
   static Result go( const Env& e, const Rep& r ) {
      DEBUGENV1("AddEnv ConjunctImpl");
      Result res = Result( 
         new ConjunctImpl( IRef<Term>( addEnv<N>(e,r.lhs) ), 
                           IRef<Term>( addEnv<tmp>(e,r.rhs) ) ) );
      DEBUGENV1("AddEnv ConjunctImpl end");
      return res;
   }
};
template <class Env, class LHS, class RHS, int N>
struct AddEnv<Env,DisjunctRep<LHS,RHS>,N > {
   static const int tmp = AddEnv<Env,LHS,N>::O;
   static const int O = AddEnv<Env,RHS,tmp>::O;
   typedef DisjunctRep<LHS,RHS> Rep;
   typedef IRef<Term> Result;
   static Result go( const Env& e, const Rep& r ) {
      DEBUGENV1("AddEnv DisjunctImpl");
      Result res = Result( 
         new DisjunctImpl( IRef<Term>( addEnv<N>(e,r.lhs) ), 
                           IRef<Term>( addEnv<tmp>(e,r.rhs) ) ) );
      DEBUGENV1("AddEnv DisjunctImpl end");
      return res;
   }
};
template <class Env, class X, int N>
struct AddEnv<Env,NotProvableRep<X>,N > {
   static const int O = AddEnv<Env,X,N>::O;
   typedef NotProvableRep<X> Rep;
   typedef IRef<Term> Result;
   static Result go( const Env& e, const Rep& r ) {
      DEBUGENV1("AddEnv NotProvableRep");
      Result res = Result( 
         new NotProvableImpl( IRef<Term>( addEnv<N>(e,r.x) ) ) );
      DEBUGENV1("AddEnv NotProvableRep end");
      return res;
   }
};
template <class Env, class LHSR, class RHSR, int N>
struct AddEnv<Env,ImplicationRep<LHSR,RHSR>,N > {
   static const int tmp = AddEnv<Env,LHSR,N>::O;
   static const int O = AddEnv<Env,RHSR,tmp>::O;
   typedef ImplicationRep<LHSR,RHSR> Rep;
   typedef typename AddEnv<Env,LHSR,N>::Result::WrappedType LHSI;
   typedef typename LHSI::HS LHSS;
   typedef IRef<ImplicationImpl<LHSS> > Result;
   static Result go( const Env& e, const Rep& r ) {
      DEBUGENV1("AddEnv ImplicationImpl");
      Result res = Result( 
         new ImplicationImpl<LHSS>( IRef<LHSS>( addEnv<N>(e,r.lhs) ), 
                                    IRef<Term>( addEnv<tmp>(e,r.rhs) ) ) );
      DEBUGENV1("AddEnv ImplicationImpl end");
      return res;
   }
};

} // end namespace Rep

//////////////////////////////////////////////////////////////////////

template <class F>
class TermFromFunctoid : public Term {
   F fxn;
public:
   TermFromFunctoid( const F& ff ) : fxn(ff) {}
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("TermFromFunctoid");
      return RTDEBUG2(fxn(future));
   }
};
template <class F>
IRef<Term> makeTermFromFunctoid( const F& f ) {
   return IRef<Term>( new TermFromFunctoid<F>(f) );
}

template <class LHSS> 
struct ImplicationI {
   mutable fcpp::RefCountType refC_;
   ImplicationI() : refC_(0) {}
   void incref() const { ++refC_; }
   void decref() const { if (!--refC_) delete this; }
   virtual List<Empty> go( IRef<LHSS> lhs_val, IRef<Term> future ) = 0;
   virtual ~ImplicationI() {}
};

template <class F, class A1, class A2, class A3> 
struct Database<HavingShapeFunctor<F,A1,A2,A3> > {
   typedef HavingShapeFunctor<F,A1,A2,A3> HS;
   static std::vector<IRef<ImplicationI<HS> > > database;
   static void add( IRef<ImplicationI<HS> > r ) {
      database.push_back(r);
   }
   static IRef<Term> makeTerm( IRef<HS> lhs_val ) {
      typedef typename std::vector<IRef<ImplicationI<HS> > >::reverse_iterator 
         Iter;
      IRef<Term> t = it_fail;
      for( Iter i = database.rbegin(); i != database.rend(); ++i ) {
         IRef<Term> temp = makeTermFromFunctoid( fcpp::curry3( 
            fcpp::ptr_to_fun(&ImplicationI<HS>::go), *i, lhs_val ) );
         t = IRef<Term>( new DisjunctImpl( temp, t ) );
      }
      return t;
   }
};
template <class F, class A1, class A2, class A3> 
std::vector<IRef<ImplicationI<HavingShapeFunctor<F,A1,A2,A3> > > > 
   Database<HavingShapeFunctor<F,A1,A2,A3> >::database;

template <class LHSS, class LHS, class RHS>
class ImplicationStorage : public ImplicationI<LHSS> {
   Rep::ImplicationRep<LHS,RHS> rep;
public:
   ImplicationStorage( const Rep::ImplicationRep<LHS,RHS>& r ) : rep(r) {}
   List<Empty> go( IRef<LHSS> lhs_val, IRef<Term> future ) {
      typedef typename Rep::ImplicationRep<LHS,RHS> R;
      typedef typename QueryResultType<R>::IE IE;
      typedef typename QueryResultType<R>::SLVL SLVL;
      IE e = make_env<SLVL>();
      IRef<ImplicationImpl<LHSS> > ii = Rep::addEnv<-1>(e,rep);
      typedef UnificationTermImpl<LHSS> UT;
      IRef<Term> t( new UT( ii->lhs, lhs_val ) );
      t = IRef<Term>( new ConjunctImpl( t, ii->rhs ) );
      RTDEBUG1("impl storage go()");
      return RTDEBUG2(t->run(future));
   }
};

//////////////////////////////////////////////////////////////////////
// Meta-programming for static analysis stuff

template <int LV, int Count> struct Ctr {};

template <class CL, class E> struct Update;
template <int LV, int Count, class TCL, class T>
struct Update<TL::CONS<Ctr<LV,Count>,TCL>,LogicVariable<T,LV> > {
   typedef TL::CONS<Ctr<LV,Count+1>,TCL> Result;
};
template <class HCL, class TCL, class E> 
struct Update<TL::CONS<HCL,TCL>,E> {
   typedef TL::CONS<HCL,typename Update<TCL,E>::Result> Result;
};
template <class T, int LV> 
struct Update<TL::NIL,LogicVariable<T,LV> > {
   typedef TL::CONS<Ctr<LV,1>,TL::NIL> Result;
};

template <class L, class Acc> struct CountH;
template <class Acc> struct CountH<TL::NIL,Acc> {
   typedef Acc Result;
};
template <class H, class T, class Acc> struct CountH<TL::CONS<H,T>,Acc> {
   typedef typename CountH<T,typename Update<Acc,H>::Result>::Result Result;
};

template <class L> struct Count {
   typedef typename CountH<L,TL::NIL>::Result Result;
};

template <int LV>
struct LogicVar {
   static void UsedOnlyOnceIn_lassert_() {
      int x = 3;
   }
};

template <class L> struct Warn;
template <int LV, class T> 
struct Warn<TL::CONS<Ctr<LV,1>,T> > {
   Warn() {
      LogicVar<LV>::UsedOnlyOnceIn_lassert_();
   }
   Warn<T> x;
};
template <int LV, int Count, class T> 
struct Warn<TL::CONS<Ctr<LV,Count>,T> > {
   Warn<T> x;
};
template <> struct Warn<TL::NIL> {};

template <class LHS, class RHS>
void lassert( Rep::ImplicationRep<LHS,RHS> ir ) {
   // Static analysis (check for one-time-use vars)
   typedef typename LV<Rep::ImplicationRep<LHS,RHS> >::LVs LVs;
   typedef typename Count<LVs>::Result Counted;
   typedef Warn<Counted> W;
   W w;
   (void) w;

   // Do the work
   typedef typename HavingShape<typename UnRep<LHS>::DS>::Type HS;
   IRef<ImplicationI<HS> > r( new ImplicationStorage<HS,LHS,RHS>(ir) );
   Database<HS>::add( r );
}
template <class T>
void lassert( T x ) {
   lassert( x -= Rep::SucceedRep() );
}

//////////////////////////////////////////////////////////////////////

template <class X, class Y> struct IfDontCare { typedef X Result; };
template <class Y> 
struct IfDontCare<DontCare,Y> { typedef Rep::DontCareLVRep<Y> Result; };

template <class Self, class A1, class A2, class A3, 
          class BB1, class BB2, class BB3>
struct FunctorHelper {
   typedef typename IfDontCare<BB1,A1>::Result B1;
   typedef typename IfDontCare<BB2,A2>::Result B2;
   typedef typename IfDontCare<BB3,A3>::Result B3;
   typedef Rep::FunctorRep<Self,B1,B2,B3> Result;
   static Result go( const BB1& b1, const BB2& b2, const BB3& b3 ) {
      EnsureSameShape<A1,B1>::go();
      EnsureSameShape<A2,B2>::go();
      EnsureSameShape<A3,B3>::go();
      return Rep::FunctorRep<Self,B1,B2,B3>( B1(b1), B2(b2), B3(b3) );
   } 
};

template <class Self, class A1, class A2, class A3>
struct Functor : public Funky {
   typedef Functor Shape;
   typedef DontCare DC;
   typedef A1 Arg1;
   typedef A2 Arg2;
   typedef A3 Arg3;

   template <class B1, class B2, class B3>
   typename FunctorHelper<Self,A1,A2,A3,B1,B2,B3>::Result
   operator()( const B1& b1, const B2& b2, const B3& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,B1,B2,B3>::go(b1,b2,b3);
   }
   template <class B1, class B2>
   typename FunctorHelper<Self,A1,A2,A3,B1,B2,DC>::Result
   operator()( const B1& b1, const B2& b2, const DC& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,B1,B2,DC>::go(b1,b2,b3);
   }
   template <class B1, class B3>
   typename FunctorHelper<Self,A1,A2,A3,B1,DC,B3>::Result
   operator()( const B1& b1, const DC& b2, const B3& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,B1,DC,B3>::go(b1,b2,b3);
   }
   template <class B2, class B3>
   typename FunctorHelper<Self,A1,A2,A3,DC,B2,B3>::Result
   operator()( const DC& b1, const B2& b2, const B3& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,DC,B2,B3>::go(b1,b2,b3);
   }
   template <class B1>
   typename FunctorHelper<Self,A1,A2,A3,B1,DC,DC>::Result
   operator()( const B1& b1, const DC& b2, const DC& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,B1,DC,DC>::go(b1,b2,b3);
   }
   template <class B2>
   typename FunctorHelper<Self,A1,A2,A3,DC,B2,DC>::Result
   operator()( const DC& b1, const B2& b2, const DC& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,DC,B2,DC>::go(b1,b2,b3);
   }
   template <class B3>
   typename FunctorHelper<Self,A1,A2,A3,DC,DC,B3>::Result
   operator()( const DC& b1, const DC& b2, const B3& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,DC,DC,B3>::go(b1,b2,b3);
   }
   typename FunctorHelper<Self,A1,A2,A3,DC,DC,DC>::Result
   operator()( const DC& b1, const DC& b2, const DC& b3 ) {
      return FunctorHelper<Self,A1,A2,A3,DC,DC,DC>::go(b1,b2,b3);
   }
};
template <class Self, class A1, class A2>
struct Functor<Self,A1,A2,Empty> : public Funky {
   typedef Functor Shape;
   typedef DontCare DC;
   typedef Empty EM;
   typedef A1 Arg1;
   typedef A2 Arg2;
   typedef Empty Arg3;

   template <class B1, class B2>
   typename FunctorHelper<Self,A1,A2,EM,B1,B2,EM>::Result
   operator()( const B1& b1, const B2& b2 ) {
      return FunctorHelper<Self,A1,A2,EM,B1,B2,EM>::go(b1,b2,Empty());
   }
   template <class B1>
   typename FunctorHelper<Self,A1,A2,EM,B1,DC,EM>::Result
   operator()( const B1& b1, const DC& b2 ) {
      return FunctorHelper<Self,A1,A2,EM,B1,DC,EM>::go(b1,b2,Empty());
   }
   template <class B2>
   typename FunctorHelper<Self,A1,A2,EM,DC,B2,EM>::Result
   operator()( const DC& b1, const B2& b2 ) {
      return FunctorHelper<Self,A1,A2,EM,DC,B2,EM>::go(b1,b2,Empty());
   }
   typename FunctorHelper<Self,A1,A2,EM,DC,DC,EM>::Result
   operator()( const DC& b1, const DC& b2 ) {
      return FunctorHelper<Self,A1,A2,EM,DC,DC,EM>::go(b1,b2,Empty());
   }
};
template <class Self, class A1>
struct Functor<Self,A1,Empty,Empty> : public Funky {
   typedef Functor Shape;
   typedef DontCare DC;
   typedef Empty EM;
   typedef A1 Arg1;
   typedef Empty Arg2;
   typedef Empty Arg3;

   template <class B1>
   typename FunctorHelper<Self,A1,EM,EM,B1,EM,EM>::Result
   operator()( const B1& b1 ) {
      return FunctorHelper<Self,A1,EM,EM,B1,EM,EM>::go(b1,Empty(),Empty());
   }
   typename FunctorHelper<Self,A1,EM,EM,DC,EM,EM>::Result
   operator()( const DC& b1 ) {
      return FunctorHelper<Self,A1,EM,EM,DC,EM,EM>::go(b1,Empty(),Empty());
   }
};

//////////////////////////////////////////////////////////////////////
// Meta-programming for sorting/unique-ing a list of LVs that appear in
// a term.

template <bool b, class T, class F> struct IfThen { typedef T Result; };
template <class T, class F> struct IfThen<false,T,F> { typedef F Result; };

struct ErrorAllLVTypesMustHaveDistinctIntegers {};

template <class LV, class SLVL> struct InsertInOrder;
template <class LV> struct InsertInOrder<LV,TL::NIL> {
   typedef TL::CONS<LV,TL::NIL> Result;
};
template <class U, int j, class T, int i, class Rest> 
struct InsertInOrder<LogicVariable<U,j>,TL::CONS<LogicVariable<T,i>,Rest> > {
   typedef typename IfThen<(j<i),
      TL::CONS<LogicVariable<U,j>,TL::CONS<LogicVariable<T,i>,Rest> >,
      typename IfThen<(j>i), 
         TL::CONS<LogicVariable<T,i>,
                  typename InsertInOrder<LogicVariable<U,j>,Rest>::Result>,
         ErrorAllLVTypesMustHaveDistinctIntegers>::Result>::Result Result;
};

template <class LVL, class SLVL> struct SortLVsHelper;
template <class SLVL> 
struct SortLVsHelper<TL::NIL,SLVL> { typedef SLVL Result; };
template <class T, int i, class Rest, class SLVL> 
struct SortLVsHelper<TL::CONS<LogicVariable<T,i>,Rest>,SLVL> { 
   typedef typename InsertInOrder<LogicVariable<T,i>,SLVL>::Result Temp;
   typedef typename SortLVsHelper<Rest,Temp>::Result Result;
};

template <class LVL>
struct SortLVs {
   typedef typename SortLVsHelper<LVL,TL::NIL>::Result Result;
};

template <class LVL>
struct QRTH {  // Query Result Type Helper (from Logic Var List)
   typedef typename SortLVs<LVL>::Result SLVL;
   typedef IRef<Environment<SLVL> > IE;   // IRef<Environment>
   typedef std::pair<IE,List<Empty> > Result;
};

// QRT gives users an easy way to get type of environment.
template <class A, class B=int, class C=int, class D=int, class E=int>
struct QRT {
   typedef TL::CONS<A,TL::CONS<B,TL::CONS<C,TL::CONS<D,
      TL::CONS<E,TL::NIL> > > > > L;
   typedef typename QRTH<L>::IE IE;
   typedef typename QRTH<L>::Result Result;
};
template <class A, class B, class C, class D>
struct QRT<A,B,C,D,int> {
   typedef TL::CONS<A,TL::CONS<B,TL::CONS<C,TL::CONS<D,TL::NIL> > > > L;
   typedef typename QRTH<L>::IE IE;
   typedef typename QRTH<L>::Result Result;
};
template <class A, class B, class C>
struct QRT<A,B,C,int,int> {
   typedef TL::CONS<A,TL::CONS<B,TL::CONS<C,TL::NIL> > > L;
   typedef typename QRTH<L>::IE IE;
   typedef typename QRTH<L>::Result Result;
};
template <class A, class B>
struct QRT<A,B,int,int,int> {
   typedef TL::CONS<A,TL::CONS<B,TL::NIL> > L;
   typedef typename QRTH<L>::IE IE;
   typedef typename QRTH<L>::Result Result;
};
template <class A>
struct QRT<A,int,int,int,int> {
   typedef TL::CONS<A,TL::NIL> L;
   typedef typename QRTH<L>::IE IE;
   typedef typename QRTH<L>::Result Result;
};

// FIX THIS: the RemoveDuplicates in TL doesn't work for some unknown
// reason, so I use my own version here.  See if TL::RemoveDuplicates is
// broken for FC++ some time.
template <class L> struct RemoveDuplicates;

template <class L, bool ok> struct RemoveDuplicatesHelp;
template <class H, class T> struct RemoveDuplicatesHelp<TL::CONS<H,T>,true> 
{ typedef TL::CONS<H,typename RemoveDuplicates<T>::Result> Result; };
template <class H, class T> struct RemoveDuplicatesHelp<TL::CONS<H,T>,false> 
{ typedef typename RemoveDuplicates<T>::Result Result; };

template <class L> struct RemoveDuplicates;
template <> struct RemoveDuplicates<TL::NIL> { typedef TL::NIL Result; };
template <class H, class T> struct RemoveDuplicates<TL::CONS<H,T> > 
{ typedef typename RemoveDuplicatesHelp<TL::CONS<H,T>,
     !(TL::Contains<T,H>::value)>::Result Result; };

template <class Q>
struct QueryResultType {
   typedef typename Q::LVs LL;
   typedef typename RemoveDuplicates<LL>::Result L;
   typedef typename QRTH<L>::SLVL SLVL;
   typedef typename QRTH<L>::IE IE;
   typedef typename QRTH<L>::Result Result;
};

//////////////////////////////////////////////////////////////////////

template <class R>
typename QueryResultType<R>::Result query( const R& someTermRep ) {
   typedef typename QueryResultType<R>::IE IE;
   typedef typename QueryResultType<R>::SLVL SLVL;
   IE e = make_env<SLVL>();
   typename Rep::AddEnv<IE,R,-1>::Result t = Rep::addEnv<-1>( e, someTermRep );
   List<Empty> l = 
      fcpp::curry2( fcpp::ptr_to_fun(&Term::run), t, it_end_of_query );
   return std::make_pair( e, l );
}

template <class R>
List<typename QueryResultType<R>::IE> lquery( const R& someTermRep ) {
   typedef typename QueryResultType<R>::IE IE;
   std::pair<IE,List<Empty> > p = query(someTermRep);
   return fcpp::curry2(fcpp::map,fcpp::ignore(fcpp::const_(p.first)),p.second);
}

template <class S, int i, class T>
void Environment<TL::CONS<LogicVariable<S,i>,T> >::show() {
   const LogicVariable<S,i>& lv = LogicVariable<S,i>::instance(0);
   //std::cout << " - LV #" << i << " (" << typeid(S).name() << ") \t= ";
   std::cout << " - " << this->name << "\t= ";
   std::cout << this->at( lv ) << std::endl;
   this->rest.show();
}

template <class R>
void iquery( const R& someTermRep ) {
   typedef typename QueryResultType<R>::IE IE;
   std::pair<IE,List<Empty> > p = query(someTermRep);
   int i = 1;
   while( p.second ) {
      std::cout << "Result #" << i << std::endl;
      ++i;
      p.first->show();
      p.second = fcpp::tail( p.second );
   }
}

//////////////////////////////////////////////////////////////////////
// Expr and is() support

// In order to store a value in an LC++ object, the value needs to 
// support == (for unification tests).  Functoids don't, so we wrap them.
template <class Functoid>
struct Opaque {
   typedef Functoid F;
   F f;
   Opaque( const F& ff ) : f(ff) {}
   bool operator==( const Opaque& ) const { return false; }
};
template <class Functoid>
Opaque<Functoid> opaque( const Functoid& f ) {
   return Opaque<Functoid>(f);
}

template <class F, class X, class Y>
struct Expr : public Functor<Expr<F,X,Y>,F,X,Y> {
   static const char * const name;
};
template <class F, class X, class Y>
const char * const Expr<F,X,Y>::name = "expr";

template <class F, class X, class Y>
struct ExprResult {
   typedef typename UnRep<Opaque<F> >::DS FS;
   typedef typename FS::F FF;
   typedef typename UnRep<X>::DS XS;
   typedef typename UnRep<Y>::DS YS;
   typedef Rep::FunctorRep<Expr<FS,XS,YS>,Opaque<F>,X,Y> Type;
   typedef typename fcpp::RT<FF,XS,YS>::ResultType RT;
};
template <class F, class X>
struct ExprResult<F,X,Empty> {
   typedef typename UnRep<Opaque<F> >::DS FS;
   typedef typename FS::F FF;
   typedef typename UnRep<X>::DS XS;
   typedef Rep::FunctorRep<Expr<FS,XS>,Opaque<F>,X> Type;
   typedef typename fcpp::RT<FF,XS>::ResultType RT;
};
template <class F>
struct ExprResult<F,Empty,Empty> {
   typedef typename UnRep<Opaque<F> >::DS FS;
   typedef typename FS::F FF;
   typedef Rep::FunctorRep<Expr<FS>,Opaque<F> > Type;
   typedef typename fcpp::RT<FF>::ResultType RT;
};

template <class F, class X, class Y>
typename ExprResult<F,X,Y>::Type
expr( const F& f, const X& x, const Y& y ) {
   typedef typename UnRep<Opaque<F> >::DS FS;
   typedef typename UnRep<X>::DS XS;
   typedef typename UnRep<Y>::DS YS;
   return Expr<FS,XS,YS>()(opaque(f),x,y);
}
template <class F, class X>
typename ExprResult<F,X>::Type
expr( const F& f, const X& x ) {
   typedef typename UnRep<Opaque<F> >::DS FS;
   typedef typename UnRep<X>::DS XS;
   return Expr<FS,XS,Empty>()(opaque(f),x);
}
template <class F>
typename ExprResult<F>::Type
expr( const F& f ) {
   typedef typename UnRep<Opaque<F> >::DS FS;
   return Expr<FS,Empty,Empty>()(opaque(f));
}

namespace Rep {
   template <class LR, class FR>
   struct IsTermRep : public HasLV {
      typedef LR LHS;
      typedef FR RHS;
      typedef typename TL::AppendList<typename LV<LHS>::LVs,
                                      typename LV<RHS>::LVs>::Result LVs;
      LHS lhs;
      RHS rhs;
      IsTermRep( const LHS& l, const RHS& r ) : lhs(l), rhs(r) {}
   };
   template <class Env, class LR, class FR, int N>
   struct AddEnv<Env,IsTermRep<LR,FR>,N > {
      typedef LR LHSR;
      typedef FR RHSR;
      static const int tmp = AddEnv<Env,LHSR,N>::O;
      static const int O = AddEnv<Env,RHSR,tmp>::O;
      typedef IsTermRep<LR,FR> Rep;
      typedef IRef<Term> Result;
      typedef typename AddEnv<Env,LHSR,N>::Result::WrappedType LHSI;
      typedef typename LHSI::HS LHSS;
      typedef typename HavingShape<typename FR::DS>::Type RHSS;
      static Result go( const Env& e, const Rep& r ) {
         DEBUGENV1("AddEnv IsTermImpl");
         typedef typename FR::DS::Arg2 X;
         typedef typename FR::DS::Arg3 Y;
         Result res = Result( 
            new IsTermImpl<LHSS,RHSS,X,Y>( IRef<LHSS>( addEnv<N>(e,r.lhs) ),
                                         IRef<RHSS>( addEnv<tmp>(e,r.rhs) )) );
         DEBUGENV1("AddEnv IsTermImpl end");
         return res;
      }
   };
}

template <class LHSS, class RHSS, class NeedlessX, class NeedlessY>
struct IsTermImpl : public Term {
   IRef<LHSS> b; 
   IRef<RHSS> efxy;
   IsTermImpl( const IRef<LHSS>& l, const IRef<RHSS>& r ) : b(l), efxy(r) {}
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("IsTermImpl");
      if( !efxy->bound() )      
         return RTDEBUG2(fcpp::NIL);
      if( !efxy->value()->arg1()->bound() )      
         return RTDEBUG2(fcpp::NIL);
      if( !efxy->value()->arg2()->bound() )      
         return RTDEBUG2(fcpp::NIL);
      if( !efxy->value()->arg3()->bound() )      
         return RTDEBUG2(fcpp::NIL);
      typedef typename fcpp::RT<typename RHSS::Arg1::F,typename
                       RHSS::Arg2,typename RHSS::Arg3>::ResultType R;
      R r = efxy->value()->arg1()->value()->data().f(
               efxy->value()->arg2()->value()->data(),
               efxy->value()->arg3()->value()->data() );
      IRef<HavingShapeAtom<R> > ir( new AtomImpl<R>(r) );
      // Note: one of few cases where we don't need to create a Term 
      // on the heap in an IRef.  We just want to call .run() is all.
      UnificationTermImpl<LHSS> uti( b, ir );
      return RTDEBUG2(uti.run(future));
   }
};

template <class LHSS, class RHSS, class NeedlessX>
struct IsTermImpl<LHSS,RHSS,NeedlessX,Empty> : public Term {
   IRef<LHSS> b; 
   IRef<RHSS> efx;
   IsTermImpl( const IRef<LHSS>& l, const IRef<RHSS>& r ) : b(l), efx(r) {}
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("IsTermImpl");
      if( !efx->bound() )      
         return RTDEBUG2(fcpp::NIL);
      if( !efx->value()->arg1()->bound() )      
         return RTDEBUG2(fcpp::NIL);
      if( !efx->value()->arg2()->bound() )      
         return RTDEBUG2(fcpp::NIL);
      typedef typename fcpp::RT<typename RHSS::Arg1::F,typename
                       RHSS::Arg2>::ResultType R;
      R r = efx->value()->arg1()->value()->data().f(
               efx->value()->arg2()->value()->data() );
      IRef<HavingShapeAtom<R> > ir( new AtomImpl<R>(r) );
      // Note: one of few cases where we don't need to create a Term 
      // on the heap in an IRef.  We just want to call .run() is all.
      UnificationTermImpl<LHSS> uti( b, ir );
      return RTDEBUG2(uti.run(future));
   }
};

template <class LHSS, class RHSS>
struct IsTermImpl<LHSS,RHSS,Empty,Empty> : public Term {
   IRef<LHSS> b; 
   IRef<RHSS> ef;
   IsTermImpl( const IRef<LHSS>& l, const IRef<RHSS>& r ) : b(l), ef(r) {}
   List<Empty> run( IRef<Term> future ) {
      RTDEBUG1("IsTermImpl");
      if( !ef->bound() )      
         return RTDEBUG2(fcpp::NIL);
      if( !ef->value()->arg1()->bound() )      
         return RTDEBUG2(fcpp::NIL);
      typedef typename fcpp::RT<typename RHSS::Arg1::F>::ResultType R;
      R r = ef->value()->arg1()->value()->data().f();
      IRef<HavingShapeAtom<R> > ir( new AtomImpl<R>(r) );
      // Note: one of few cases where we don't need to create a Term 
      // on the heap in an IRef.  We just want to call .run() is all.
      UnificationTermImpl<LHSS> uti( b, ir );
      return RTDEBUG2(uti.run(future));
   }
};

//////////////////////////////////////////////////////////////////////
// is()

namespace Rep {
   template <class S, int i>
   template <class F, class X, class Y>
   IsTermRep<LogicVariable<S,i>,typename ExprResult<F,X,Y>::Type> 
   LogicVariable<S,i>::is( const F& f, const X& x, const Y& y ) const {
      return IsTermRep<LogicVariable<S,i>,
                       typename ExprResult<F,X,Y>::Type>( *this, expr(f,x,y) );
   }
   template <class S, int i>
   template <class F, class X>
   IsTermRep<LogicVariable<S,i>,typename ExprResult<F,X>::Type> 
   LogicVariable<S,i>::is( const F& f, const X& x ) const {
      return IsTermRep<LogicVariable<S,i>,
                       typename ExprResult<F,X>::Type>( *this, expr(f,x) );
   }
   template <class S, int i>
   template <class F>
   IsTermRep<LogicVariable<S,i>,typename ExprResult<F>::Type> 
   LogicVariable<S,i>::is( const F& f ) const {
      return IsTermRep<LogicVariable<S,i>,
                       typename ExprResult<F>::Type>( *this, expr(f) );
   }
}
template <class F, class X, class Y>
Rep::IsTermRep<Rep::DontCareLVRep<typename ExprResult<F,X,Y>::RT>,
               typename ExprResult<F,X,Y>::Type> 
DontCare::is( const F& f, const X& x, const Y& y ) const {
   typedef Rep::DontCareLVRep<typename ExprResult<F,X,Y>::RT> DCLVR;
   return Rep::IsTermRep<DCLVR,typename ExprResult<F,X,Y>::Type>
          ( DCLVR(*this), expr(f,x,y) );
}
template <class F, class X>
Rep::IsTermRep<Rep::DontCareLVRep<typename ExprResult<F,X>::RT>,
               typename ExprResult<F,X>::Type> 
DontCare::is( const F& f, const X& x ) const {
   typedef Rep::DontCareLVRep<typename ExprResult<F,X>::RT> DCLVR;
   return Rep::IsTermRep<DCLVR,typename ExprResult<F,X>::Type>
          ( DCLVR(*this), expr(f,x) );
}
template <class F>
Rep::IsTermRep<Rep::DontCareLVRep<typename ExprResult<F>::RT>,
               typename ExprResult<F>::Type> 
DontCare::is( const F& f ) const {
   typedef Rep::DontCareLVRep<typename ExprResult<F>::RT> DCLVR;
   return Rep::IsTermRep<DCLVR,typename ExprResult<F>::Type>
          ( DCLVR(*this), expr(f) );
}

//////////////////////////////////////////////////////////////////////
// Facades

template <class T>
class AtomFacade {
   IRef< HavingShapeAtom<T> > x;
public:
   AtomFacade( HavingShapeAtom<T>* p ) : x(p) {}
   operator bool() const { return x->bound(); }
   T operator*()   const { return x->value()->data(); }
   void* name()    const { return dynamic_cast<Binding<T>*>( &*x )->name(); }
};
template <class T>
std::ostream& operator<<( std::ostream& o, const AtomFacade<T>& af ) {
   if( af )
      return o << *af;
   else
      return o << "_" << af.name();
}

template <class F, class X, class Y=Empty, class Z=Empty>
class FunctorValue {
   IRef< FunctorImpl<F,X,Y,Z> > x;
public:
   FunctorValue( FunctorImpl<F,X,Y,Z>* p ) : x(p) {}
   typename HavingShape<X>::FacadeType arg1() { return x->arg1()->facade(); }
   typename HavingShape<Y>::FacadeType arg2() { return x->arg2()->facade(); }
   typename HavingShape<Z>::FacadeType arg3() { return x->arg3()->facade(); }
};

template <class T>
struct OperatorRightArrowProxy {
   T x;
   OperatorRightArrowProxy( const T& xx ) : x(xx) {}
   T* operator->() { return &x; }
};

template <class F, class X, class Y, class Z>
class FunctorFacade {
   IRef< HavingShapeFunctor<F,X,Y,Z> > x;
public:
   FunctorFacade( HavingShapeFunctor<F,X,Y,Z>* p ) : x(p) {}
   operator bool() const { return x->bound(); }
   FunctorValue<F,X,Y,Z> operator*() const   { return &*x->value(); }
   OperatorRightArrowProxy< FunctorValue<F,X,Y,Z> > operator->() const { 
      return FunctorValue<F,X,Y,Z>( &*x->value() ); 
   }
   void* name() const { 
      //return dynamic_cast<Binding<Functor<F,X,Y,Z> >*>( &*x )->name(); 
      return dynamic_cast<Binding<F>*>( &*x )->name(); 
   }
};
template <class F, class X, class Y, class Z>
std::ostream& operator<<( std::ostream& o, const FunctorFacade<F,X,Y,Z>& ff ) {
   if( ff )
      return o << F::name << "(" 
         << ff->arg1() << ","
         << ff->arg2() << ","
         << ff->arg3() << ")";
   else
      return o << "_" << ff.name();
}
template <class F, class X, class Y>
std::ostream& operator<<( std::ostream& o, const FunctorFacade<F,X,Y>& ff ) {
   if( ff )
      return o << F::name << "(" 
         << ff->arg1() << ","
         << ff->arg2() << ")";
   else
      return o << "_" << ff.name();
}
template <class F, class X>
std::ostream& operator<<( std::ostream& o, const FunctorFacade<F,X>& ff ) {
   if( ff )
      return o << F::name << "(" 
         << ff->arg1() << ")";
   else
      return o << "_" << ff.name();
}

//////////////////////////////////////////////////////////////////////

// FIX THIS (per .h file)
IRef<Term> it_fail( new FailImpl );
IRef<Term> it_end_of_query( new EndOfQuery );

DontCare _;
Rep::FailRep fail;
Rep::SucceedRep succeed;

} // end namespace impl

//////////////////////////////////////////////////////////////////////
// Here are all the names just visible in the "lcpp" namespace.

// types
using impl::Empty;
using impl::Environment;
using impl::Functor;
using impl::Rep::LogicVariable;
using impl::QRT;
using impl::AtomFacade;
using impl::FunctorValue;
using impl::FunctorFacade;

// values
using impl::_;
using impl::fail;
using impl::succeed;

// functions (which would probably be found with Koenig lookup even if
// we didn't export the names here)
using impl::Rep::not_provable;
using impl::lassert;
using impl::query;
using impl::iquery;
using impl::lquery;
// Note that stuff like the operator overloads in Rep are not explicitly
// exported; those are the only other things that clients can use which
// don't explicitly appear here, I think.

} // end namespace lcpp 

