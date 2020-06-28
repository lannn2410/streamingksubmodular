#include "Randomized.h"
#include <stdlib.h>  
#include <math.h>

Randomized::Randomized(Network * g) : Streaming(g)
{
}

Randomized::~Randomized()
{
}

int Randomized::select_element(int j, uint e, int step)
{
	vector<double> inf(Constants::K), d(Constants::K);
	uint T = 0;


	double eps = Constants::NO_DENOISE_STEPS == 1 ? Constants::EPS : (Constants::EPS / (Constants::NO_DENOISE_STEPS - 1) * step);

	//double eps = Constants::EPS / (Constants::NO_DENOISE_STEPS > 1 ? Constants::NO_DENOISE_STEPS - 1 : Constants::NO_DENOISE_STEPS) * step;


	for (int i = 0; i < Constants::K; ++i) {
		kseeds tmp = sub_seeds[j][step];
		tmp.push_back(pair<uint, uint>(e, i));
		inf[i] = estimate_influence(tmp);
		++no_queries;
		
		// new probability
		d[i] = inf[i] / (1 - eps) - influences[j][step] / (1 + eps);
		d[i] = d[i] >= thresholds[j] ? d[i] : 0;

		// old probability
		//d[i] = inf[i] / (1 - eps) - influences[j][step] / (1 + eps) - thresholds[j];
		//d[i] = d[i] > 0 ? d[i] : 0;
		if (d[i] > 0) ++T;
	}

	if (T == 0) return -1;
	else if (T == 1) {
		for (int i = 0; i < Constants::K; ++i) {
			if (d[i] > 0) {
				influences[j][step] = inf[i];
				if (influences[j][step] > max_solution) max_solution = influences[j][step];
				return i;
			}
		}
	}
	else {
		double D = 0.0;
		for (int i = 0; i < Constants::K; ++i) {
			d[i] = pow(d[i], T - 1);
			D += d[i];
		}
		double sum = 0.0;
		double r = (double)(rand() % 10000) / 10000 * D;
		for (int i = 0; i < Constants::K; ++i) {
			if (sum <= r && r < sum + d[i]) {
				influences[j][step] = inf[i];
				if (influences[j][step] > max_solution) max_solution = influences[j][step];
				return i;
			}
			sum += d[i];
		}
	}

	return -1;
}
