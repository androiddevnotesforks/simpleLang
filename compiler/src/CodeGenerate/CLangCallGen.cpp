#include "stdafx.h"
#include "CLangCallGen.h"
#include "caster.h"

using namespace llvm;
/*
	���� C ���ĺ���
*/
CLangCallGen::CLangCallGen(llvm::Function* func, std::vector<CodeGen*>&& attrs)
	: llvmFunction(func), params(std::move(attrs)) {
	auto *t=func->getFunctionType();
	type = t->getReturnType();
}

template< typename T >
inline void putBack(llvm::IRBuilder<>& builder, std::vector< llvm::Value* >& a, llvm::Value* v, T& iter, T& end)
{
	if (iter == end)
		a.push_back(v);
	else {
		a.push_back(try_cast(builder, *iter, v));
		iter++;
	}
}

llvm::Value* CLangCallGen::generateCode(const Generater& generater)
{
	auto &c=generater.context();
	auto funcType = llvmFunction->getFunctionType();
	auto iter = funcType->param_begin();
	auto end = funcType->param_end();

	auto& builder = generater.builder();
	std::vector< llvm::Value* > a;
	for (auto* i : params) {
		if (iter != end)
			i->hope = *iter;
		
		Value* v = i->generate(generater);
		// ����ǻ�������
		if (i->type->isIntegerTy() || i->type->isFloatingPointTy()) {
			if (v->getType()->isPointerTy())
				v = builder.CreateLoad(v);
			putBack(builder, a, v, iter, end);
		}
		else if (i->type->isArrayTy()) {
			putBack(builder, a, v, iter, end);
			// д���С
			auto sz = i->type->getArrayNumElements();
			auto* s = ConstantInt::get(c, APInt(32, sz));
			putBack(builder, a, s, iter, end);
		}
		else {
			putBack(builder, a, v, iter, end);
		}
	}

	Value* v = builder.CreateCall(funcType, llvmFunction, a);
	// ���һ��ǿ��ת��������ĳЩ c �������ص����Ͳ�һ��
	if (type && type->isStructTy()) {
		Type* ty = llvm::PointerType::get(type, 0);
		return builder.CreateBitOrPointerCast(v, ty);
	}
	return v;
}
