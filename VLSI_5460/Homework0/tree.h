#ifndef TREE_H
#define TREE_H

#include <string.h>
#include "node.h"
using namespace std;

class TREE {
    public:
        TREE();
        TREE(int value);

        void insert_node(int value);
        void delete_node(int value);
        // void delete_node(node_t value);
        node_t find_node(int value);
        bool check_balance();

        void traverse_pre();
        void traverse_post();
        void traverse_in();

        void print_heights();

    private:
        node_t* Root;
        void rebalance();
        void rebalance_helper(node_t* node);
        node_t* get_node(node_t* node, int value);
        void rotate_right(node_t* y);
        void rotate_left(node_t* y);

        void reset_heights(node_t* node);
        int get_height(node_t* node);

        void traverse_pre_helper(node_t* node, string tabs);
        void traverse_in_helper(node_t* node, string tabs);
        void traverse_post_helper(node_t* node, string tabs);
        bool check_balance_helper(node_t* node);
        void insert_node_helper(node_t* node, int value);
        void delete_node_helper(node_t* node, int value);

        void print_heights_helper(node_t* node, string tabs);
};

#endif //TREE_H