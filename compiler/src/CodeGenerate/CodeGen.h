#pragma once
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

class CodeGen {
public:
	CodeGen(llvm::Type* t = nullptr);
	virtual ~CodeGen(){}
	llvm::Type* type = nullptr;
	llvm::Value* value = nullptr;
	virtual llvm::Value* generate(llvm::Module *m, llvm::Function *func, llvm::IRBuilder<>&builder);
	// enum ValueType { ivalue = 0, lvalue, rvalue }; // ����ν����ֵ����ֵ
	// ValueType valueType = ivalue;
	llvm::Value* load(llvm::IRBuilder<>& builder, llvm::Value* v);
	bool parameter = false;	// �Ƿ�������������������������Ա������	
	bool escape = false;	// �Ƿ����ݱ���
protected:
	virtual llvm::Value* generateCode(llvm::Module *m, llvm::Function *func, llvm::IRBuilder<>&) = 0;
};

