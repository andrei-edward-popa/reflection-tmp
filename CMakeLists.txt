cmake_minimum_required(VERSION 3.20)
project(Reflection)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER clang++-refl)

set(CMAKE_CXX_FLAGS
"-Ofast -Wall -Wextra -Werror -Wpedantic -Wdeprecated \
-Wconversion -Wshadow -Wnon-virtual-dtor -Wcast-align \
-Wpointer-arith -Wunused -Woverloaded-virtual \
-Wuninitialized -freflection-ts -freflection-ext -stdlib=libc++"
)

include_directories(components)

add_executable(app
        components/reflect/reflect.hpp
        components/reflect/reflect_utils.hpp
        components/reflect/reflect_data_member.hpp
        components/reflect/reflect_member_function.hpp
        components/reflect/reflect_operator.hpp
        components/reflect/reflect_constructor.hpp
        components/reflect/reflect_destructor.hpp
        components/json/json_utils.hpp
        components/json/json_value.hpp
        components/json/json.hpp
        main.cpp
)

