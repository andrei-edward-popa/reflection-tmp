#include <vector>

#include <json/json.hpp>
#include <reflect/reflect.hpp>

template<typename T>
struct S {
	std::string string_value;
	float floating_point_value;
	int integer_value;
	bool boolean_value;
    const int const_integer_value = 4;

    void member_function(const int&, const volatile float&&) {}
    void member_function2(std::string&) {}
    std::vector<int> member_function3(std::string&) {
        return {1, 2, 3};
    }
};

struct S2 {
	std::string string_value;
	float floating_point_value;
	int integer_value;
	bool boolean_value;
    const int const_integer_value = 4;

    void member_function(const int&, const volatile float&&) {}
    void member_function2(std::string&) {}
    std::vector<int> member_function3(std::string&) {
        return {1, 2, 3};
    }
};

void test_json() {
    JSON::Json root;
    JSON::Value arrval;
    arrval["test6"] = "thing";
    arrval["test9"] = "thing";
    root["test1"]["test2"] = 4.44;
    root["test1"]["test3"] = false;
    root["test1"]["test7"]["test8"] = false;
    root["test1"]["test7"]["test9"] = true;
    root["test1"]["test4"] = { arrval, arrval, arrval, arrval };
    root["test1"]["test10"] = { 4, 4, 4, 4 };
    root["test5"] = 10.045;
    root.print(std::cout);
}

void test_reflection() {
    JSON::Json root;
    root["reflect"] = std::vector<JSON::Value>{};
    wtf::print_struct<S<int>>(root["reflect"]);
    wtf::print_struct<std::vector<int>>(root["reflect"]);
    wtf::print_struct<std::string>(root["reflect"]);
    wtf::print_struct<S2>(root["reflect"]);
    root.print(std::cout);
}

int main() {
    // test_json();
    test_reflection();
    return 0;
}

