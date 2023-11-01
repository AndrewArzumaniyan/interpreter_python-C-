#include "alphabet.h"

const std::string Alphabet::literals = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const std::string Alphabet::digits = "0123456789";
const std::string Alphabet::spaces = " ";
const std::string Alphabet::metachars = ":()[]{},+-*/%=!<>\n\t";
const char Alphabet::dot = '.';
const std::string Alphabet::quotes = "\"'";

bool Alphabet::isspace(const char& c) {
	return spaces.find(c) != std::string::npos;
}

bool Alphabet::isliteral(const char& c) {
	return literals.find(c) != std::string::npos;
}

bool Alphabet::isdigit(const char& c) {
	return digits.find(c) != std::string::npos;
}

bool Alphabet::ismetachar(const char& c) {
	return metachars.find(c) != std::string::npos;
}

bool Alphabet::isquote(const char& c) {
	return quotes.find(c) != std::string::npos;
}

bool Alphabet::isdot(const char& c) {
	return c == dot;
}

bool Alphabet::iscomment(const char& c) {
	return c == '#';
}

bool Alphabet::isnewline(const char& c) {
	return c == '\n';
}

bool Alphabet::isalphanumeric(const char& c) {
	return isliteral(c) || isdigit(c);
}
