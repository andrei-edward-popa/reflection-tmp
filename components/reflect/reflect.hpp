#pragma once

#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <experimental/reflect>

#include <json/json.hpp>
#include <reflect/reflect_utils.hpp>
#include <reflect/reflect_data_member.hpp>

namespace refl = std::experimental::reflect;

namespace wtf {

template <typename Tp>
struct ReflectClass;

template <typename Tp, typename = wtf::underlying_type<Tp>>
void print_struct(JSON::Value& json) {
    using TpInfo = reflexpr(Tp);
    using AliasedTpInfo = refl::get_aliased_t<TpInfo>;
    std::string reflected_object_number;
    JSON::Value value;

    value["name"] = refl::get_display_name_v<AliasedTpInfo>;
    value["type"] = "class";
    value["templates"] = std::vector<JSON::Value>{};
    if constexpr (get_template_v<Tp>) {
        ReflectClass<get_template_t<Tp>>::printTemplateArguments(value["templates"]);
    }
    value["attributes"] = std::vector<JSON::Value>{};

    using DataMemberObjectSequence = refl::get_data_members_t<TpInfo>;
    using MemberFunctionObjectSequence = refl::get_member_functions_t<TpInfo>;
    using OperatorObjectSequence = refl::get_operators_t<TpInfo>;
    using ConstructorObjectSequence = refl::get_constructors_t<TpInfo>;
    using DestructorObjectSequence = refl::get_destructors_t<TpInfo>;

    ReflectDataMember<refl::unpack_sequence_t<std::tuple, DataMemberObjectSequence>>::print(value["attributes"]);
    ReflectClass<refl::unpack_sequence_t<std::tuple, MemberFunctionObjectSequence>>::printMemberFunction(value);
    ReflectClass<refl::unpack_sequence_t<std::tuple, OperatorObjectSequence>>::printMemberFunction(value);
    ReflectClass<refl::unpack_sequence_t<std::tuple, ConstructorObjectSequence>>::printNonReturnTypeMember(value);
    ReflectClass<refl::unpack_sequence_t<std::tuple, DestructorObjectSequence>>::printNonReturnTypeMember(value);

    json.array().push_back(value);
}

template <>
struct ReflectClass<std::tuple<>> {
    static void printMemberFunction(JSON::Value&) {}
    static void printNonReturnTypeMember(JSON::Value&) {}
    static void printParameters(JSON::Value&) {}
    static void printTemplateArguments(JSON::Value&) {}
    static void printMembersTemplateArguments(JSON::Value&) {}
};

template <typename Member, typename... Members>
struct ReflectClass<std::tuple<Member, Members...>> {
    static void printMemberFunction(JSON::Value& json_value) {
        using Parameters = refl::get_parameters_t<Member>;

        //printConstVolatileReference<Member>(json_value);
        //os << refl::get_name_v<Member>;

        ReflectClass<refl::unpack_sequence_t<std::tuple, Parameters>>::printParameters(json_value);
    
        ReflectClass<std::tuple<Members...>>::printMemberFunction(json_value);
    }

    static void printNonReturnTypeMember(JSON::Value& json_value) {
        using Parameters = refl::get_parameters_t<Member>;

        //os << refl::get_name_v<Member>;

        ReflectClass<refl::unpack_sequence_t<std::tuple, Parameters>>::printParameters(json_value);

        ReflectClass<std::tuple<Members...>>::printNonReturnTypeMember(json_value);
    }

    static void printParameters(JSON::Value& json_value) {
        //printConstVolatileReference<Member>(json_value);

        if constexpr (sizeof...(Members) != 0) {

        }

        ReflectClass<std::tuple<Members...>>::printParameters(json_value);
    }

    template<typename DataMember>
    static void printConstVolatileReference(JSON::Value& json_value) {
        using ParameterType = refl::get_reflected_type_t<refl::get_type_t<DataMember>>;
        JSON::Value value;

        value["name"] = refl::get_name_v<DataMember>;

        value["is_const"] = static_cast<bool>(std::is_const_v<std::remove_reference_t<ParameterType>>);
        value["is_volatile"] = static_cast<bool>(std::is_volatile_v<std::remove_reference_t<ParameterType>>);
        value["is_static"] = static_cast<bool>(refl::is_static_v<DataMember>);
        value["type"] = "";

        ReflectClass<std::tuple<ParameterType>>::printMembersTemplateArguments(value["type"]);

        value["is_lvalue_reference"] = static_cast<bool>(std::is_lvalue_reference_v<ParameterType>);
        value["is_rvalue_reference"] = static_cast<bool>(std::is_rvalue_reference_v<ParameterType>);

        json_value.array().push_back(value);
    }

    static void printTemplateArguments(JSON::Value& json_value, bool is_recursive = false) {
        //os << refl::get_display_name_v<refl::get_aliased_t<reflexpr(Member)>>;
        auto& json_array = json_value.array();

        if (!is_recursive) {
            json_array.push_back(refl::get_display_name_v<refl::get_aliased_t<reflexpr(Member)>>);
        } else {
            json_array[json_array.size() - 1].string() += refl::get_display_name_v<refl::get_aliased_t<reflexpr(Member)>>;
        }

        if constexpr (get_template_v<std::remove_cvref_t<Member>>) {
            json_array[json_array.size() - 1].string() += "<";
            ReflectClass<get_template_t<std::remove_cvref_t<Member>>>::printTemplateArguments(json_value, true);
            json_array[json_array.size() - 1].string() += ">";
        }

        if constexpr (sizeof...(Members) != 0) {

        }

        ReflectClass<std::tuple<Members...>>::printTemplateArguments(json_value);
    }

    static void printMembersTemplateArguments(JSON::Value& json_value) {
        auto& json_string = json_value.string();
        
        json_string += refl::get_display_name_v<refl::get_aliased_t<reflexpr(Member)>>;

        if constexpr (get_template_v<std::remove_cvref_t<Member>>) {
            json_string += "<";
            ReflectClass<get_template_t<std::remove_cvref_t<Member>>>::printMembersTemplateArguments(json_value);
            json_string += ">";
        }

        if constexpr (sizeof...(Members) != 0) {
            json_string += ", ";
        }

        ReflectClass<std::tuple<Members...>>::printMembersTemplateArguments(json_value);
    }
};

}
