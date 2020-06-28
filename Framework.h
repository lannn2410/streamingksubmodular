#pragma once
#include "Network.h"

class Framework
{
public:
	Framework(Network * g);
	~Framework();
	virtual double get_solution(bool is_ds = true) = 0;
	double estimate_influence(const kseeds & seeds);
	double estimate_test(const kseeds & seeds, uint n);

protected:
	Network * g;
	uint no_samples;
};

