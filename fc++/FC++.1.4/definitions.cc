#ifdef FCPP_THIS_IS_NEVER_DEFINED
echo '#include "prelude.h"'
echo '#undef FCPP_MAYBE_INIT'
echo '#define FCPP_MAYBE_INIT(x) x'
echo 'namespace fcpp {'
for FILE in *.h
do
   echo "// from $FILE"
   cat $FILE | grep ^FCPP_MAYBE_EXTERN | cut -c18-
done
echo '}'
exit
#endif

#include "prelude.h"
#undef FCPP_MAYBE_INIT
#define FCPP_MAYBE_INIT(x) x
namespace fcpp {
// from config.h
// from curry.h
 AutoCurryType _;   // this is a legal identifier as fcpp::_
 Const const_;   // C++ keyword, so add trailing underscore
 Bind1of1 bind1of1;
 Bind1of2 bind1of2;
 Bind2of2 bind2of2;
 Bind1and2of2 bind1and2of2;
 Bind1and2and3of3 bind1and2and3of3; 
 Bind1and2of3 bind1and2of3;
 Bind2and3of3 bind2and3of3;
 Bind1and3of3 bind1and3of3;
 Bind1of3 _bind1of3;
 Curryable2<Bind1of3> bind1of3 FCPP_MAYBE_INIT((_bind1of3));
 Bind2of3 _bind2of3;
 Curryable2<Bind2of3> bind2of3 FCPP_MAYBE_INIT((_bind2of3));
 Bind3of3 _bind3of3;
 Curryable2<Bind3of3> bind3of3 FCPP_MAYBE_INIT((_bind3of3));
 Curry3 curry3; 
 Curry2 curry2;
 Curry1 curry1, curry; // "curry" is the same as "curry1"
// from function.h
// from list.h
 Head head;
 Tail tail; 
 Null null;
 Cons _cons; 
 Curryable2<Cons> cons FCPP_MAYBE_INIT((_cons));
 Cat _cat;
 Curryable2<Cat> cat FCPP_MAYBE_INIT((_cat));
 Delay delay;
 Force force;
// from operator.h
 MakePair _makePair;
 Curryable2<MakePair> makePair FCPP_MAYBE_INIT((_makePair));
 Min _min;
 Curryable2<Min> min FCPP_MAYBE_INIT((_min));
 Max _max;
 Curryable2<Max> max FCPP_MAYBE_INIT((_max));
 Plus _plus;
 Curryable2<Plus> plus FCPP_MAYBE_INIT((_plus));
 Minus _minus;
 Curryable2<Minus> minus FCPP_MAYBE_INIT((_minus));
 Multiplies _multiplies;
 Curryable2<Multiplies> multiplies FCPP_MAYBE_INIT((_multiplies));
 Divides _divides;
 Curryable2<Divides> divides FCPP_MAYBE_INIT((_divides));
 Modulus _modulus;
 Curryable2<Modulus> modulus FCPP_MAYBE_INIT((_modulus));
 Negate negate;
 Equal _equal;
 Curryable2<Equal> equal FCPP_MAYBE_INIT((_equal));
 NotEqual _notEqual;
 Curryable2<NotEqual> notEqual FCPP_MAYBE_INIT((_notEqual));
 Greater _greater;
 Curryable2<Greater> greater FCPP_MAYBE_INIT((_greater));
 Less _less;
 Curryable2<Less> less FCPP_MAYBE_INIT((_less));
 GreaterEqual _greaterEqual;
 Curryable2<GreaterEqual> greaterEqual FCPP_MAYBE_INIT((_greaterEqual));
 LessEqual _lessEqual;
 Curryable2<LessEqual> lessEqual FCPP_MAYBE_INIT((_lessEqual));
 LogicalAnd _logicalAnd;
 Curryable2<LogicalAnd> logicalAnd FCPP_MAYBE_INIT((_logicalAnd));
 LogicalOr _logicalOr;
 Curryable2<LogicalOr> logicalOr FCPP_MAYBE_INIT((_logicalOr));
 LogicalNot logicalNot;
 Dereference dereference;
 AddressOf addressOf;
 Inc inc; 
 Always1 always1;
 Never1 never1;
// from prelude.h
 Id id;
 Compose compose_;
 Curryable2<Compose> compose FCPP_MAYBE_INIT((compose_));
 Compose2 _compose2;
 Curryable3<Compose2> compose2 FCPP_MAYBE_INIT((_compose2));
 Until _until;
 Curryable3<Until> until FCPP_MAYBE_INIT((_until));
 Last last;
 Init init;
 Length length;
 At _at;
 Curryable2<At> at FCPP_MAYBE_INIT((_at));
 Filter _filter;
 Curryable2<Filter> filter FCPP_MAYBE_INIT((_filter));
 Concat concat;
 Foldr _foldr;
 Curryable3<Foldr> foldr FCPP_MAYBE_INIT((_foldr));
 Foldr1 _foldr1;
 Curryable2<Foldr1> foldr1 FCPP_MAYBE_INIT((_foldr1));
 Foldl _foldl;
 Curryable3<Foldl> foldl FCPP_MAYBE_INIT((_foldl));
 Foldl1 _foldl1;
 Curryable2<Foldl1> foldl1 FCPP_MAYBE_INIT((_foldl1));
 Scanr _scanr;
 Curryable3<Scanr> scanr FCPP_MAYBE_INIT((_scanr));
 Scanr1 _scanr1;
 Curryable2<Scanr1> scanr1 FCPP_MAYBE_INIT((_scanr1));
 Scanl _scanl;
 Curryable3<Scanl> scanl FCPP_MAYBE_INIT((_scanl));
 Scanl1 _scanl1;
 Curryable2<Scanl1> scanl1 FCPP_MAYBE_INIT((_scanl1));
 Iterate _iterate;
 Curryable2<Iterate> iterate FCPP_MAYBE_INIT((_iterate));
 Repeat repeat;
 Map _map;
 Curryable2<Map> map FCPP_MAYBE_INIT((_map));
 Take _take;
 Curryable2<Take> take FCPP_MAYBE_INIT((_take));
 Drop _drop;
 Curryable2<Drop> drop FCPP_MAYBE_INIT((_drop));
 TakeWhile _takeWhile; 
 Curryable2<TakeWhile> takeWhile FCPP_MAYBE_INIT((_takeWhile));
 DropWhile _dropWhile;
 Curryable2<DropWhile> dropWhile FCPP_MAYBE_INIT((_dropWhile));
 Replicate _replicate; 
 Curryable2<Replicate> replicate FCPP_MAYBE_INIT((_replicate));
 Cycle cycle; 
 SplitAt _splitAt; 
 Curryable2<SplitAt> splitAt FCPP_MAYBE_INIT((_splitAt));
 Span _span; 
 Curryable2<Span> span FCPP_MAYBE_INIT((_span));
 Break _break_;
 Curryable2<Break> break_ FCPP_MAYBE_INIT((_break_));   
 Flip flip;
 Reverse reverse;
 And and_;
 Or or_;
 All _all;
 Curryable2<All> all FCPP_MAYBE_INIT((_all));
 Any _any; 
 Curryable2<Any> any FCPP_MAYBE_INIT((_any));
 Elem _elem; 
 Curryable2<Elem> elem FCPP_MAYBE_INIT((_elem));
 NotElem _notElem; 
 Curryable2<NotElem> notElem FCPP_MAYBE_INIT((_notElem));
 Sum sum;
 Product product;
 Minimum minimum; 
 Maximum maximum; 
 ZipWith _zipWith; 
 Curryable3<ZipWith> zipWith FCPP_MAYBE_INIT((_zipWith));
 Zip _zip;
 Curryable2<Zip> zip FCPP_MAYBE_INIT((_zip));
 Fst fst;
 Snd snd;
 Unzip unzip;
 Gcd _gcd; 
 Curryable2<Gcd> gcd FCPP_MAYBE_INIT((_gcd));
 Odd odd; 
 Even even; 
 EnumFrom enumFrom; 
 EnumFromTo _enumFromTo; 
 Curryable2<EnumFromTo> enumFromTo FCPP_MAYBE_INIT((_enumFromTo));
 ListUntil _listUntil;
 Curryable3<ListUntil> listUntil FCPP_MAYBE_INIT((_listUntil));
 NoOp noOp;
 Before before_;
 Curryable2<Before> before FCPP_MAYBE_INIT((before_));
 After after_;
 Curryable2<After> after FCPP_MAYBE_INIT((after_));
 Uncurry uncurry;
 Duplicate duplicate;
 Ignore ignore;
// from ref_count.h
// from reuse.h
 AUniqueTypeForNil NIL;
// from signature.h
}
