#include "StreamingGreedy.h"

StreamingGreedy::StreamingGreedy(Network * g) : Framework(g), no_queries(0)
{
}

StreamingGreedy::~StreamingGreedy()
{
}

double StreamingGreedy::get_solution(bool is_ds)
{
	kseeds seeds;
	int no_nodes = g->get_no_nodes();

	for (int u = 0; u < no_nodes; ++u) {
		if (seeds.size() < Constants::BUDGET) {
			double r = ((double)(Kcommon::getInstance()->randomInThread(omp_get_thread_num()) % 10000)) / 10000;
			if (r <= ((double)Constants::BUDGET) / no_nodes) { // select this element
				double max = 0.0, i = 0;
				for (int ii = 0; ii < Constants::K; ++ii) {
					vector<pair<uint, uint>> seeds_tmp = seeds;
					seeds_tmp.push_back(pair<uint, uint>(u, ii));
					double est_F = estimate_influence(seeds_tmp);
					no_queries++;
					if (max < est_F) {
						max = est_F;
						i = ii;
					}
				}
				seeds.push_back(kp(u, i));
			}
		}
	}

	return estimate_influence(seeds);
}

int StreamingGreedy::get_no_queries()
{
	return no_queries;
}
