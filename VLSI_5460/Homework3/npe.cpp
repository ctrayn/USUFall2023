#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include "node.h"
using namespace std;

bool check_balloting(string NPE) {
    vector<char> operands = {};
    int num_operators = 0;
    int num_operands = 0;
    char current;
    int size = NPE.size();

    // VLSI Textbook pg 1.9 (1)
    for (int i = 0; i < size; i++) {
        current = NPE[i];

        if (current == 'V' || current == 'v' || current == 'H' || current == 'h') {
            num_operators++;
            continue;
        }

        if (find(operands.begin(), operands.end(), current) != operands.end()) {
            cout << "Found repeated operand: " << current << endl;
            return false;   //found the same operand twice in the string
        }
        else {
            num_operands++;
            operands.push_back(current);
        }
    }

    // VLSI Textbook pg 1.9 (2)
    if (num_operands > num_operators) {
        return true;
    }
    else {
        cout << "Too many operators" << endl;
        return false;
    }
}

int cost(string NPE, vector<node_t> nodes) {
    cout << "Starting cost for NPE " << NPE << endl;
    vector<node_t> stack = {};
    node_t first;
    node_t second;
    node_t sub_tree;
    pair_t orient;

    int npe_size = NPE.size();
    char current;
    for (int i = 0; i < npe_size; i++) {
        current = NPE[i];
        cout << "Current char " << current << endl;

        if (current == 'V' || current == 'v') { //Vertical cut
            cout << "Into Vertical" << endl;
            //take two off the stack and combine orientations
            first = stack.back();
            stack.pop_back();
            second = stack.back();
            stack.pop_back();
            sub_tree = node_t(second.id + first.id + current);
            
            int f_size = first.orientations.size();
            int s_size = second.orientations.size();
            for (int f = 0; f < f_size; f++) {
                for (int s = 0; f < s_size; f++) {
                    orient.height = max(first.orientations[f].height, second.orientations[s].height);
                    orient.width = first.orientations[f].width + second.orientations[s].width;
                    sub_tree.orientations.push_back(orient);
                }
            }
            sub_tree.trim_orientations();
            stack.push_back(sub_tree);
        }
        else if (current == 'H' || current == 'h') {    //Horizontal cut
            cout << "Into Horizontal" << endl;
            //take two off the stack and combine orientations
            first = stack.back();
            stack.pop_back();
            second = stack.back();
            stack.pop_back();
            sub_tree = node_t(second.id + first.id + current);

            int f_size = first.orientations.size();
            int s_size = second.orientations.size();
            for (int f = 0; f < f_size; f++) {
                for (int s = 0; s < s_size; s++) {
                    orient.height = first.orientations[f].height + second.orientations[s].height;
                    orient.width = max(first.orientations[f].width, second.orientations[s].width);
                    sub_tree.orientations.push_back(orient);
                }
            }
            sub_tree.trim_orientations();
            stack.push_back(sub_tree);
        }
        else {
            cout << "Into Operand" << endl;
            // Is an operand, put it onto the stack
            int nodes_size = nodes.size();
            bool found = false;
            for (int n = 0; n < nodes_size; n++) {
                // cout << "N " << n << " node " << nodes[n].id << endl;
                if (nodes[n].id[0] == current) {
                    cout << "Found " << current << endl;
                    stack.push_back(nodes[n]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Error: node not found: " << current << endl;
                return -1;
            }
        }
    }

    cout << NPE << endl;
    node_t final = stack.back();
    stack.pop_back();
    assert(stack.empty());

    float min_area = final.orientations[0].height * final.orientations[0].width;
    for (int i = 0; i < final.orientations.size(); i++) {
        float area = final.orientations[i].height * final.orientations[i].width;
        min_area = min(min_area, area);
        cout << "Orientation " <<  final.orientations[i].height << "-" << final.orientations[i].width<< endl;
    }
    cout << "Min Area: " << min_area << endl;
    
    return 0;
}
