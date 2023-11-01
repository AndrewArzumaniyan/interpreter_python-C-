#pragma once
#include <string>

class Alphabet {
	static const std::string literals;
	static const std::string digits;
	static const std::string spaces;
	static const std::string metachars;
	static const std::string quotes;
	static const char dot;
public:
	static bool isspace(const char&);
	static bool isliteral(const char&);
	static bool isdigit(const char&);
	static bool isalphanumeric(const char&);
	static bool ismetachar(const char&);
	static bool isdot(const char&);
	static bool isquote(const char&);
	static bool iscomment(const char&);
	static bool isnewline(const char&);
};