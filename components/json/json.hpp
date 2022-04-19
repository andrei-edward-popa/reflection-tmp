#pragma once

#include <vector>
#include <string>
#include <utility>
#include <ostream>
#include <type_traits>
#include <unordered_map>

#include <json/json_utils.hpp>
#include <json/json_value.hpp>

namespace JSON {

struct Json {
    Value& operator[](const std::string& str) noexcept {
        if (!root_.contains(str)) {
			root_.emplace(std::piecewise_construct,
				std::forward_as_tuple(str),
				std::forward_as_tuple());
        }
        return root_[str];
    }

    void print(std::ostream& os) const noexcept {
        os << "{\n";
        uint64_t json_size = root_.size();
        for (const auto & it : root_) {
            if (--json_size) {
            	it.second.print(os, ',', Value::space_, it.first);
           	} else {
                it.second.print(os, ' ', Value::space_, it.first);
            }
        }
        os << "}\n";
    }

    Json() noexcept {
    	root_.reserve(Value::reserve_);
    }

    bool empty() const noexcept {
        return root_.empty();
    }

    std::unordered_map<std::string, Value>::const_iterator cbegin() const noexcept {
        return root_.cbegin();
    }

    std::unordered_map<std::string, Value>::const_iterator cend() const noexcept {
        return root_.cend();
    }

    std::unordered_map<std::string, Value>::iterator begin() noexcept {
        return root_.begin();
    }

    std::unordered_map<std::string, Value>::iterator end() noexcept {
        return root_.end();
    }

    void clear() noexcept {
        root_.clear();
    }

private:
    std::unordered_map<std::string, Value> root_;
};

}
