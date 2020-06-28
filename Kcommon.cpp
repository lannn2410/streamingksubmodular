#include "Kcommon.h"

Kcommon * Kcommon::instance = nullptr;

Kcommon::Kcommon()
{
	seed = new int[10000];
	for (int i = 0; i < 10000; i++) {
		seed[i] = rand();
	}
}

Kcommon::~Kcommon()
{
}

Kcommon * Kcommon::getInstance()
{
	if (!instance)
		instance = new Kcommon();
	return instance;
}

unsigned Kcommon::randomInThread(int thread_id)
{
	unsigned tmp = seed[thread_id % 10000];
	tmp = tmp * 17931 + 7391;
	seed[thread_id % 10000] = tmp;
	return tmp;
}
