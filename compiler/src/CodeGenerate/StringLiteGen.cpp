#include "stdafx.h"
#include <Windows.h>
#include <llvm/IR/Constants.h>

#include "StringLiteGen.h"
#include "CallGen.h"
#include "NewGen.h"
#include "../modules.h"

using namespace llvm;
extern bool utf8File;
StringLiteGen::StringLiteGen(llvm::LLVMContext& c, const std::string & s) : _str(s), _context(c)
{
	// ������
	UINT code = utf8File ? CP_UTF8 : CP_ACP;
	int  len = (int)_str.length();
	int  unicodeLen = ::MultiByteToWideChar(code,
											0,
											_str.c_str(),
											-1,
											NULL,
											0);
	wchar_t *  pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(code,
						  0,
						  _str.c_str(),
						  -1,
						  (LPWSTR)pUnicode,
						  unicodeLen);
	int ulen = unicodeLen * sizeof(wchar_t);

	_data.assign(pUnicode, unicodeLen-1);
	delete[]  pUnicode;

	type = CLangModule::getStruct("si", "String");
}

void StringLiteGen::append(StringLiteGen * g)
{
	_str += g->_str;
}

llvm::Value * StringLiteGen::generateCode(llvm::Module * m, llvm::Function * func, llvm::IRBuilder<>& builder)
{
	auto &c = func->getContext();
	int ulen = (int)_data.size() * sizeof(wchar_t);
	auto *v=builder.CreateGlobalStringPtr(StringRef(
		(const char*)_data.c_str(), ulen
	));

	NewGen n(type);
	auto* obj = n.generate(m, func, builder);
	auto* creator=CLangModule::getFunction("si_String_Init");
	assert(creator);

	auto *call=CallGen::call(builder, creator,
		obj,
		v,
		_data.size(),
		1200);
	return obj;
}
