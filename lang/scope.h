#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "tree.h"

enum ObjType{
	VARIABLE, CLASS_OBJ, FUNCTION
};

struct Object {
	std::string name;
	ObjType type;

	Object(std::string name, ObjType type) : name(name), type(type) {}

	virtual ~Object() = default;
};

struct VarObj : Object {
	std::any value;

	VarObj(std::string name, std::any value) : value(value), Object(name, VARIABLE) {}
};

struct FuncObj : Object {
	std::vector<std::string> args;
	BlockContainer block;

	FuncObj(std::string name, std::vector<std::string> args, BlockContainer block) : 
		block(block), args(args), Object(name, FUNCTION) {}
};

struct ClassObj : Object {
	std::vector<std::string> args;
	ClassBlockContainer block;

	ClassObj(std::string name, std::vector<std::string> args, ClassBlockContainer block) : 
		block(block), args(args), Object(name, CLASS_OBJ) {}
};



using Objects = std::unordered_map<std::string, std::shared_ptr<Object>>;


struct Scope {
	std::string name;
	std::size_t level;

	Objects objects;

	std::shared_ptr<Scope> parent_scope;

	Scope(const std::string& name, std::size_t level, const std::shared_ptr<Scope>& parent_scope = nullptr) :
		name(name), level(level), parent_scope(parent_scope), objects({}) {}
};