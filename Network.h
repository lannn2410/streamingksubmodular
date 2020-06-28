#pragma once
#include <vector>
#include <map>
#include "Kcommon.h"

using namespace std;



class Network
{
public:
	Network();
	~Network();
	int get_no_nodes();
	
	// for social network
	int get_out_degree(uint n);
	bool read_network_from_file(int no_nodes, string file, bool is_directed); 
	void generate_random_network(int no_nodes, double p, bool is_directed);
	uint sample_influence(const kseeds & seeds); // estimate influence, input is a list of pairs, each pair is (node_id, product)
	uint sample_influence_reverse(const kseeds & seeds); // estimate influence but use reverse sampling
	uint sample_influence_linear_threshold(const kseeds & seeds);

	// for sensor
	bool read_sensor_data(int no_nodes, string file);
	double get_entropy(const kseeds & seeds);
private:
	void clear();
	void recursive_entropy(int idx, const kseeds & seeds, double & re, double & prob); // used to calculate entropy

	uint number_of_nodes;
	Kcommon * common_instance;

	// for social network
	bool is_directed;
	map<uint, uint> map_node_id; // map from true id -> ordered id (used for read graph from file)
	vector<vector<uint>> out_neighbors, in_neighbors; // map from node_id -> list of out (in) neighbor of the node
	vector<vector<uint>> preferences; // map from node_id -> preferences on partition - this impacts the weight of an out-edge with adopting different product
	vector<double> probabilities; // map from preference -> probability to influence

	// for sensor data
	vector<ksensors> sensor_data; // map from loc id -> kind of sensor (0 temp, 1 humid, 2 light) -> bin 
	int max_no_bin; // no. bins
};

