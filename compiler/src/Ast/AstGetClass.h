#pragma once
#include "AstNode.h"
#include "AstClass.h"

/* ��ȡ�� */
class AstGetClass : public AstType {
public:
	std::vector<AstType*> templateVars;
	virtual llvm::Type* llvmType(llvm::LLVMContext& context);
	AstContext * context = nullptr;
};
