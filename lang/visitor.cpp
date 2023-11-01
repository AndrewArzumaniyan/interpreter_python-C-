#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include "visitor.h"
#include "operation.h"

void Printer::visit(const BlockContainer& obj) {
	for (auto it : obj.block) {
		std::cout << "\t";
		for (int i = 1; i < obj.level; std::cout << "\t", i++);
		it->accept(*this);
	}
	std::cout << std::endl;
}

void Printer::visit(const ClassBlockContainer& obj) {
	for (auto it : obj.block) {
		std::cout << "\t";
		for (int i = 1; i < obj.level; std::cout << "\t", i++);
		it->accept(*this);
	}
	std::cout << std::endl;
}

void Printer::visit(const Pass& obj) {
	std::cout << obj.value << std::endl;
}

void Printer::visit(const Const<int>& obj) {
	std::cout << obj.value;
}

void Printer::visit(const Const<float>& obj) {
	std::cout << obj.value;
}

void Printer::visit(const Const<std::string>& obj) {
	std::cout << obj.value;
}

void Printer::visit(const Const<bool>& obj) {
	std::cout << obj.value;
}

void Printer::visit(const Var& obj) {
	std::cout << obj.value << "(type: " << obj.type << ")";
}

void Printer::visit(const Unary& obj) {
	std::cout << obj.value;
	obj.child->accept(*this);
}

void Printer::visit(const Binary& obj) {
	obj.left->accept(*this);
	std::cout << " " << obj.value << " ";
	obj.right->accept(*this);
	std::cout << std::endl;
}

void Printer::visit(const While& obj) {
	std::cout << "while ";
	obj.expr->accept(*this);
	obj.block.accept(*this);
}

void Printer::visit(const For& obj) {
	std::cout << "while ";
	obj.expr->accept(*this);
	obj.block.accept(*this);
}

void Printer::visit(const Func& obj) {
	std::cout << obj.value << "(";
	for (auto it : obj.branches) {
		it->accept(*this);
	}
	std::cout << ")";
}

void Printer::visit(const Conditional& obj) {
	std::cout << "if ";
	obj.expr[0]->accept(*this);
	std::cout << ":\n";
	obj.ifblocks[0].accept(*this);
	
	for (int i = 1; i < obj.ifblocks.size(); i++) {
		std::cout << "elif ";
		obj.expr[i]->accept(*this);
		std::cout << ":\n";

		for (int j = 0; j < obj.ifblocks[i].level; std::cout << std::endl, j++);
		obj.ifblocks[i].accept(*this);
	}

	if (obj.elseblock.block.size() != 0) {
		std::cout << "else:\n";
		obj.elseblock.accept(*this);
	}
}

void Printer::visit(const Return& obj) {
	std::cout << obj.value << " ";
	obj.expr->accept(*this);
}

void Printer::visit(const Break& obj) {
	std::cout << obj.value << " ";
	obj.level->accept(*this);
}

void Printer::visit(const Continue& obj) {
	std::cout << obj.value << std::endl;
}

void Printer::visit(const Func_decl& obj) {
	std::cout << "def " << obj.value << "(";
	for (auto it : obj.args) {
		std::cout << it << ",";
	}
	std::cout << "):\n";
	obj.block.accept(*this);
}

void Printer::visit(const Class_decl& obj) {
	std::cout << "def " << obj.value << "(";
	for (auto it : obj.args)
		std::cout << it << ",";
	std::cout << "):\n";
	obj.block.accept(*this);
}


//-----------------------------------------------------

void Analyzer::visit(BlockContainer& obj) {
	for (auto it : obj.block) {
		it->accept(*this);
		if (returned) {
			break;
		}
	}
};

void Analyzer::visit(ClassBlockContainer& obj) {

};

void Analyzer::visit(Pass& obj) {

};

void Analyzer::visit(Const<int>& obj) {
	result = std::any(obj.value);
};

void Analyzer::visit(Const<float>& obj) {
	result = std::any(obj.value);
};

void Analyzer::visit(Const<std::string>& obj) {
	result = std::any(obj.value);
};

void Analyzer::visit(Const<bool>& obj) {
	result = std::any(obj.value);
};

void Analyzer::visit(Var& obj) {
	auto tmp_scope = scope;
	for (auto search = tmp_scope->objects.find(obj.value); search == tmp_scope->objects.end(); search = tmp_scope->objects.find(obj.value)) {
		if (tmp_scope->parent_scope == nullptr) {
			throw std::runtime_error("No such element in map!");
		}
		tmp_scope = tmp_scope->parent_scope;
	}
	
	auto tmp = tmp_scope->objects.at(obj.value);
	result = std::dynamic_pointer_cast<VarObj>(tmp)->value;
};

void Analyzer::visit(Unary& obj) {
	obj.child->accept(*this);
	auto tmp_res = get_result();

	result = Operation::unary_op.at(obj.value)(tmp_res);
};

void Analyzer::visit(Binary& obj) {
	if (obj.value == "=") {
		if (obj.left->type != VAR) {
			throw std::runtime_error("Invalid declaration!");
		}

		auto tmp = std::dynamic_pointer_cast<Var>(obj.left);

		obj.right->accept(*this);
		scope->objects.insert(std::make_pair(tmp->value, std::make_shared<VarObj>(tmp->value, result)));
	}
	else {
		obj.right->accept(*this);
		auto right_res = get_result();
		int tmp = std::any_cast<int>(right_res);
		obj.left->accept(*this);
		auto left_res = get_result();
		tmp = std::any_cast<int>(left_res);

		result = Operation::binary_op.at(obj.value)(left_res, right_res);
	}

};

void Analyzer::visit(While& obj) {
	
};

void Analyzer::visit(For& obj) {

};

void Analyzer::visit(Func& obj) {
	if (obj.type == BUILTIN_FUNC) {
		std::vector<std::any> args;
		for (auto& it : obj.branches) {
			it->accept(*this);
			args.push_back(result);
		}
		result = Operation::builtin_func.at(obj.value)(args);
	}
	else {
		auto tmp_scope = scope;
		for (auto search = tmp_scope->objects.find(obj.value); search == tmp_scope->objects.end(); search = tmp_scope->objects.find(obj.value)) {
			if (tmp_scope->parent_scope == nullptr) {
				throw std::runtime_error("Unknown function!");
			}
			tmp_scope = tmp_scope->parent_scope;
		}
	
		auto tmp = std::dynamic_pointer_cast<FuncObj>(tmp_scope->objects.at(obj.value));
		if (obj.branches.size() != tmp->args.size())
			throw std::runtime_error("Invalid count of arguments in " + obj.value + "() function!");

		std::vector<std::any> args;
		for (auto it : obj.branches) {
			it->accept(*this);
			args.push_back(result);
		}


		Scope newscope(obj.value, tmp->block.level, tmp_scope);
		for (int i = 0; i < args.size(); i++) {
			newscope.objects.insert(std::make_pair(tmp->args[i], std::make_shared<VarObj>(tmp->args[i], args[i])));
		}

		std::shared_ptr<Scope> ptr = std::make_shared<Scope>(newscope);

		Analyzer analyzer(ptr);

		tmp->block.accept(analyzer);
		if (analyzer.returned) {
			std::cout << "253: return: " << std::any_cast<int>(result) << std::endl;
			result = analyzer.func_res;
			returned = false;
		}
	}
};

void Analyzer::visit(Conditional& obj) {
	obj.expr[0]->accept(*this);
	if (std::any_cast<bool>(result)) {
		obj.ifblocks[0].accept(*this);
		if (returned) {
			return;
		}
	}
	else if (obj.expr.size() > 1) {
		for (int i = 1; i < obj.expr.size(); i++) {
			obj.expr[i]->accept(*this);
			if (std::any_cast<bool>(result)) {
				obj.ifblocks[i].accept(*this);
				if (returned) {
					return;
				}
				break;
			}
		}
	}
	else if (obj.elseblock.block.size() != 0) {
		obj.elseblock.accept(*this);
		if (returned) {
			return;
		}
	}
	
};

void Analyzer::visit(Return& obj) {
	obj.expr->accept(*this);
	returned = true;
	func_res = result;
};

void Analyzer::visit(Break& obj) {

};

void Analyzer::visit(Continue& obj) {

};

void Analyzer::visit(Func_decl& obj) {
	scope->objects.insert(std::make_pair(obj.value, std::make_shared<FuncObj>(obj.value, obj.args, obj.block)));
};

void Analyzer::visit(Class_decl& obj) {
	Scope newscope(obj.value, obj.block.level, scope);
	std::shared_ptr<Scope> ptr = std::make_shared<Scope>(newscope);
	Analyzer analyzer(ptr);

	obj.block.accept(analyzer);
};