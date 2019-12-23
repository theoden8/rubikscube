#pragma once

#include <type_traits>
#include <utility>
#include <tuple>
#include <functional>

template <typename... Ts> void unroll(Ts...){}

namespace Tuple {
template <
    size_t Index = 0, // start iteration at 0 index
    typename TTuple,  // the tuple type
    size_t Size = std::tuple_size<std::remove_reference_t<TTuple>>::value,
    typename TCallable, // the callable to bo invoked for each tuple item
    typename... TArgs   // other arguments to be passed to the callable
>
void for_each(TTuple&& tpl, TCallable&& callable, TArgs&&... args) {
  if constexpr(Index < Size) {
    std::invoke(callable, args..., std::get<Index>(tpl));
    if constexpr(Index + 1 < Size) {
      for_each<Index + 1>(
        std::forward<TTuple>(tpl),
        std::forward<TCallable>(callable),
        std::forward<TArgs>(args)...
      );
    }
  }
}

template <typename TupleT, typename ElementT>
int lfind(TupleT &&tpl, ElementT &&value) {
  int ind = -1;
  int i = -1;
  Tuple::for_each(tpl, [&](auto &a) mutable -> void {
    ++i;
    if(ind != -1)return;
    if constexpr(std::is_same_v<
      std::remove_const_t<std::remove_reference_t<decltype(a)>>,
      std::remove_const_t<std::remove_reference_t<ElementT>>
      >)
    {
      if(a == value) {
        ind = i;
      }
    }
  });
  return ind;
}

} // namespace Tuple
