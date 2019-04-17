#include "stdafx.h"
#include "GoDispatch.h"
#include "DispatchBind.h"
#include "Coroutine.h"
#include <mutex>
#include <thread>
// #include <chrono>
#include <iostream>
#include <boost/lockfree/queue.hpp>

using namespace std;
volatile bool interrupt = false;

void DispatchCreateCoroutine(Coroutine* co, SiGoFunction *func)
{
	Dispatch::createCoroutine(co, func);
}

void DispatchYield()
{
	Dispatch::yield();
}

Coroutine* DispatchGetCurrent()
{
	return Dispatch::getCurrent();
}
Coroutine* DispatchSetSuspend(Chan *chan, long waiting)
{
	return Dispatch::setSuspend(chan, waiting);
}

// TODO: �����̳߳�
std::mutex threadMutex;
std::thread* dispatchThread = nullptr;
boost::lockfree::queue<std::pair<Coroutine*, SiGoFunction*>*> requests(1);

void destroyCoroutine()
{
	if (!interrupt) {
		interrupt = true;
		dispatchThread->join();
	}
}

inline int64_t now()
{
	time_t t = time(0);
	return t;
}

// ������
static thread_local Dispatch* dispatch;
// ��ǰЭ��
static thread_local Coroutine* current = nullptr;

// Э�̵�������
void __stdcall coroutineMain(LPVOID lpParameter)
{
	SiGoFunction* co = (SiGoFunction*)lpParameter;
	void* p = (*co)();
	current->data = p;
	current->status = COROUTINE_DEAD;
	dispatch->give(current);
}

void Dispatch::createCoroutine(Coroutine* co, SiGoFunction* func)
{
	requests.push(new std::pair<Coroutine*, SiGoFunction*>(co, func));
	if (!dispatchThread) {
		atexit(&destroyCoroutine);
		lock_guard<std::mutex> lck(threadMutex);
		if (dispatchThread) return;
		dispatchThread = new thread([]() {
			dispatch = new Dispatch();
			dispatch->give();
			});
	}
}

void Dispatch::yield()
{
	dispatch->give(current);
}

Coroutine* Dispatch::getCurrent()
{
	return current;
}

Coroutine* Dispatch::setSuspend(Chan *chan, long waiting)
{
	current->chan = chan;
	current->status = COROUTINE_SUSPEND;
	current->timeout = waiting ? now() + waiting : 0;
	return current;
}

// ���ƽ��ص�����
void Dispatch::give(Coroutine* c)
{
	// ����ǽ�����
	if (c) {
		if (c->status == COROUTINE_DEAD) {
			died.push(c);
			// �����Լ�ɾ���Լ�
			// DeleteFiber(c->lpFiber); 
		} else if (isRun(c->status)) {	// ��Ȼ��ִ���У������������� cpu
			c->status = COROUTINE_READY;
			ready.push_back(c);
		} else { // �����ȴ�
			suspends.push_back(c);
		}
		SwitchToFiber(lpFiber);
	} else {
		lpFiber = ConvertThreadToFiber(this);
		run();
	}
}

void Dispatch::run()
{
	std::pair<Coroutine*, SiGoFunction*>* ptr = nullptr;
	while (!interrupt) {
		Coroutine* p = nullptr;

		while (!died.empty()) {
			Coroutine* c = died.front();
			died.pop();
			DeleteFiber(c->lpFiber);
			c->lpFiber = nullptr;
			// TODO: deleteObject
		}

		// �������µ�
		if (requests.pop(ptr)) {
			create(ptr->first, ptr->second);
			current = ptr->first;
			SwitchToFiber(current->lpFiber);
			continue;
		}

		if (!ready.empty()) {
			p = ready.front();
			ready.pop_front();
		} else if (!suspends.empty()) {
			int64_t n = now();
			for (auto i = suspends.begin(); i != suspends.end(); ) {
				Coroutine* x = *i;
				if (isRun(x->status)) {
					p = x;
					i = suspends.erase(i);
					break;
				}
				// ��� chan
				if (x->chan && x->chan->setted) {
					x->data = x->chan->data;
					p = x;
					i = suspends.erase(i);
					break;
				}

				if (x->timeout > 0 && x->timeout > n) {// �ȴ�
					x->status = COROUTINE_TIMEOUT;
					i = suspends.erase(i);
				} else {
					i++;
				}
			}
		}

		if (p) {
			current = p;
			p->status = COROUTINE_RUNNING;
			SwitchToFiber(p->lpFiber);
		} else {
			this_thread::yield();
#ifdef _WINDOWS
			Sleep(10);
#endif
		}
	}
	for (auto& i : ready) {
		DeleteFiber(i->lpFiber);
	}
	for (auto& i : suspends) {
		DeleteFiber(i->lpFiber);
	}
	clog << "Dispatch close." << endl;
}

void Dispatch::create(Coroutine* co, SiGoFunction* func)
{
	co->lpFiber = CreateFiberEx(INIT_STACK, 0, FIBER_FLAG_FLOAT_SWITCH, coroutineMain, func);
	co->status = COROUTINE_READY;
	co->timeout = 0;
}

Dispatch::~Dispatch()
{
}

bool Dispatch::isRun(int status)
{
	return status == COROUTINE_RUNNING || status == COROUTINE_READY;
}
