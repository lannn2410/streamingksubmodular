#include "Greedy.h"

Greedy::Greedy(Network * g) : Framework(g)
{
}

Greedy::~Greedy()
{
}

double Greedy::get_solution(bool is_ds)
{
	vector<pair<uint, uint>> seeds;
	int no_nodes = g->get_no_nodes();
	vector<bool> selected(no_nodes, false);
	for (int j = 0; j < Constants::BUDGET; ++j) {
		double max = 0;
		int e = 0, i = 0, u = 0;
		for (u = 0; u < no_nodes; ++u) {
			if (!selected[u]) {
				for (int ii = 0; ii < Constants::K; ++ii) {
					vector<pair<uint, uint>> seeds_tmp = seeds;
					seeds_tmp.push_back(pair<uint, uint>(u, ii));
					double est_F = estimate_influence(seeds_tmp);
					if (max < est_F) {
						max = est_F;
						e = u;
						i = ii;
					}
				}
			}
		}

		if (max > 0) {
			seeds.push_back(pair<uint, uint>(e, i));
			selected[e] = true;
		}
		else break;
	}
	return estimate_influence(seeds);
}
