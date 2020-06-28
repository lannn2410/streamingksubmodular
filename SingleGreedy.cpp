#include "SingleGreedy.h"


SingleGreedy::SingleGreedy(Network * g, uint k) : Framework(g), k(k)
{
}

SingleGreedy::~SingleGreedy()
{
}

double SingleGreedy::get_solution()
{
	vector<pair<uint, uint>> seeds;
	int no_nodes = g->get_no_nodes();
	vector<bool> selected(no_nodes, false);
	for (int j = 0; j < Constants::BUDGET; ++j) {
		double max = 0;
		int e = 0, u = 0;
		for (u = 0; u < no_nodes; ++u) {
			if (!selected[u]) {
				vector<pair<uint, uint>> seeds_tmp = seeds;
				seeds_tmp.push_back(pair<uint, uint>(u, k));
				double est_F = estimate_influence(seeds_tmp);
				if (max < est_F) {
					max = est_F;
					e = u;
				}
			}
		}

		if (max > 0) {
			seeds.push_back(pair<uint, uint>(e, k));
			selected[e] = true;
		}
		else break;
	}
	return estimate_influence(seeds);
}
