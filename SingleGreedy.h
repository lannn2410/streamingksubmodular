#pragma once
#include "Framework.h"
class SingleGreedy : public Framework
{
public:
	SingleGreedy(Network * g, uint k);
	~SingleGreedy();
	double get_solution();
private:
	uint k;
};

