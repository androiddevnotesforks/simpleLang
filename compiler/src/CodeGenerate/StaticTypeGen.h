#pragma once
#include "CodeGen.h"

/* �����ھ�̬�Ƶ� */
class StaticTypeGen : public CodeGen {
public:
	virtual llvm::Value* generateCode(llvm::Module *m, llvm::Function *func, llvm::IRBuilder<>&) { return nullptr; };

	StaticTypeGen(bool isTrue) {
		this->type = nullptr;
		this->isTrue = isTrue;
	}
	bool isTrue;
};