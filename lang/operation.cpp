#include <iostream>
#include "operation.h"

int fact(int a) {
	if (a == 0) {
		return 1;
	}
	return a * fact(a - 1);
}


std::string Operation::any_to_string(const std::any& el) {
	try {
		auto tmp = std::any_cast<int>(el);
		return std::to_string(tmp);
	}
	catch (...) {
		try {
			auto tmp = std::any_cast<float>(el);
			return std::to_string(tmp);
		}
		catch (...) {
			try {
				auto tmp = std::any_cast<bool>(el);
				return std::to_string(tmp);
			}
			catch (...) {
				try {
					auto tmp = std::any_cast<std::string>(el);
					return tmp;
				}
				catch (...) {
					std::cout << "no str method";
				}
			}
		}
	}
}

const std::unordered_map<std::string, std::function<std::any(const std::any&)>> Operation::unary_op {
	{"+", [](const std::any& x) -> std::any {
		return x;
	}},
	{ "-", [](const std::any& x) -> std::any { 
		if (auto res = std::any_cast<int>(x)) {
			return -res;
		}
		return -std::any_cast<float>(x);
	}},
};
const std::unordered_map<std::string, std::function<std::any(const std::any&, const std::any&)>> Operation::binary_op {
	{"+", [](const std::any& x, const std::any& y) -> std::any {
		try {
			auto left = std::any_cast<int>(x);
			auto right = std::any_cast<int>(y);
			return left + right;
		}
		catch (...) {
			try {
				auto left = std::any_cast<float>(x);
				auto right = std::any_cast<float>(y);
				return left + right;
			}
			catch (...) {
				try {
					auto left = std::any_cast<std::string>(x);
					auto right = std::any_cast<std::string>(y);
					return left + right;
				}
				catch (...) {
					std::cout << "Not such operand from these types!";
				}
			}
		}
	}},
	{ "-", [](const std::any& x, const std::any& y) -> std::any {
		try {
			auto left = std::any_cast<int>(x);
			auto right = std::any_cast<int>(y);
			return left - right;
		}
		catch (...) {
			try {
				auto left = std::any_cast<float>(x);
				auto right = std::any_cast<float>(y);
				return left - right;
			}
			catch (...) {
				try {
					auto left = std::any_cast<bool>(x);
					auto right = std::any_cast<bool>(y);
					return left - right;
				}
				catch (...) {
					std::cout << "Not such operand from these types!";
				}
			}
		}
	} },
	{ ">", [](const std::any& x, const std::any& y) -> std::any {
		try {
			auto left = std::any_cast<int>(x);
			auto right = std::any_cast<int>(y);
			return left > right;
		}
		catch (...) {
			try {
				auto left = std::any_cast<float>(x);
				auto right = std::any_cast<float>(y);
				return left > right;
			}
			catch (...) {
				try {
					auto left = std::any_cast<std::string>(x);
					auto right = std::any_cast<std::string>(y);
					return left > right;
				}
				catch (...) {
					try {
					auto left = std::any_cast<bool>(x);
					auto right = std::any_cast<bool>(y);
					return left > right;
				}
				catch (...) {
					std::cout << "Not such operand from these types!";
				}
				}
			}
		}
	} },
	{ "==", [](const std::any& x, const std::any& y) -> std::any {
		try {
			auto left = std::any_cast<int>(x);
			auto right = std::any_cast<int>(y);
			return left == right;
		}
		catch (...) {
			try {
				auto left = std::any_cast<float>(x);
				auto right = std::any_cast<float>(y);
				return left == right;
			}
			catch (...) {
				try {
					auto left = std::any_cast<std::string>(x);
					auto right = std::any_cast<std::string>(y);
					return left == right;
				}
				catch (...) {
					try {
						auto left = std::any_cast<bool>(x);
						auto right = std::any_cast<bool>(y);
						return left == right;
					}
					catch (...) {
						std::cout << "Not such operand from these types!";
					}
				}
			}
		}
	} }
};

const std::unordered_map<std::string, std::function<std::any(const std::vector<std::any>&)>> Operation::builtin_func {
	{"print", [](const std::vector<std::any>& vec) -> std::any {
		for (const auto& it : vec) {
			std::cout << any_to_string(it) << " ";
		}
		return std::any();
	}},
	{ "fact", [](const std::vector<std::any>& vec) -> std::any {
		if (vec.size() != 1)
			throw std::runtime_error("Invalid count of arguments for fact() function!");
		try {
			return std::any(fact(std::any_cast<int>(vec[0])));
		}
		catch (...) {
			throw std::runtime_error("Argument for fact() function must be integer!");
		}
	} },
};