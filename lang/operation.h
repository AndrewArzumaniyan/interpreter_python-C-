#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <any>

class Operation {
public:
	static const std::unordered_map<std::string, std::function<std::any(const std::any&)>> unary_op;
	static const std::unordered_map<std::string, std::function<std::any(const std::any&, const std::any&)>> binary_op;
	static const std::unordered_map<std::string, std::function<std::any(const std::vector<std::any>&)>> builtin_func;
	static std::string any_to_string(const std::any&);
};