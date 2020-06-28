#pragma once
#include "Framework.h"

class StreamingGreedy : public Framework
{
public:
	StreamingGreedy(Network * g);
	~StreamingGreedy();
	double get_solution(bool is_ds = true);
	int get_no_queries();
private:
	int no_queries;
};