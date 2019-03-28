#pragma once
#include "AstNode.h"

/**
	���ü�ͷ������
		expr->(int v){}
*/
class AstFunction;
class AstArrayCall : public AstNode {
public:
	AstNode* expr = nullptr;
	AstFunction* func = nullptr;
protected:
	virtual CodeGen* makeGen(AstContext* parent);
};