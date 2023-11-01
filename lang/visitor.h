#pragma once
#include <vector>
#include <any>
#include "tree.h"
#include "scope.h"

class MutableVisitor {
public:
	virtual void visit(BlockContainer&) = 0;
	virtual void visit(ClassBlockContainer&) = 0;
	virtual void visit(Pass&) = 0;
	virtual void visit(Const<int>&) = 0;
	virtual void visit(Const<float>&) = 0;
	virtual void visit(Const<std::string>&) = 0;
	virtual void visit(Const<bool>&) = 0;
	virtual void visit(Var&) = 0;
	virtual void visit(Unary&) = 0;
	virtual void visit(Binary&) = 0;
	virtual void visit(While&) = 0;
	virtual void visit(For&) = 0;
	virtual void visit(Func&) = 0;
	virtual void visit(Conditional&) = 0;
	virtual void visit(Return&) = 0;
	virtual void visit(Break&) = 0;
	virtual void visit(Continue&) = 0;
	virtual void visit(Func_decl&) = 0;
	virtual void visit(Class_decl&) = 0;
};

class ConstVisitor {
public:
	virtual void visit(const BlockContainer&) = 0;
	virtual void visit(const ClassBlockContainer&) = 0;
	virtual void visit(const Pass&) = 0;
	virtual void visit(const Const<int>&) = 0;
	virtual void visit(const Const<float>&) = 0;
	virtual void visit(const Const<std::string>&) = 0;
	virtual void visit(const Const<bool>&) = 0;
	virtual void visit(const Var&) = 0;
	virtual void visit(const Unary&) = 0;
	virtual void visit(const Binary&) = 0;
	virtual void visit(const While&) = 0;
	virtual void visit(const For&) = 0;
	virtual void visit(const Func&) = 0;
	virtual void visit(const Conditional&) = 0;
	virtual void visit(const Return&) = 0;
	virtual void visit(const Break&) = 0;
	virtual void visit(const Continue&) = 0;
	virtual void visit(const Func_decl&) = 0;
	virtual void visit(const Class_decl&) = 0;
};

class Printer : public ConstVisitor {
public:
	void visit(const BlockContainer&) override;
	void visit(const ClassBlockContainer&) override;
	void visit(const Pass&) override;
	void visit(const Const<int>&) override;
	void visit(const Const<float>&) override;
	void visit(const Const<std::string>&) override;
	void visit(const Const<bool>&) override;
	void visit(const Var&) override;
	void visit(const Unary&) override;
	void visit(const Binary&) override;
	void visit(const While&) override;
	void visit(const For&) override;
	void visit(const Func&) override;
	void visit(const Conditional&) override;
	void visit(const Return&) override;
	void visit(const Break&) override;
	void visit(const Continue&) override;
	void visit(const Func_decl&) override;
	void visit(const Class_decl&) override;
};

class Analyzer : public MutableVisitor {
private:
	std::any result;
	std::any func_res;
	bool returned = false;
public:
	std::shared_ptr<Scope> scope;

	void visit(BlockContainer&) override;
	void visit(ClassBlockContainer&) override;
	void visit(Pass&) override;
	void visit(Const<int>&) override;
	void visit(Const<float>&) override;
	void visit(Const<std::string>&) override;
	void visit(Const<bool>&) override;
	void visit(Var&) override;
	void visit(Unary&) override;
	void visit(Binary&) override;
	void visit(While&) override;
	void visit(For&) override;
	void visit(Func&) override;
	void visit(Conditional&) override;
	void visit(Return&) override;
	void visit(Break&) override;
	void visit(Continue&) override;
	void visit(Func_decl&) override;
	void visit(Class_decl&) override;

	Analyzer(const std::shared_ptr<Scope>& scope = nullptr) : scope(scope) {}
	auto get_result() {
		return result;
	}
	auto get_func_res() {
		return result;
	}
};