#pragma once
#include "Framework.h"
class Degree : public Framework
{
public:
	Degree(Network * g);
	~Degree();
	double get_solution();
};

