#include <vector>
#include <iostream>
#include <fstream>
#include "node.h"
#include "npe.h"
#include "constants.h"
using namespace std;

int main(int argc, char *argv[]) {
    string line;
    string id;
    string area;
    string aspect_ratio;
    vector<node_t> nodes = {};
    srand(iseed);

    if (argc != 2) {
        cout << "Wrong number of arguments: " << argc << endl;
    }
    ifstream input (argv[1]);
    if (input.fail()) {
        cout << "Failed to open file: " << argv[1] << endl;
        return 1;
    }

    while (getline(input,line)) {
        // cout << line << endl;
        id = line.substr(0, line.find("\t"));
        area = line.substr(line.find("\t") + 1, line.find("\t", line.find("\t") + 1) - 2);
        aspect_ratio = line.substr(line.find(area) + area.size() + 1);

        // cout << "'" << id << "' '" << area << "' '" << aspect_ratio << "'" << endl;

        nodes.push_back(node_t(id, stof(area), stof(aspect_ratio)));
    }

    string E0 = nodes[0].id;
    for (int i = 1; i < nodes.size(); i++) {
        E0 += nodes[i].id;
        E0 += "V";
    }
    cout << "E0: " << E0 << endl;
    cout << "Initial Cost: " << npe_cost(E0, nodes) << endl;

    string Ef = anneal(E0, nodes);

    cout << "Ef: " << Ef << endl;
    cout << "Final Cost: " << npe_cost(Ef, nodes) << endl;
    return 0;
}