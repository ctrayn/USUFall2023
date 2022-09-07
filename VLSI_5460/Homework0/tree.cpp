// #include "node.h"
#include <algorithm>
#include <string>
#include "tree.h"
#include "write.h"

using namespace std;

char white_space = ' ';

TREE::TREE() {
    Root = NULL;
}

TREE::TREE(int value) {
    Root = new node_t {.value = value, .height = 0, .right = NULL, .left = NULL};
}

void TREE::insert_node(int value) {
    if (Root == NULL) {
        Root = new node_t {.value = value, .height = 0, .right = NULL, .left = NULL};
    }
    else {
        insert_node_helper(Root, value);
    }
    
    rebalance();
}

void TREE::insert_node_helper(node_t* node, int value) {
    if (node->value == value) {
        write("Value already in tree: " + to_string(value));
        return;
    }
    else if (node->value > value) {
        if (node->right == NULL) {
            node->right = new node_t {.value = value, .height = 0, .right = NULL, .left = NULL};
        }
        else {
            return insert_node_helper(node->right, value);
        }
    }
    else  { //node->value < value
        if (node->left == NULL) {
            node->left = new node_t {.value = value, .height = 0, .right = NULL, .left = NULL};
        }
        else {
            return insert_node_helper(node->left, value);
        }
    }
}

void TREE::delete_node(int value) {
    if (Root->value == value) {
        //TODO: Deal with this
    }

    delete_node_helper(Root, value);
}

void TREE::delete_node_helper(node_t* node, int value) {
    //assuming that node->value != value, should be handled before passing to that node
    //assuming node isn't null

    if (node->value > value) {
        //Right
        if (node->right == NULL) {
            write("Couldn't delete value: " + to_string(value));
            return;
        }
        if (node->right->value == value) {
            if (node->right->left != NULL) {
                node_t* current = node->right->left;
                while (true) {
                    
                    current = current->right;
                    
                }
            }
        }
        else {
            delete_node_helper(node->right, value);
        }
    }
    else { //node->value < value
        //Left
        if (node->left == NULL) {
            write("Couldn't delete value: " + to_string(value));
            return;
        }

        if (node->left->value == value) {
            //TODO
        }
        else {
            delete_node_helper(node->left, value);
        }
        
    }
}

node_t TREE::find_node(int value) {
    return *get_node(Root, value);
}

bool TREE::check_balance() {
    return check_balance_helper(Root);
}

bool TREE::check_balance_helper(node_t* node) {
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);

    if ((abs(node->height - left_height) > 1) || (abs(node->height - right_height) > 1)) {
        return false;
    }
    else {
        if (check_balance_helper(node->left)) {
            return check_balance_helper(node->right);
        }
        else {
            write("Tree not balanced, rebalancing");
            rebalance();
            return false;
        }
    }
}

void TREE::traverse_pre() {
    if (Root != NULL) {
        traverse_pre_helper(Root, "");
    }
    else {
        write("No nodes in tree");
    }
}

void TREE::traverse_pre_helper(node_t* node, string tabs) {
    //self
    write(tabs + to_string(node->value));

    //left
    if (node->left != NULL) {
        traverse_pre_helper(node->left, tabs + white_space);
    }

    //right
    if (node->right != NULL) {
        traverse_pre_helper(node->right, tabs + white_space);
    }
}

void TREE::traverse_in() {
    if (Root != NULL) {
        traverse_in_helper(Root, "");
    }
    else {
        write("No nodes in tree");
    }
}

void TREE::traverse_in_helper(node_t* node, string tabs) {
    //left
    if (node->left != NULL) {
        traverse_in_helper(node->left, tabs + white_space);
    }
    
    //self
    write(tabs + to_string(node->value));

    //right
    if (node->right != NULL) {
        traverse_in_helper(node->right, tabs + white_space);
    }
}

void TREE::traverse_post() {
    traverse_post_helper(Root, "");
}

void TREE::traverse_post_helper(node_t* node, string tabs) {
    //left
    if (node->left != NULL) {
        traverse_post_helper(node->left, tabs + white_space);
    }

    //right
    if (node->right != NULL) {
        traverse_post_helper(node->right, tabs + white_space);
    }

    //self
    write(tabs + to_string(node->value));
}

node_t* TREE::rotate_right(node_t* y) {
    node_t* x = y->left;
    node_t* sub_x = x->right;

    //Do the rotation
    x->right = y;
    y->left = sub_x;

    //Update heights
    y->height = max(get_height(y->right), get_height(y->left)) + 1;
    x->height = max(get_height(x->right), get_height(x->left)) + 1;

    return x;
}

node_t* TREE::rotate_left(node_t* y) {
    node_t* x = y->right;
    node_t* sub_x = x->left;

    x->left = y;
    y->right = sub_x;

    y->height = max(get_height(y->right), get_height(y->left)) + 1;
    x->height = max(get_height(x->right), get_height(x->left)) + 1;

    return x;
}

node_t* TREE::get_node(node_t* node, int value) {
    if (value == node->value) {
        return node;
    }
    else if (value > node->value) {
        if (node->right == NULL) {
            return node;
        }
        else {
            return get_node(node->right, value);
        }
    }
    else {  //value < node->value
        if (node->left == NULL) {
            return node;
        }
        else {
            return get_node(node->left, value);
        }
    }
}

int TREE::get_height(node_t* node) {
    if (node == NULL) {
        return 0;
    }
    else {
        return node->height;
    }
}

void TREE::rebalance() {
    write("Rebalance"); //TODO
}