#pragma once
#include <stdint.h>
#include <setjmp.h>

struct si_Coroutine {
	jmp_buf buf;	// ��ת��ַ
	int64_t value;	// ֵ
};