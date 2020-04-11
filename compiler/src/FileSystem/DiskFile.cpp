#include "stdafx.h"
#include <string>
#include <llvm/Support/DynamicLibrary.h>

#include "DiskFile.h"
#include "Exception/LoadFileException.h"

using namespace std;
DiskFile::DiskFile(const std::filesystem::path& path) : _path(path)
{
}

void DiskFile::loadDynamicLibrary()
{
	string err;
	string file=_path.string();
	if (llvm::sys::DynamicLibrary::LoadLibraryPermanently(file.c_str(), &err)) {
		cerr << "��ȡ clib.dll ʧ�ܣ�" << err << endl;
		throw LoadFileException(file, err);
	}
}
