#include <vector>
#include <iostream>
#include <fstream>
#include "node.h"
#include "npe.h"
#include "constants.h"
using namespace std;

int get_rand_operand_idx(string E, vector<node_t> nodes) {
    int rand_max = 1000;

    int nodes_size = nodes.size();
    // cout << "Node size: " << nodes_size << endl;
    int rand_idx = abs((rand() * rand_max) % nodes_size);
    // cout << "Rand Index: " << rand_idx << endl;
    node_t node = nodes[rand_idx];
    // cout << "Got node" << endl;
    char operand = node.id[0];
    // cout << "Random operand: " << operand << endl;

    int string_size = E.size();
    for (int i = 0; i < string_size; i++) {
        if (E[i] == operand) {
            return i;
        }
    }
    return -1;
}

bool is_operator(char c) {
    if (c == 'V' || c == 'H') {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Divdes one/two
 * @return float quotient
 */
float safe_div(float one, float two) {
    if (one == 0 || two == 0) {
        return 0;
    }
    else {
        return one/two;
    }
}

string make_move(string E, vector<node_t> nodes) {
    string NewE = E;
    int first;
    int second;
    int C;
    bool done;
    int pos;
    int M = abs((rand() * 10) % 3);
    switch(M) {
        case 0: //Two operands
            std::cout << "Operands: " << E << endl;
            first = get_rand_operand_idx(NewE, nodes);
            // cout << "First " << first << endl;
            second = -1;
            while ((second < 0) && (second != first)) {
                // cout << "Trying Second" << endl;
                second = get_rand_operand_idx(NewE, nodes);
            }
            // cout << "Second " << second << endl;
            swap(NewE[first], NewE[second]);
            std::cout << "NewE:     " << NewE << endl;
            break;

        case 1: //Two operators
            std::cout << "Operators: " << E << endl;
            C = abs((rand() * 100) % (int)(NewE.size() / 2));
            for(int i = 0; i < NewE.size() && C > 0; i++) {
                if (NewE[i] == 'V') {
                    NewE[i] = 'H';
                    C--;
                }
                else if (NewE[i] == 'H') {
                    NewE[i] = 'V';
                    C--;
                }
            }
            std::cout << "NewE: " << NewE << endl;
            break;

        case 2: //Operands and operators
            std::cout << "Mixed: " << E << endl;
            done = false;
            pos = 0;
            while (!done) {
                //rand starting pos
                pos = abs((rand() * 100) % (int)(E.size() / 2));
                // cout << "Pos: " << pos << endl;
                if (pos < 2) {
                    pos = 2;    //Can't have the first two characters not be operands
                }
                int i;
                for (i = pos; i < NewE.size() - 1; i++) {
                    if (is_operator(NewE[i]) && !is_operator(NewE[i + 1]) ||
                        !is_operator(NewE[i]) && is_operator(NewE[i + 1])) {
                            done = true;
                            break;
                        }
                }
                if (done) {
                    swap(NewE[i], NewE[i + 1]);
                }
            }
            std::cout << "NewE:  " << NewE << endl;
            break;

        default:
            std::cout << "M out of bounds: " << M << endl;
            return "";
    }
    return NewE;
}

string anneal(const string E0, vector<node_t> nodes) {
    std::cout << "Anneal: " << E0 << endl;
    string E = E0;
    string Best = E0;
    string NewE = E0;
    float T = t0;
    if (T < 0) {
        T = 1000;   //TODO This should be calculated into a large number
    }
    int uphill = 0;
    int MT = 0;
    int Reject = 0;
    bool out_of_time = false;
    int delta_cost = 0;

    while ((safe_div(Reject, MT) <= 0.95) ){//|| (T > epsilon) || !out_of_time) {
        std::cout << "T: " << T << endl;
        MT = uphill = Reject = 0;
        // E = Best;
        while ((uphill <= nmoves) && (MT <= 2 * nmoves)) {
            NewE = make_move(E, nodes);
            MT++;
            cout << "Evaluating" << endl;
            float new_cost = npe_cost(NewE, nodes);
            cout << "New cost: " << new_cost << endl;
            float old_cost = npe_cost(E, nodes);
            cout << "Old cost: " << old_cost << endl;
            delta_cost = npe_cost(NewE, nodes) - npe_cost(E, nodes);
            cout << "delta_cost " << delta_cost << endl;
            //TODO: Track chip ratio
            if ((delta_cost < 0) || (rand() < exp(-delta_cost / T))) {
                if (delta_cost > 0) { uphill++; }
                //Accept NewE
                cout << "Accept NewE: " << NewE << endl;
                E = NewE;
                if (npe_cost(E, nodes) < npe_cost(Best, nodes)) {
                    //Save if is best so far
                    cout << "Best: " << E << endl;
                    Best = E;
                }
            }
            else {
                cout << "Rejected: " << NewE << endl;
                Reject++;
            }
        }
        T *= ratio;
    }
    return Best;
}

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