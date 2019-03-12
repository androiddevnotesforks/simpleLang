#pragma once
#include "CodeGen.h"

class ThisGen : public CodeGen
{
public:
	ThisGen(llvm::Type* tp = nullptr)
	{
		type = tp;
	}
	virtual llvm::Value* generateCode(llvm::Module *m, llvm::Function *func, llvm::IRBuilder<>& builder)
	{
		llvm::raw_os_ostream os(std::clog);
		std::clog << "Get this: ";
		func->print(os);
	
		auto begin = func->args().begin();
		auto end = func->args().end();
		if (!begin || begin == end)
			throw std::runtime_error("Function do not have 'this'.");
		llvm::Argument &it = *begin;
		type = it.getType();
		llvm::Value* v=&it;

		os << " ";
		v->print(os, true);
		os.flush();
		std::clog << std::endl;

		return v;
	}
};