#pragma once
#include "CodeGen.h"

/*
   Э�̲�����װ
*/
class GoWrapValue : public CodeGen
{
public:
	GoWrapValue(size_t index, llvm::Type* type);

	virtual llvm::Value* generateCode(const Generater& generater);
private:
	size_t _index;
};
