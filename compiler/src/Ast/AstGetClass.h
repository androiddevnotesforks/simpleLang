#pragma once
#include "AstNode.h"
#include "AstClass.h"

/* ��ȡ�� */
class AstGetClass : public AstType {
public:
	AstClass* toClass = nullptr;
};
