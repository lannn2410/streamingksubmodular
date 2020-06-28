#pragma once
#include <omp.h>
#include <vector>
#include <string>
#include "Constants.h"

using namespace std;

typedef unsigned int uint;
typedef pair<uint, uint> kp;
typedef vector<kp> kseeds;

typedef vector<double> kbins;
typedef vector<kbins> ksensors;

class Kcommon
{
public:
	Kcommon();
	~Kcommon();

	static Kcommon * getInstance();
	unsigned randomInThread(int thread_id);

private:
	static Kcommon * instance;
	int * seed;
};

