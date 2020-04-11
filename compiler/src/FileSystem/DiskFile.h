#pragma once
#include "IFile.h"
#include <filesystem>

/*
 * ֱ��λ�ڴ����ϵ��ļ�
 */
class DiskFile :	public IFile
{
public:
	DiskFile(const std::filesystem::path& path);
	virtual void loadDynamicLibrary();
private:
	std::filesystem::path _path;
};

