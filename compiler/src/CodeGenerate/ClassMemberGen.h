#pragma once
#include "CodeGen.h"

class ClassInterface;
class ClassMemberGen : public CodeGen
{
public:
	CodeGen *object = nullptr;	// ����
	int index = -1;
	std::string name;
	llvm::StructType * classType = nullptr;
	bool isProtected = false;
	llvm::Function* setFunction = nullptr;
	llvm::Function* getFunction = nullptr;
	llvm::Value* generateCode(llvm::Module *m, llvm::Function *func, llvm::IRBuilder<>&);
};

