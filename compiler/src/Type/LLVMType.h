﻿#pragma once
#include "AstType.h"

class LLVMType : public AstType
{
public:
	LLVMType(llvm::Type* type);
	virtual llvm::Type* llvmType(llvm::LLVMContext& context) { return _llvmType; }
	static AstType* get(llvm::Type* type);
	static AstType* getVoidPtr(llvm::LLVMContext& context);
public:
	llvm::Type* _llvmType;
};

