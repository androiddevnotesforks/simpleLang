#pragma once
#include <string>

class ClassType;
class Field {
private:
	std::string _name;
	ClassType* _classType;	// �������
	ClassType* _fieldType;		// �ֶ�����

	intptr_t* _getFunction = nullptr;
	intptr_t* _setFunction = nullptr;
};
