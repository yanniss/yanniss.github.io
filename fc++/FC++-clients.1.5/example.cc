#include <iostream>
using std::cout; using std::endl;

#include <algorithm>
using std::transform; using std::copy;

#include <string>
using std::string;

#include <iterator>
using std::ostream_iterator;

#include <functional>
using std::bind1st;

#include "prelude.h"
using fcpp::_;

// This example shows currying and type-inference; fcpp::plus is like
// plus, except that it can infer it argument types and you don't
// need calls like bind1st for partial application (currying is automatic).
int main() {
   string a[] = { "man", "bowl", "ego" };
   string prefix = "super";

#ifdef FCPP_WAY
   transform( a, a+3, a, fcpp::plus(prefix) );
#else // STL way
   transform( a, a+3, a, bind1st(std::plus<string>(),prefix) );
#endif

   copy( a, a+3, ostream_iterator<string>(cout,"\n") );
}

