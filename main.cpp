#include<iostream>
#include <omp.h>
#include <time.h>
#include <fstream>
#include <stdlib.h>
#include <string>


#include "Network.h"
#include "Constants.h"
#include "Greedy.h"
#include "Deterministic.h"
#include "Randomized.h"
#include "Degree.h"
#include "SingleGreedy.h"
#include "StreamingGreedy.h"

//#include <time.h>
#include <chrono>
#include <iomanip> // setprecision
#if defined(_WIN32)
#include<direct.h>
#else 
#include <sys/stat.h>
#include <sys/types.h>
#endif

using namespace std;

#pragma warning(disable: 4996) 
vector<uint> B_vals = { 15,20,25,30,35,40,45,50 };
//vector<uint> B_vals = { 50 };
// for sensor
//vector<uint> B_vals = { 5,6,7,8,9,10,11,12,13,14,15 };

//vector<uint> M_vals = {3,4,5};
vector<uint> M_vals = { 3 };
vector<double> Delta_vals = {1};
//vector<double> Delta_vals = { 0.5,1.0,1.5};
//vector<double> Delta_vals = { 1.5, 2.0, 3.0 };

Network * g;

void run_experiments(uint no_nodes, string file, bool is_directed, bool is_random = false) {
	long folder_prefix = time(NULL);
	string re_folder = "result/" + to_string(folder_prefix) + "_" + (is_random ? "random" : file) + "_"
		+ (is_directed ? "directed" : "undirected");
	
	g = new Network();

	if (Constants::DATA == Social) {
		if (!is_random)
			g->read_network_from_file(no_nodes, "data/" + file, is_directed);
	}
	else
		g->read_sensor_data(no_nodes, "data/" + file);
			
	#if defined(_WIN32)
		mkdir(re_folder.c_str());
	#else 
		mkdir(re_folder.c_str(), 0777); // notice that 777 is different than 0777
	#endif

	string sol_output_file = re_folder + "/result.csv";
	string query_output_file = re_folder + "/query.csv";
	ofstream sol_writefile(sol_output_file), query_writefile(query_output_file);
	if (sol_writefile.is_open() && query_writefile.is_open()) {
		// header
		if (is_random) {
			sol_writefile << "p,";
			query_writefile << "p,";
		}
		sol_writefile << "B,";
		query_writefile << "B,";

		
		for (uint m : M_vals) {
			for (double d : Delta_vals) {
				sol_writefile << "ra_" << m << "_" << d << ",";
				query_writefile << "ra_" << m << "_" << d << ",";
				/*sol_writefile << "de_" << m << "_" << d << ",";
				query_writefile << "de_" << m << "_" << d << ",";*/
				/*sol_writefile << "de_" << m << "_" << d << "," << "ra_" << m << "_" << d << ",";
				query_writefile << "de_" << m << "_" << d << "," << "ra_" << m << "_" << d << ",";*/
			}
		}
		
		//sol_writefile << "gr,";

		/*
		for (int ii = 0; ii < Constants::K; ++ii) {
			sol_writefile << "sg_" << ii << ",";
		}*/
		//sol_writefile << "s_gr,";
		
		sol_writefile << endl;
		
		query_writefile << endl;
			

		// body content
		if (!is_random) {
			for (uint b : B_vals) {
				Constants::BUDGET = b;
				sol_writefile << Constants::BUDGET << ",";
				query_writefile << Constants::BUDGET << ",";
				cout << "B " << b << " K " << Constants::K << endl;

				
				for (uint m : M_vals) {
					for (double d : Delta_vals) {
						Constants::M = m;
						Constants::DELTA = d;

						/*Constants::NO_DENOISE_STEPS = 1;
						Deterministic * de = new Deterministic(g);
						double de_sol = de->get_solution();
						sol_writefile << de_sol << ",";
						query_writefile << de->get_no_queries() << ",";
						cout << "de_" << m << "_" << d << ": " << de_sol << " " << de->get_no_queries() << endl;
						delete de;*/

						// test 10 times
						Constants::NO_DENOISE_STEPS = 2;
						Randomized * ra = new Randomized(g);
						double ra_sol = ra->get_solution(false);
						sol_writefile << ra_sol << ",";
						query_writefile << ra->get_no_queries() << ",";
						cout << "ra_" << m << "_" << d << ": " << ra_sol << " " << ra->get_no_queries() << endl;
						delete ra;
						
					}
				}

				// greedy
				/*Greedy * gr = new Greedy(g);
				double gr_sol = gr->get_solution();
				sol_writefile << gr_sol << ",";
				cout << "greedy: " << gr_sol << endl;
				delete gr;*/

				/*for (int ii = 0; ii < 5; ++ii) {
					StreamingGreedy * s_gr = new StreamingGreedy(g);
					double s_gr_sol = s_gr->get_solution();
					sol_writefile << s_gr_sol << ",";
					cout << "streaming greedy: " << s_gr_sol << endl;
					delete s_gr;
				}*/

				sol_writefile << endl;
				query_writefile << endl;

				cout << "--------------------------------------------------" << endl;
			}
		}
		
		sol_writefile.close();
		query_writefile.close();
	}
	delete g;
}


void print_help() {
	cout << "Options: " << endl;
	cout << "-f <data filename>" << endl
		<< "-V <size of V> # default: 50" << endl
		<< "-t <type of experiment, 0: influence maximization, 1: sensor placement> # default: 0" << endl
		<< "-k <value of k> # default: 3" << endl
		<< "-B <value of B> # default: 10" << endl
		<< "-M <value of M> # default: 3" << endl
		<< "-e <value of epsilon> # default: 0.5 for IM and 0 for sensor placement" << endl
		<< "-n <value of eta - denoise step for RStream> # default: 2" << endl
		<< "-g <value of gamma> # default: 1.0" << endl
		<< "-a <algorithm, 0: greedy, 1: DStream, 2: RStream, 3: SGr>  # default: 1, please use SSA source code for testing IM algorithm" << endl
		<< "-p <number of threads to running algorithms> # default: 4" << endl;
}

pair<string, int> parseArgs(int argc, char** argv) {
	string filename = "error";
	int no_nodes = 50;

	/*if (argc == 1) {
		print_help();
		return pair<string,int>(filename, no_nodes);
	}*/

	int i = 1;
	while (i < argc - 1) {
		string arg = argv[i];
		if (arg == "-f") {
			filename = argv[i + 1];
		}
		else if (arg == "-g" || arg == "-e") {
			string s_val = argv[i + 1];
			std::string::size_type sz;
			if (arg == "-g")
				Constants::DELTA = std::stod(s_val, &sz);
			else 
				Constants::EPS = std::stod(s_val, &sz);
		}
		else {
			string s_val = argv[i + 1];
			std::string::size_type sz;
			int val = std::stoi(s_val, &sz);
			if (arg == "-t") {
				Constants::DATA = val == 0 ? Social : Sensor;
				if(Constants::DATA == Sensor) {
					Constants::EPS = 0;
					Constants::NO_DENOISE_STEPS = 1;
				}
			}
			else if (arg == "-V") {
				no_nodes = val;
			}
			else if (arg == "-k") {
				Constants::K = val;
			}
			else if (arg == "-B") {
				Constants::BUDGET = val;
			}
			else if (arg == "-n") {
				Constants::NO_DENOISE_STEPS = val;
				if (Constants::DATA == Sensor) {
					Constants::NO_DENOISE_STEPS = 1;
				}
			}
			else if (arg == "-M") {
				Constants::M = val;
			}
			else if (arg == "-a") {
				switch (val)
				{
				case 0:
					Constants::ALGORITHM = aGreedy;
					break;
				case 1:
					Constants::ALGORITHM = Dstream;
					break;
				case 2:
					Constants::ALGORITHM = Rstream;
					break;
				case 3:
					Constants::ALGORITHM = SGr;
					break;
				default:
					Constants::ALGORITHM = Dstream;
					break;
				}
			}
			else if (arg == "-p") {
				Constants::NUM_THREAD = val;
			}
		}
		i += 2;
	}

	return pair<string,int>(filename, no_nodes);
}

void run_command(string filename, int no_nodes) {
	Network * g = new Network();
	bool r = false;
	if (Constants::DATA == Social) {
		r = g->read_network_from_file(no_nodes, filename, false);
	}
	else
		r = g->read_sensor_data(no_nodes, filename);

	if (!r) {
		cout << "Wrong file, format or arguments" << endl;
		print_help();
	}
	else {
		double sol = 0.0;
		int no_queries = 0;
		omp_set_num_threads(Constants::NUM_THREAD);
		switch (Constants::ALGORITHM) {
			case aGreedy: {
				Greedy * gr = new Greedy(g);
				sol = gr->get_solution();
				no_queries = Constants::BUDGET * Constants::K * no_nodes;
				delete gr;
				break;
			}
			case Rstream: {
				Randomized * ra = new Randomized(g);
				sol = ra->get_solution(false);
				no_queries = ra->get_no_queries();
				delete ra;
				break;
			}
			case SGr: {
				StreamingGreedy * s_gr = new StreamingGreedy(g);
				sol = s_gr->get_solution();
				no_queries = s_gr->get_no_queries();
				delete s_gr;
				break;
			}
			default: {
				Constants::NO_DENOISE_STEPS = 1;
				Deterministic * de = new Deterministic(g);
				sol = de->get_solution();
				no_queries = de->get_no_queries();
				delete de;
				break;
			}
		}
		if (Constants::DATA == Sensor) sol = sol / 100;
		cout << "Output: " << sol << endl
			<< "Number of queries: " << no_queries << endl;
	}
	delete g;
}


int main(int argc, char* argv[]) {
	srand(time(NULL));
	//omp_set_num_threads(Constants::NUM_THREAD);
	//run_experiments(4039, "facebook_combined.txt", false);
	//run_experiments(58, "sensor.txt", false);
	
	pair<string, int> r = parseArgs(argc, argv);
	if (r.first == "error") {
		cout << "Wrong file, format or arguments" << endl;
		print_help();
	} else 
		run_command(r.first, r.second);
	
	return 0;
}