#include <vector>
#include <iostream>
#include <fstream>
#include "node.h"
#include "npe.h"
using namespace std;

int main() {
    ifstream input ("input_file.txt");
    string line;
    string id;
    string area;
    string ratio;
    vector<node_t> nodes = {};

    while (getline(input,line)) {
        // cout << line << endl;
        id = line.substr(0, line.find("\t"));
        area = line.substr(line.find("\t") + 1, line.find("\t", line.find("\t") + 1) - 2);
        ratio = line.substr(line.find(area) + area.size() + 1);

        // cout << "'" << id << "' '" << area << "' '" << ratio << "'" << endl;

        nodes.push_back(node_t(id, stof(area), stof(ratio)));
    }

    cout << "*****************" << endl;
    cout << "Discovering Nodes" << endl;
    for (int i = 0; i < nodes.size(); i++) {
        cout << nodes[i].id << " " << endl;
    }
    cout << "*****************" << endl;

    // cost("213546H7VHVa8V9HcVHgHibdHkVHfeHVlHVjHVH", nodes);
    cost("21H", nodes);
    return 0;
}