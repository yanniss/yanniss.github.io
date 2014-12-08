#include <iostream>
#include "prelude.h"
using std::endl;
using std::cout;
using namespace fcpp;

int main() {
   // These will die unless FCPP_DEBUG
   List<int> l;
   try {
      head(l);
   }
   catch( fcpp_exception& e ) {
      cout << e.what() << endl;
   }
   try {
      tail(l);
   }
   catch( fcpp_exception& e ) {
      cout << e.what() << endl;
   }

   OddList<int> m;
   try {
      head(m);
   }
   catch( fcpp_exception& e ) {
      cout << e.what() << endl;
   }
   try {
      tail(m);
   }
   catch( fcpp_exception& e ) {
      cout << e.what() << endl;
   }

   List<int> huge = enumFromTo(1,200000);
   length(huge);
   // likely to blow stack unless FCPP_SAFE_LIST
}

