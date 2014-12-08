#include <iostream>
#include <string>
#include <utility>

using std::string;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

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
   mutable Image* image;
   string fn;
   void cache() const { if(!image) image = new Image(fn); }
public:
   ImageProxy( string filename ) : image(0), fn(filename) {}
   Extent get_extent() const { cache(); return image->get_extent(); }
};

int main() {
   cout << "make images" << endl;
   Graphic *i  = new Image("g1");
   Graphic *ip = new ImageProxy("g2");
   cout << "find extents" << endl;
   i ->get_extent();
   ip->get_extent();
}
