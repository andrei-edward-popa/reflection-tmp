#pragma once

#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <experimental/reflect>

#include <json/json.hpp>
#include <reflect/reflect_utils.hpp>

namespace refl = std::experimental::reflect;

namespace wtf {

template <typename Tp>
struct ReflectDataMember;

template <>
struct ReflectDataMember<std::tuple<>> {
    static void print(JSON::Value&) {}
    static void printDataMember(JSON::Value&) {}
    static void printTemplate(JSON::Value&) {}
};

template <typename Member, typename... Members>
struct ReflectDataMember<std::tuple<Member, Members...>> {
    static void print(JSON::Value& json_value) {
        printDataMember<Member>(json_value);
        ReflectDataMember<std::tuple<Members...>>::print(json_value);
    }

    template<typename DataMember>
    static void printDataMember(JSON::Value& json_value) {
    	JSON::Value value;
        using ParameterType = refl::get_reflected_type_t<refl::get_type_t<DataMember>>;

		value["type"] = "";
        value["name"] = refl::get_name_v<DataMember>;
        value["is_static"] = static_cast<bool>(refl::is_static_v<DataMember>);
        value["is_lvalue_reference"] = static_cast<bool>(std::is_lvalue_reference_v<ParameterType>);
        value["is_rvalue_reference"] = static_cast<bool>(std::is_rvalue_reference_v<ParameterType>);
        value["is_const"] = static_cast<bool>(std::is_const_v<std::remove_reference_t<ParameterType>>);
        value["is_volatile"] = static_cast<bool>(std::is_volatile_v<std::remove_reference_t<ParameterType>>);

        ReflectDataMember<std::tuple<ParameterType>>::printTemplate(value["type"]);

        json_value.array().push_back(value);
    }

    static void printTemplate(JSON::Value& json_value) {
        std::string &json_string = json_value.string();
        json_string += refl::get_display_name_v<refl::get_aliased_t<reflexpr(Member)>>;

        if constexpr (get_template_v<std::remove_cvref_t<Member>>) {
            json_string += "<";
            ReflectDataMember<get_template_t<std::remove_cvref_t<Member>>>::printTemplate(json_value);
            json_string += ">";
        }
        if constexpr (sizeof...(Members) != 0) {
            json_string += ", ";
        }

        ReflectDataMember<std::tuple<Members...>>::printTemplate(json_value);
    }
};

}
