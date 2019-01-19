// Autogenerated by MCQC
#pragma once
#include "nat.hpp"
#include "proc.hpp"
#include "show.hpp"
#include "variant.hpp"

using namespace Nat;
using namespace Proc;
using namespace Show;
using namespace Variant;
// Checks if type is callable by decltype magic
template<class T, class...Args>
struct is_callablish
{
    template<class U> static auto test(U*p) -> decltype((*p)(std::declval<Args>()...), void(), std::true_type());
    template<class U> static auto test(...) -> decltype(std::false_type());
    static constexpr auto value = decltype(test<T>(nullptr))::value;
};

// Function reference
template<class A, class B>
using arrow = B(&)(A&&);

template<class T>
struct Coq_nil {};
template<class T>
struct Coq_cons {
  T a;
  std::shared_ptr<std::variant<Coq_nil<T>, Coq_cons<T>>> b;
  Coq_cons(T a, std::shared_ptr<std::variant<Coq_nil<T>, Coq_cons<T>>> b) {
    this->a = a;
    this->b = b;
  };
};


template<class T>
using list = std::variant<Coq_nil<T>, Coq_cons<T>>;

template<class T>
std::shared_ptr<list<T>> coq_nil() {
  return std::make_shared<list<T>>(Coq_nil<T>());
}

template<class T>
std::shared_ptr<list<T>> coq_cons(T a, std::shared_ptr<list<T>> b) {
  return std::make_shared<list<T>>(Coq_cons<T>(a, b));
}


template<class T, class U, class V>
auto match(std::shared_ptr<list<T>> self, U f, V g) {
  return gmatch(self, [=](Coq_nil<T> _) { return f(); }, [=](Coq_cons<T> _) { return g(_.a, _.b); });
}


template<class T, class V, class R = std::invoke_result_t<V,T>>
std::shared_ptr<list<R>> map(V f, std::shared_ptr<list<T>> l) {
  return match(l,
    [=]() { return coq_nil<R>(); },
    [=](auto a, auto t) { return coq_cons<R>(f(a), map<T>(f, t)); });
}


int main() {
  print(show(map([=](auto x) { return add(x, x); }, coq_cons((nat)1, coq_cons((nat)2, coq_nil<nat>())))));
  return 0;
}


