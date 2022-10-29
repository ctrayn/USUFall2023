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

        //If it's an operator
        if (current == 'V' || current == 'v' || current == 'H' || current == 'h') {
            num_operators++;
        }
        //Check for repeated operand names
        else if (find(operands.begin(), operands.end(), current) != operands.end()) {
            cout << "Found repeated operand: " << current << endl;
            return false;   //found the same operand twice in the string
        }
        //valid operand
        else {
            num_operands++;
            operands.push_back(current);
        }

        //Check if has the correct number of operands/operators
        if (num_operators >= num_operands) {
            // cout << "Too many operators in subtree up to idx " << i << endl;
            return false;
        }
    }

    // VLSI Textbook pg 1.9 (2)
    if (num_operands > num_operators) {
        return true;
    }
    else {
        // cout << "Too many operators" << endl;
        return false;
    }
}

pair_t npe_cost_ratio(string NPE, vector<node_t> nodes) {
    // cout << "Starting cost for NPE " << NPE << endl;
    vector<node_t> stack = {};
    node_t first;
    node_t second;
    node_t sub_tree;
    pair_t orient;

    //Check if the NPE is valid
    if (!check_balloting(NPE)) {
        // cout << "Not a valid NPE" << endl;
        pair_t bad_pair;
        bad_pair.width = 100;
        bad_pair.height = 100;
        return bad_pair;
    }
    // else {
    //     cout << "Valid NPE" << endl;
    // }

    int npe_size = NPE.size();
    char current;
    for (int i = 0; i < npe_size; i++) {        //For each characeter in the string
        current = NPE[i];
        // printf("Current char %c\n", current);

        if (current == 'V' || current == 'v') { //Vertical cut
            //take two off the stack and combine orientations
            first = stack.back();
            stack.pop_back();
            second = stack.back();
            stack.pop_back();
            sub_tree = node_t(second.id + first.id + current);
            
            int f_size = first.orientations.size();
            int s_size = second.orientations.size();

            //Add all possible orientations of these two nodes to the new subtree node
            for (int f = 0; f < f_size; f++) {
                for (int s = 0; s < s_size; s++) {
                    orient.height = max(first.orientations[f].height, second.orientations[s].height);
                    orient.width = first.orientations[f].width + second.orientations[s].width;
                    sub_tree.orientations.push_back(orient);
                }
            }
            //Remove excess orientations
            sub_tree.trim_orientations();
            stack.push_back(sub_tree);
        }
        else if (current == 'H' || current == 'h') {    //Horizontal cut
            //take two off the stack and combine orientations
            first = stack.back();
            stack.pop_back();
            second = stack.back();
            stack.pop_back();
            sub_tree = node_t(second.id + first.id + current);

            int f_size = first.orientations.size();
            int s_size = second.orientations.size();
            
            //Add all possible orientations of these two nodes to the new subtree node
            for (int f = 0; f < f_size; f++) {
                for (int s = 0; s < s_size; s++) {
                    orient.height = first.orientations[f].height + second.orientations[s].height;
                    orient.width = max(first.orientations[f].width, second.orientations[s].width);
                    sub_tree.orientations.push_back(orient);
                }
            }
            //Remove excess orientations
            sub_tree.trim_orientations();
            stack.push_back(sub_tree);
        }
        else {
            // Is an operand, put it onto the stack
            int nodes_size = nodes.size();
            bool found = false;
            for (int n = 0; n < nodes_size; n++) {  //find the operand node
                if (nodes[n].id[0] == current) {
                    stack.push_back(nodes[n]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                // printf("Error: node not found: %c\n", current);
                pair_t bad_pair;
                bad_pair.height = 1;
                bad_pair.width = -1;
                return bad_pair;
            }
        }
    }

    node_t final = stack.back();
    stack.pop_back();
    assert(stack.empty());

    //Get the minimum area from the possible orientations of the final tree
    float min_area = final.orientations[0].height * final.orientations[0].width;
    pair_t min_pair = {final.orientations[0].width, final.orientations[0].height};
    for (int i = 0; i < final.orientations.size(); i++) {
        float area = final.orientations[i].height * final.orientations[i].width;
        if (area < min_area) {
            min_pair.height = final.orientations[i].height;
            min_pair.width  = final.orientations[i].width;
            min_area = area;
        }
    }
    //Return the minimum area pair
    return min_pair;
}

/* Get the minimum area pair and turn it into a single area */
float npe_cost(string NPE, vector<node_t> nodes) {
    pair_t pair = npe_cost_ratio(NPE, nodes);
    float area = pair.height * pair.width;
    return area;
}
