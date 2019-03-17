#include "stdafx.h"
#include "AstGo.h"
#include "AstFunction.h"
#include "AstContext.h"
#include "AstLet.h"
#include "AstList.h"
#include "AstImport.h"
#include "AstReturn.h"
#include "AstConstant.h"
#include "../Type/LLVMType.h"
#include "CodeGenerate/GoGen.h"
#include "modules.h"

AstGo::AstGo(AstNode* node) : _node(node)
{
	assert(node);
}

CodeGen * AstGo::makeGen(AstContext * parent)
{
	auto&c = parent->context();
	auto* func = dynamic_cast<AstFunction*>(_node);
	// ������Ǻ�����˵���Ǻ������ã���װΪ��������
	if (!func) {
		func = new AstFunction();
		func->funcType = AstFunction::Lambda;
		func->block.push_back(_node);
	} 
	// go ���������� void*, ���Ǹ���һ��
	AstLet* let = new AstLet();
	let->type = LLVMType::getVoidPtr(parent->context());
	func->rets.push_back(let);
	auto * r = new AstReturn();
	auto *zero = new AstConstant("0");
	zero->set_value(0L);
	r->returnValues.push_back(zero);
	func->block.push_back(r);

	auto* x = func->makeGen(parent);
	auto* p = new GoGen(x);
	// auto* g = AstImport::loadClass("si", "Coroutine");
	p->type = CLangModule::getStruct("si", "Coroutine");
	return p;
}
