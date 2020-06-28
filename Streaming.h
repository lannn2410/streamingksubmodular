#pragma once
#include "Framework.h"
 

class Streaming : public Framework
{
public:
	Streaming(Network * g);
	~Streaming();
	double get_solution(bool is_ds = true);
	virtual int select_element(int j, uint e, int step) = 0; // select element, different in each streaming algorithm
	uint get_no_queries();
protected:	
	double max_solution; // save the influence of the best among sub seeds
	
	vector<vector<kseeds>> sub_seeds; 
	// store seeds of each (1+delta)^j and eps', index of first list is j, second list is denoise step
	
	vector<double> thresholds; // store value of (1+\delta)^j
	vector<vector<double>> influences; // store estimated influence of each (1+\delta)^j and eps'
	vector<uint> node_sequence;
	uint no_queries; // count number of queries to F
};

