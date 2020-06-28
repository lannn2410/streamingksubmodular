#include "Streaming.h"
#include <algorithm>
#include <math.h>

Streaming::Streaming(Network * g) : Framework(g)
{
	int no_nodes = g->get_no_nodes();
	for (int i = 0; i < no_nodes; ++i) {
		node_sequence.push_back(i);
	}
	random_shuffle(node_sequence.begin(), node_sequence.end());
	no_queries = 0;
	max_solution = 0.0;
}

Streaming::~Streaming()
{
}

double Streaming::get_solution(bool is_ds)
{
	double max_gain = 0.0;
	int j_min, j_max = 0;
	double log_delta = log(1 + Constants::DELTA);
	int c_passed = 0;
	for (uint e : node_sequence) {
		++c_passed;
		// get max gain of a single e,i
		for (int i = 0; i < Constants::K; ++i) {
			vector<pair<uint, uint>> single_seed = {pair<uint,uint>(e,i)};
			double tmp = estimate_influence(single_seed);
			++no_queries;
			if (max_gain < tmp) {
				max_gain = tmp;
				int j_tmp = ceil(log(max_gain / ((1 + Constants::EPS) * (1 + Constants::DELTA) * Constants::BUDGET * Constants::M)) / log_delta); // could < 0
				j_min = j_tmp >= 0 ? j_tmp : 0;
				if (is_ds) // upper bound if ds is running
					j_max = floor(log(max_gain * (1 + Constants::EPS)) / log_delta);
				else // upper bound if rs is running
					j_max = floor(log(max_gain * ((1+Constants::EPS)*(1+Constants::EPS) + 4*Constants::EPS*Constants::BUDGET)/((1-Constants::EPS*Constants::EPS)*(1-Constants::EPS))) / log_delta);
				if (j_max + 1 > sub_seeds.size()) {
					sub_seeds.resize(j_max + 1, vector<kseeds>(Constants::NO_DENOISE_STEPS));
					influences.resize(j_max + 1, vector<double>(Constants::NO_DENOISE_STEPS, 0.0));
					for (int j = thresholds.size(); j < j_max + 1; ++j) {
						thresholds.push_back(pow(1 + Constants::DELTA, j));
					}
				}
			}
		}
		
		// select element for each sub seed
		if (sub_seeds.size() > 0) {
			for (int j = j_min; j <= j_max; ++j) {
				for (int jj = 0; jj < Constants::NO_DENOISE_STEPS; ++jj) {
					if (sub_seeds[j][jj].size() < Constants::BUDGET) {
						int i = select_element(j, e, jj);
						if (i != -1) {
							sub_seeds[j][jj].push_back(pair<uint, uint>(e, i));
						}
					}
				}
				
			}
		}
	}
	return max_solution;
}

uint Streaming::get_no_queries()
{
	return no_queries;
}
