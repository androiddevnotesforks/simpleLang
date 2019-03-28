#pragma once
#include "CodeGen.h"
#include <llvm/IR/ValueSymbolTable.h>

class ParamenterGen : public CodeGen
{
public:
	std::string name;
	llvm::Value* value = nullptr;

	virtual llvm::Value* generateCode(llvm::Module *m, llvm::Function *func, llvm::IRBuilder<>&builder) {
		if (!value) {
			auto* s=func->getValueSymbolTable();
			auto *v=s->lookup(name);
			auto* type = v->getType();
			if (!type->isPointerTy()) {	// Ϊ�˴����㣬��ֵ��������Ϊָ��
				value = builder.CreateAlloca(v->getType());
				builder.CreateStore(v, value);
			} else {
				value = v;
			}
		}
		return value;
	}
};
