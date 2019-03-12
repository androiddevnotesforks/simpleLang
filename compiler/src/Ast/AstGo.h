#pragma once
#include "AstNode.h"

class AstFunction;
class AstGo : public AstNode {
public:
	AstGo(AstNode* node);
	/// ��������ģ���Ƶ������ɴ���
	virtual CodeGen* makeGen(AstContext* parent);
private:
	AstNode* _node;
};

