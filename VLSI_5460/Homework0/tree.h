#ifndef TREE_H
#define TREE_H

#include <vector>
#include "node.h"

class TREE {
    public:
        TREE();
        TREE(int value);

        void insert_node(node_t node);
        // void delete_node(int value); // This may be helpful
        void delete_node(node_t value);
        node_t find_node(int value);
        bool check_balance();

        void traverse_pre();
        void traverse_post();
        void traverse_in();

    private:
        node_t* Root;
        void rebalance();
};

#endif //TREE_H