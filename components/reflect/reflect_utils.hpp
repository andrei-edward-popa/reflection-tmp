#pragma once

#include <tuple>
#include <type_traits>

namespace wtf {

template <typename>
struct get_template : std::false_type {};

template <template <typename...> typename Tp, typename ... Args>
struct get_template<Tp<Args...>> : std::true_type {
    using type = std::tuple<Args...>;
};

template <typename Tp>
using get_template_t = typename get_template<Tp>::type;

template <typename Tp>
constexpr bool get_template_v = get_template<Tp>::value;

template<typename T>
using underlying_type = decltype(T{});

}
