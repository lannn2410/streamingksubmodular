#include "Deterministic.h"


Deterministic::Deterministic(Network * g) : Streaming(g)
{
}

Deterministic::~Deterministic()
{
}

int Deterministic::select_element(int j, uint e, int step)
{
	double max = 0.0;
	uint jj;
	for (int ii = 0; ii < Constants::K; ++ii) {
		kseeds seeds_tmp = sub_seeds[j][step];
		seeds_tmp.push_back(kp(e, ii));
		double est_F = estimate_influence(seeds_tmp);
		++no_queries;
		if (max < est_F) {
			max = est_F;
			jj = ii;
		}
	}

	// no need to consider denoise step in deterministic
	//double eps = Constants::EPS / (Constants::NO_DENOISE_STEPS > 1 ? Constants::NO_DENOISE_STEPS - 1 : Constants::NO_DENOISE_STEPS) * step;
	double eps = Constants::EPS;

	if (max / (1 - eps) >= (sub_seeds[j][step].size() + 1) * thresholds[j]) {
		if (max > max_solution)
			max_solution = max;
		return jj;
	}
	else return -1;
}
