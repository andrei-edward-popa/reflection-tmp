#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <unordered_map>

#include <json/json_utils.hpp>

namespace JSON {

struct Value {
    template<String StringTp>
    Value(const StringTp& string) noexcept : type_(Type::String) {
        data_.string = string;
    }

    template<String StringTp>
    Value(StringTp&& string) noexcept : type_(Type::String) {
        data_.string = std::forward<StringTp>(string);
    }

    template<Arithmetic ArithmeticTp>
    Value(const ArithmeticTp number) noexcept : type_(Type::Number) {
        data_.number = number;
    }

    Value(const bool boolean) noexcept : type_(Type::Boolean) {
        data_.boolean = boolean;
    }

    Value(const std::vector<Value>& array) noexcept : type_(Type::Array) {
        std::copy(array.begin(), array.end(), data_.array.begin());
    }

    Value(std::vector<Value>&& array) noexcept : type_(Type::Array) {
        data_.array = std::move(array);
    }

    Value(std::initializer_list<Value>&& array) noexcept : type_(Type::Array) {
        data_.array = array;
    }

    Value() noexcept : type_(Type::Object) {}

    Value& operator[](const std::string& str) noexcept {
        if (data_.object.bucket_count() < reserve_) {
            data_.object.reserve(reserve_);
        }
        if (!data_.object.contains(str)) {
        	data_.object.emplace(std::piecewise_construct,
				std::forward_as_tuple(str),
				std::forward_as_tuple());
        }
        return data_.object[str];
    }

    void assertType(const Type other_type) const {
        if (this->type_ != other_type) {
            throw std::invalid_argument("Type is not correct!");
        }
    }

    const Type& type() const noexcept {
        return type_;
    }

    Arithmetic auto& number() {
        return data_.number;
    }

    String auto& string() {
        return data_.string;
    }

    bool& boolean() {
        return data_.boolean;
    }

    std::vector<Value>& array() {
        return data_.array;
    }

    std::unordered_map<std::string, Value>& object() {
        return data_.object;
    }

    void print(std::ostream& os, char ch, uint64_t spaces, const std::string& key = "") const noexcept {
        os << std::string(spaces, ' ');
        if (!key.empty()) {
            os << "\"" << key << "\": ";
        }
        if (type_ == Type::Number) {
            os << data_.number << ch << "\n";
        } else if (type_ == Type::String) {
            os << "\"" << data_.string << "\"" << ch << "\n";
        } else if (type_ == Type::Boolean) {
            os << std::boolalpha << data_.boolean << ch << "\n";
        } else if (type_ == Type::Object) {
            os << "{\n";
            auto &object = data_.object;
            uint64_t object_size = object.size();
            for (const auto & it : object) {
                if (--object_size) {
                    it.second.print(os, ',', spaces + space_, it.first);
                } else {
                    it.second.print(os, ' ', spaces + space_, it.first);
                }
            }
            os << std::string(spaces, ' ') << "}" << ch << "\n";
        } else if (type_ == Type::Array) {
            os << "[\n";
            auto &array = data_.array;
            uint64_t array_size = array.size();
            for (const auto & it : array) {
                if (--array_size) {
                    it.print(os, ',', spaces + space_);
                } else {
                    it.print(os, ' ', spaces + space_);
                }
            }
            os << std::string(spaces, ' ') << "]" << ch << "\n";
        }
    }

	static constexpr uint8_t space_ = 2;
    static constexpr uint8_t reserve_ = 32;

private:
    struct Data {
        std::string string;
        bool boolean{};
        double number{};
        std::vector<Value> array;
        std::unordered_map<std::string, Value> object;
    } data_;

    Type type_;
};

}
