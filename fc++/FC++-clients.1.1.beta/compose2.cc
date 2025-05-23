#include "prelude.h"

template<class F, class G>
class AddHelper {
  F f;
  G g;
public:
  AddHelper(const F& an_f, const G& a_g) : f(an_f), g(a_g) {}

  template <class I>
  struct Sig : public FunType<I, typename F::Sig<I>::ResultType> {};

  template <class I>
  typename F::template Sig<I>::ResultType operator()( const I& i ) const {
    return f(i)+g(i);
  }
};


struct Add {
  template <class F, class G>
  struct Sig: public FunType<F,G,AddHelper<F,G> > {};

  template <class F, class G>
  AddHelper<F,G> operator() (F f, G g) const { return AddHelper<F,G>(f,g); }
} add;


int main() {
  cout << add(curry2(Plus(),3), curry2(Plus(), 6))(7) << endl;
  Compose2 my_compose2;
  cout << my_compose2(Plus(), curry2(Plus(),3), curry2(Plus(), 6))(7) << endl;
  Fun2<int,int,int> f = makeFun2(monomorphize2<int,int,int>(Plus()));
  cout << f(3,5) << endl;
  
  // Test automatic currying.
  cout << (f(3))(5) << endl;

  // Really test automatic currying! f is treated as a function of 1 argument
  // (in which case it returns a function of one argument). When it is
  // curried explicitly, it becomes a 0-argument function returning a 1-arg
  // function.
  Fun0<Fun1<int,int> > g = makeFun0(curry1(f, 5));
  cout << g()(3) << endl;
}
