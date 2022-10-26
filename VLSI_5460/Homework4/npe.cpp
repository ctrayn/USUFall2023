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

float npe_cost(string NPE, vector<node_t> nodes) {
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
        // printf("Current char %c ", current);

        if (current == 'V' || current == 'v') { //Vertical cut
            // printf("Vertical ");
            //take two off the stack and combine orientations
            first = stack.back();
            stack.pop_back();
            second = stack.back();
            stack.pop_back();
            sub_tree = node_t(second.id + first.id + current);
            
            int f_size = first.orientations.size();
            int s_size = second.orientations.size();
            // printf("\nFirst: %c\n", first.id[0]);
            // for (int i = 0; i < first.orientations.size(); i++) {
            //     printf("\t");
            //     first.orientations[i].print('\n');
            // }
            // printf("Second: %c\n", second.id[0]);
            // for (int i = 0; i < second.orientations.size(); i++) {
            //     printf("\t");
            //     second.orientations[i].print('\n');
            // }

            // cout << "Adding node to stack " << sub_tree.id << " with orientations: " << endl;
            for (int f = 0; f < f_size; f++) {
                for (int s = 0; s < s_size; s++) {
                    orient.height = max(first.orientations[f].height, second.orientations[s].height);
                    orient.width = first.orientations[f].width + second.orientations[s].width;
                    // printf("First width %f Second width %f, Orient width %f\n", first.orientations[f].width, second.orientations[s].width, orient.width);
                    sub_tree.orientations.push_back(orient);
                    // orient.print('\n');
                }
            }
            sub_tree.trim_orientations();
            // printf("\n");
            // for (int i = 0; i < sub_tree.orientations.size(); i++) {
                // printf("i %d\t", i);
                // sub_tree.orientations[i].print('\n');
            // }
            stack.push_back(sub_tree);
        }
        else if (current == 'H' || current == 'h') {    //Horizontal cut
            // printf("Horizontal ");
            //take two off the stack and combine orientations
            first = stack.back();
            stack.pop_back();
            second = stack.back();
            stack.pop_back();
            sub_tree = node_t(second.id + first.id + current);

            // cout << "Adding node to stack " << sub_tree.id << " with orientations: ";
            int f_size = first.orientations.size();
            int s_size = second.orientations.size();
            // printf("\nFirst: %c\n", first.id[0]);
            // for (int i = 0; i < first.orientations.size(); i++) {
                // printf("\t");
            //     first.orientations[i].print('\n');
            // }
            // printf("Second: %c\n", second.id[0]);
            // for (int i = 0; i < second.orientations.size(); i++) {
            //     printf("\t");
            //     second.orientations[i].print('\n');
            // }
            
            for (int f = 0; f < f_size; f++) {
                for (int s = 0; s < s_size; s++) {
                    orient.height = first.orientations[f].height + second.orientations[s].height;
                    // printf("First height %f Second Height %f, Orient Height %f\n", first.orientations[f].height, second.orientations[s].height, orient.height);
                    orient.width = max(first.orientations[f].width, second.orientations[s].width);
                    sub_tree.orientations.push_back(orient);
                    // orient.print('\n');
                }
            }
            sub_tree.trim_orientations();
            // printf("\n");
            // for (int i = 0; i < sub_tree.orientations.size(); i++) {
                // printf("i %d\t", i);
                // sub_tree.orientations[i].print('\n');
            // }
            stack.push_back(sub_tree);
        }
        else {
            // printf("Operand ");
            // Is an operand, put it onto the stack
            int nodes_size = nodes.size();
            bool found = false;
            for (int n = 0; n < nodes_size; n++) {
                // cout << "N " << n << " node " << nodes[n].id << endl;
                if (nodes[n].id[0] == current) {
                    // printf(" Found");
                    stack.push_back(nodes[n]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                // printf("Error: node not found: %c\n", current);
                return -1;
            }
        }
        // printf("\n");
    }

    node_t final = stack.back();
    stack.pop_back();
    assert(stack.empty());

    float min_area = final.orientations[0].height * final.orientations[0].width;
    for (int i = 0; i < final.orientations.size(); i++) {
        float area = final.orientations[i].height * final.orientations[i].width;
        min_area = min(min_area, area);
        // printf("Orientation %f-%f Area %f\n", final.orientations[i].height, final.orientations[i].width, area);
    }
    printf("Min Area: %f\n", min_area);
    
    return min_area;
}
