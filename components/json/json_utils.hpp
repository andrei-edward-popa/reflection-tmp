#pragma once

#include <type_traits>

namespace JSON {

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
struct is_string : std::disjunction<
                   std::is_same<char *, typename std::decay_t<T>>,
                   std::is_same<const char *, typename std::decay_t<T>>,
                   std::is_same<std::string, typename std::decay_t<T>>
                   > {};

template<typename T>
constexpr bool is_string_v = is_string<T>::value;

template<typename T>
concept String = is_string_v<T>;

enum struct Type {
    String,
    Boolean,
    Number,
    Array,
    Object
};

}
