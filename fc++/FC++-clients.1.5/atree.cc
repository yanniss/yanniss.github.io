#include <iostream>
#include "prelude.h"

using namespace fcpp;

using std::cout;
using std::endl;

template <class T>
struct Tree {
   virtual ~Tree() {}
   virtual Fun1<T,void>& inorder( Fun1<T,void>& f ) const = 0;
};

template <class T>
class Node : public Tree<T> {
   T data_;
   Tree<T>* left_;
   Tree<T>* right_;
public:
   Node( T d, Tree<T>* l, Tree<T>* r ) : data_(d),left_(l),right_(r) {}
   Fun1<T,void>& inorder( Fun1<T,void>& f ) const {
      left_->inorder(f);
      f(data_);
      right_->inorder(f);
      return f;
   }
};
      
template <class T>
struct EmptyNode : public Tree<T> {
   Fun1<T,void>& inorder( Fun1<T,void>& f ) const {
      return f;
   }
};

struct Dump : public CFunType<int,void> {
   Dump() {}
   void operator()(int n) const { cout << n << ","; }
};

int main() {
//     1
//    / \      -
//   2   3
//  / \        -
// 4   5
   Tree<int>* NIL = new EmptyNode<int>;
   Tree<int>* t = new Node<int>(1,
      new Node<int>(2,new Node<int>(4,NIL,NIL),new Node<int>(5,NIL,NIL)),
      new Node<int>(3,NIL,NIL) );
   Dump d;
   Fun1<int,void> dd = makeFun1(d);
   t->inorder(dd);
   cout << endl;
   return 0;
}

