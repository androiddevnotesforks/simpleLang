#pragma once
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

struct Generater {
	llvm::Module* module;
	llvm::Function* func;
	llvm::IRBuilder<>* _builder;
	llvm::BasicBlock* deallocate;
public:
	llvm::LLVMContext& context() const { return _builder->getContext(); }
	llvm::IRBuilder<>& builder() const { return *_builder; }
	// �ں������׸��飨����飩�������
	inline llvm::Value* alloc(llvm::Type* ty, const std::string& name = std::string(), llvm::Value* sz = nullptr) const {
		auto& block = func->getEntryBlock();
		llvm::IRBuilder<> b(&block);
		return b.CreateAlloca(ty, sz, name);
	}
};

class CodeGen {
public:
	CodeGen(llvm::Type* t = nullptr);
	virtual ~CodeGen(){}
	llvm::Type* type = nullptr;
	llvm::Value* value = nullptr;
	virtual llvm::Value* generate(const Generater& generater);
	// enum ValueType { ivalue = 0, lvalue, rvalue }; // ����ν����ֵ����ֵ
	// ValueType valueType = ivalue;
	llvm::Value* load(llvm::IRBuilder<>& builder, llvm::Value* v);
	bool parameter = false;	// �Ƿ�������������������������Ա������	
	bool escape = false;	// �Ƿ����ݱ���

	// ����������
	llvm::Type* hope = nullptr;
protected:
	virtual llvm::Value* generateCode(const Generater& generater) = 0;
};

