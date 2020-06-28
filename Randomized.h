#pragma once
#include "Streaming.h"

class Randomized : public Streaming
{
public:
	Randomized(Network * g);
	~Randomized();
	int select_element(int j, uint e, int step);
};

