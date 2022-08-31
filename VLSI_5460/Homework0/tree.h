#ifndef TREE_H
#define TREE_H

// #include <vector>
#include "node.h"

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
        void set_heights();
        void set_height_helper(node_t node);
        void rotate_right(node_t* node);
        void rotate_left(node_t* node);
};

#endif //TREE_H