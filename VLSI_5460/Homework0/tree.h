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

    private:
        node_t* Root;
        void rebalance();
        node_t* get_node(node_t* node, int value);
        node_t* rotate_right(node_t* y);
        node_t* rotate_left(node_t* y);
        int get_height(node_t* node);

        void traverse_pre_helper(node_t* node, string tabs);
        void traverse_in_helper(node_t* node, string tabs);
        void traverse_post_helper(node_t* node, string tabs);
        bool check_balance_helper(node_t* node);
        void insert_node_helper(node_t* node, int value);
        void delete_node_helper(node_t* node, int value);
};

#endif //TREE_H