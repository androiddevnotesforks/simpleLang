#pragma once

#include "AstList.h"

class AstSequence : public AstNode {
public:
	AstSequence(AstNode* node);
	/// ��������ģ���Ƶ�
	virtual CodeGen* makeGen(AstContext* parent);

	std::vector<AstNode*> lines;			// ����
};
