#include <iostream>
#include <string>
#include <utility>
#include "prelude.h"

using namespace fcpp;

using std::make_pair;
using std::pair;
using std::string;
using std::cout;
using std::endl;

template <class T, class A1> 
struct Make1 : public CFunType<A1,T*> {
   T* operator()(A1 x) const { return new T(x); }
};

template <class T>
class LazyPtrProxy {
   Fun0<T*> f;
   mutable T* p;
   void cache() const { if(!p) p = f(); }
public:
   LazyPtrProxy( const Fun0<T*> ff ) : f(ff), p(0) {}
   template <class F> LazyPtrProxy( F ff ) : f(makeFun0(ff)), p(0) {}
   T& operator*() const { cache(); return *p; }
   T* operator->() const { cache(); return p; }
};

typedef pair<int,int> Extent;

class Graphic {
public:
   virtual Extent get_extent() const =0;
   virtual ~Graphic() {}
};

class Image : public Graphic {
public:
   Image( string filename )  { cout << "Opening file: " << filename << endl; }
   Extent get_extent() const { return make_pair(40,30); }
};

class ImageProxy : public Graphic {
   LazyPtrProxy<Image> image;
public:
   ImageProxy( string filename ) 
      : image(curry(Make1<Image,string>(),filename)) {}
   Extent get_extent() const { return image->get_extent(); }
};

int main() {
   cout << "make images" << endl;
   Graphic *i  = new Image("g1");
   Graphic *ip = new ImageProxy("g2");
   cout << "find extents" << endl;
   i ->get_extent();
   ip->get_extent();
}
