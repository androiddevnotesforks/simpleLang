#pragma once

class CompilerOptions
{
public:
	static CompilerOptions& instance();
public:
	std::string triple;
	bool directlyExecute = true;	// �Ƿ�ֱ������
};
