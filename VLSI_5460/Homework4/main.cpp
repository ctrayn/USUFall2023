#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "node.h"
#include "npe.h"
#include "constants.h"
#include "anneal.h"
using namespace std;

int main(int argc, char *argv[]) {
    string line;
    string id;
    string area;
    string aspect_ratio;
    vector<node_t> nodes = {};
    srand(iseed);

    //Open the file
    if (argc != 2) {
        cout << "Wrong number of arguments: " << argc << endl;
    }
    ifstream input (argv[1]);
    if (input.fail()) {
        cout << "Failed to open file: " << argv[1] << endl;
        return 1;
    }

    //Read in the input
    while (getline(input,line)) {
        id = line.substr(0, line.find("\t"));
        area = line.substr(line.find("\t") + 1, line.find("\t", line.find("\t") + 1) - 2);
        aspect_ratio = line.substr(line.find(area) + area.size() + 1);

        nodes.push_back(node_t(id, stof(area), stof(aspect_ratio)));
    }

    //Make the initial NPE
    string E0 = nodes[0].id;
    for (int i = 1; i < nodes.size(); i++) {
        E0 += nodes[i].id;
        E0 += "V";
    }
    cout << "E0: " << E0 << endl;
    cout << "Initial Cost: " << npe_cost(E0, nodes) << endl;

    //Do the simulated annealing
    string Ef = anneal(E0, nodes);
    float NPE_ratio = npe_cost_ratio(Ef, nodes);

    //Check for chip ratio error
    if (((1/chipratio) <= NPE_ratio) || (NPE_ratio <= chipratio)) {
        cout << "\033[31mError\033[0m: Chip ratio not valid : " << NPE_ratio << ", please adjust starting parameters and rerun" << endl;
    }

    cout << "Ef: " << Ef << endl;
    cout << "Final Cost: " << npe_cost(Ef, nodes) << endl;
    return 0;
}