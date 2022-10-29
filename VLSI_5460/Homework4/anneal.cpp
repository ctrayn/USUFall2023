#include "anneal.h"
#include "npe.h"
#include "constants.h"
#include "node.h"
#include <vector>
#include <iostream>
#include <string>
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
        return 1;
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
    int rand_idx;
    int M = abs((rand() * 10) % 3);
    switch(M) {
        case 0: //Two operands
            first = get_rand_operand_idx(NewE, nodes);
            second = -1;
            while ((second < 0) && (second != first)) {
                second = get_rand_operand_idx(NewE, nodes);
            }
            swap(NewE[first], NewE[second]);
            break;

        case 1: //Two operators
            rand_idx = abs((rand() * 100) % (int)(NewE.size() / 2));
            for(int i = rand_idx; i < NewE.size(); i++) {
                if (NewE[i] == 'V') {
                    NewE[i] = 'H';
                    break;
                }
                else if (NewE[i] == 'H') {
                    NewE[i] = 'V';
                    break;
                }
            }
            break;

        case 2: //Operands and operators
            done = false;
            pos = 0;
            while (!done) {
                //rand starting pos
                pos = abs((rand() * 100) % (int)(E.size() / 2));
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
            break;

        default:
            std::cout << "M out of bounds: " << M << endl;
            return "";
    }
    return NewE;
}

float get_init_temp(string E0, vector<node_t> nodes) {
    const int NUM_AVG = 10;
    float initial_cost = npe_cost(E0, nodes);
    int count = 0;
    string NewE;
    float summation;
    float NewCost;

    //Get 10 uphill moves
    while(count < NUM_AVG) {
        NewE = make_move(E0, nodes);
        NewCost = npe_cost(NewE, nodes) - initial_cost;
        if (NewCost > 0) {
            summation += NewCost;
            count++;
        }
    }
    summation /= NUM_AVG;   //Get the average of the differences
    float initial_temp = -summation / log(P); //log is the natural log
    return initial_temp;
}

string anneal(const string E0, vector<node_t> nodes) {
    string E = E0;
    string Best = E0;
    string NewE = E0;
    float T = t0;
    if (T < 0) {
        T = get_init_temp(E0, nodes);
    }
    int uphill = 0;
    int MT = 0;
    int Reject = 0;
    bool out_of_time = false;
    int delta_cost = 0;

    while ((safe_div(Reject, MT) > 0.95) && (T >= epsilon) && !out_of_time) {
        MT = uphill = Reject = 0;
        NewE = E;
        while ((uphill <= nmoves) && (MT <= 2 * nmoves)) {
            //make a change in the NPE string
            NewE = make_move(NewE, nodes);
            //Evaluate the move
            MT++;
            float new_cost = npe_cost(NewE, nodes);
            float old_cost = npe_cost(E, nodes);
            delta_cost = npe_cost(NewE, nodes) - npe_cost(E, nodes);
            if ((delta_cost < 0) || (rand() < exp(-delta_cost / T))) {
                if (delta_cost > 0) { uphill++; }
                //Accept NewE
                E = NewE;
                if (npe_cost(E, nodes) < npe_cost(Best, nodes)) {
                    //Save if is best so far
                    Best = E;
                }
            }
            else {
                Reject++;
            }
        }
        T *= ratio;
    }
    return Best;
}
