#include "visitor.h"
template struct Const<int>;
template struct Const<float>;
template struct Const<std::string>;
template struct Const<bool>;


void BlockContainer::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void ClassBlockContainer::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Pass::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

template<typename T>
void Const<T>::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Var::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Unary::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Binary::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Func::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Conditional::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void While::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void For::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Return::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Break::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Continue::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Func_decl::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}

void Class_decl::accept(ConstVisitor& visitor) const {
	visitor.visit(*this);
}


//-------------------------------------------------------

void BlockContainer::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void ClassBlockContainer::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Pass::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

template<typename T>
void Const<T>::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Var::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Unary::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Binary::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Func::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Conditional::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void While::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void For::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Return::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Break::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Continue::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Func_decl::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}

void Class_decl::accept(MutableVisitor& visitor) {
	visitor.visit(*this);
}