#pragma once
#include "Streaming.h"

class Deterministic : public Streaming
{
public:
	Deterministic(Network * g);
	~Deterministic();
	int select_element(int j, uint e, int step);
};

