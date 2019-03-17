#pragma once
#include <utility>
#include <list>
#include <queue>
#include "Coroutine.h"

struct si_Coroutine;
typedef struct si_Coroutine Coroutine;

class Dispatch {
#ifdef _WINDOWS
	LPVOID lpFiber = 0;
#endif
public:
	static void createCoroutine(Coroutine* co, SiGoFunction *func);
	static void yield();
	static Coroutine* getCurrent();
	static Coroutine* setSuspend(long waiting = 0);
public:
	// ��ջ��С
	int INIT_STACK = 1048576;

	// ���ƽ��ص�����
	void give(Coroutine* c = nullptr);

	void run();

	std::list<Coroutine*> ready;
	std::list<Coroutine*> suspends;
	std::queue<Coroutine*> died;

	void create(Coroutine* co, SiGoFunction* func);

	~Dispatch();
private:
	bool isRun(int status);
};
