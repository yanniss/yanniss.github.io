#include <iostream>
#include <utility>
#include "prelude.h"

using namespace fcpp;

using std::pair;
using std::ostream;
using std::cout;
using std::endl;

ostream& operator<<( ostream& o, pair<int,int> p ) {
   o << "(" << p.first << "," << p.second << ")";
   return o;
}

template <class T>
ostream& operator<<( ostream& o, OddList<T> l ) {
   while( l ) {
      o << head(l) << " ";
      l = tail(l);
   }
   return o;
}

template <class T>
ostream& operator<<( ostream& o, List<T> l ) {
   return (o << l.force());
}

struct Double : public CFunType<int,int> {
   int operator()( int x ) const {
      return 2*x;
   }
};

int main() {
   using fcpp::plus;
   using fcpp::greater;

   List<int> l = cat( list_with(0,2,5,7,9), list_with(11,15,18) );
   cout << "Original   :" << l << endl;
   List<List<int> > ll;
   ll = cons( l, ll );
   ll = cons( l, ll );
   cout << "Concat     :" << curry(concat,ll)() << endl;
   cout << "Init       :" << curry(init,l)() << endl;
   cout << "Last       :" << curry(last,l)() << endl;
   cout << "Length     :" << curry(length,l)() << endl;
   cout << "Reverse    :" << curry(fcpp::reverse,l)() << endl;
   cout << "TakeWhile<9:" << takeWhile(fcpp::less(_,9))(l) << endl;
   cout << "DropWhile<9:" << dropWhile(fcpp::less(_,9))(l) << endl;
   cout << "Cycle (121):" 
#ifdef FCPP_SIMPLE_PRELUDE
        << take(3, curry( cycle, cons(1,cons(2,NIL)).delay() )() ) << endl;
#else
        << take(3, curry( cycle, cons(1,cons(2,NIL)) )() ) << endl;
#endif
   cout << "At(2)      :" << at(l)(2) << endl;
   cout << "Filter >10 :" << filter( bind2of2( greater, 10 ))(l) << endl;
   cout << "Foldr +    :" << foldr(plus,0)(l) << endl;
   cout << "Foldr1 +   :" << foldr1(plus)(l) << endl;
   cout << "Foldl +    :" << foldl(plus,0)(l) << endl;
   cout << "Foldl1 +   :" << foldl1(plus)(l) << endl;
   cout << "Scanr +    :" << scanr(plus,0)(l) << endl;
   cout << "Scanr1 +   :" << scanr1(plus)(l) << endl;
   cout << "Scanl +    :" << scanl(plus,0)(l) << endl;
   cout << "Scanl1 +   :" << scanl1(plus)(l) << endl;
   cout << "Drop 5     :" << drop(5)(l) << endl;

   cout << "Take 5 of Iterate double 1:" 
        << take(5,iterate(Double())(1)) << endl;
   cout << "Take 5 of repeat 1        :" 
        << take(5, curry(repeat,1)() ) << endl;

   pair<List<int>,List<int> > p = splitAt( 5)( l );
   cout << "SplitAt 5 a:" << p.first << endl;
   cout << "SplitAt 5 b:" << p.second << endl;

   p = span( bind2of2( greater, 10 ))( l );
   cout << "Span >10  a:" << p.first << endl;
   cout << "Span >10  b:" << p.second << endl;

   p = curry2( Break(), bind2of2(greater,10) )( l );
   cout << "Break>10  a:" << p.first << endl;
   cout << "Break>10  b:" << p.second << endl;
   List<bool> b = map( Odd(), l );
   cout << "And Odd    :" << And()(b) << endl;
   cout << "Or  Odd    :" << Or()(b) << endl;
   cout << "And Odd 0  :" << And()(take(1,b)) << endl;
   cout << "Or  Odd 0  :" << Or()(take(1,b)) << endl;
   cout << "Any >10    :" << any( bind2of2(greater,10))( l) << endl;
   cout << "All >10    :" << all( bind2of2(greater,10))( l) << endl;
   cout << "Any >-10   :" << any( bind2of2(greater,-10))( l) << endl;
   cout << "All >-10   :" << all( bind2of2(greater,-10))( l) << endl;
   cout << "Elem 10    :" << elem(10)(l) << endl;
   cout << "NotElem 10 :" << notElem(10)(l) << endl;
   cout << "Sum        :" << curry(sum,l)() << endl;
   cout << "Producttail:" << curry(product,tail(l))() << endl;
   cout << "Maximum    :" << curry(maximum,l)() << endl;
   cout << "Minimum    :" << curry(minimum,l)() << endl;
   cout << "Zip w/ 1s  :" << zip(l)(repeat(1)) << endl;
   cout << "Zip w/ +1s :" << zipWith(plus,l)(repeat(1)) << endl;
   cout << "gcd(30,18) :" << gcd(30)(18) << endl;

   return 0;
}
   
