#include "Degree.h"
#include <algorithm>

Degree::Degree(Network * g) : Framework(g)
{
}

Degree::~Degree()
{
}

double Degree::get_solution()
{
	int no_nodes = g->get_no_nodes();
	vector<uint> degree(no_nodes), idx(no_nodes);
	for (int i = 0; i < no_nodes; ++i) {
		degree[i] = g->get_out_degree(i);
		idx[i] = i;
	}
	sort(idx.begin(), idx.end(), [&degree](size_t i1, size_t i2) {return degree[i1] > degree[i2]; });
	kseeds seeds;
	for (int i = 0; i < Constants::BUDGET; ++i) {
		seeds.push_back(pair<uint, uint>(idx[i], 0));
	}
	return estimate_influence(seeds);
}
