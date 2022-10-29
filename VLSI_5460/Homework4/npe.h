#pragma once
#include <string>
using namespace std;

bool check_balloting(string NPE);
float npe_cost(string NPE, vector<node_t> nodes);
float npe_cost_ratio(string NPE, vector<node_t> nodes);