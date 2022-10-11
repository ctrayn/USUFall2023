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

    // cout << "*****************" << endl;
    // cout << "Discovering Nodes" << endl;
    // for (int i = 0; i < nodes.size(); i++) {
        // cout << nodes[i].id << " " << endl;
    // }
    // cout << "*****************" << endl;

    float cost3a = npe_cost("12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlV", nodes);
    // printf("**********************************************\n");
    float cost3b = npe_cost("12H3H4H5H6H7H8H9HaHbHcHdHeHfHgHiHjHkHlH", nodes);
    // printf("**********************************************\n");
    float cost3c = npe_cost("213546H7VHVa8V9HcVHgHibdHkVHfeHVlHVjHVH", nodes);

    printf("\n");
    printf(" Problem # |   Cost   \n");
    printf("-----------|----------\n");
    printf("     3a    | %f \n", cost3a);
    printf("     3b    | %f \n", cost3b);
    printf("     3c    | %f \n", cost3c);
    return 0;
}