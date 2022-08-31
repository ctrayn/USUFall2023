// #include "node.h"
#include "tree.h"
#include <algorithm>

TREE::TREE() {
    Root = NULL;
}

TREE::TREE(int value) {
    Root.value = value;
    Root.left = NULL;
    Root.right = NULL;
}

TREE::insert_node(int value) {
    if (Root == NULL) {
        Root = new node_t;
        Root.value = value;
    }
    else {
        node_t* closest = get_node(Root, value);
        if (closest->value == value) {
            cout << "Value already in tree: " << value << endl;
        }
        else if (value > closest->value) {
            closest->right = new node_t;
            closest->right.value = value;
        }
        else {  //value < closest->value
            closest->left = new node_t;
            closest->left.value = value;
        }
    }

    rebalance();
}

TREE::delete_node(int value) {
    //TODO
}

TREE::set_heights() {
    set_heights_helper(Root);
}

TREE::set_height_helper(node_t* node) {
    int right_val = -1;
    int left_val = -1;
    
    if (node->right == NULL) {
        right_val = 0;
    }
    else {
        right_val = set_height_helper(node->right);
    }

    if (node->left == NULL) {
        left_val = 0;
    }
    else {
        left_val = set_height_helper(node->left);
    }

    node->height = max(right_val, left_val);
    return node->height;
}

TREE::rotate_right(node* node) {
    node_t* left = node->left;
    node->left = left->right;
    left->right = node;
    node = left;
}

TREE::rotate_left(node* node) {
    node_t* right = node->right;
    node->right = right->left;
    right->left = node;
    node = right;
}

TREE::get_node(node_t* node, int value) {
    if (value == node.value) {
        return node;
    }
}