// #include "node.h"
#include <algorithm>
#include <string>
#include <iostream>
#include "tree.h"
#include "write.h"

using namespace std;

char white_space = ' ';

TREE::TREE() {
    Root = nullptr;
}

TREE::TREE(int value) {
    Root = new node_t {.value = value, .height = 1, .right = nullptr, .left = nullptr};
}

void TREE::insert_node(int value) {
    /* Root case */
    if (Root == nullptr) {
        Root = new node_t {.value = value, .height = 1, .right = nullptr, .left = nullptr};
        write(to_string(value) + ' ');
    }
    else {
        insert_node_helper(Root, value);
    }
    /* Check balance after every insertion */
    rebalance();
}

void TREE::insert_node_helper(node_t* node, int value) {
    /* Case already exists */
    if (node->value == value) {
        write("Value already in tree: '" + to_string(value) + "' ");
        return;
    }
    /* Case right */
    else if (value > node->value) {
        /* Found the place for the new node */
        if (node->right == nullptr) {
            write(to_string(value) + ' ');
            node->right = new node_t;
            node->right->value = value;
            node->right->height = 1;
            node->right->right = nullptr;
            node->right->left = nullptr;
        }
        /* Try again on the right child */
        else {
            return insert_node_helper(node->right, value);
        }
    }
    /* Case left */
    else  { //value < node->value
        /* Found the place for the new node */
        if (node->left == nullptr) {
            write(to_string(value) + ' ');
            node->left = new node_t;
            node->left->value = value;
            node->left->height = 1;
            node->left->right = nullptr;
            node->left->left = nullptr;
        }
        /* Try again on the left child */
        else {
            return insert_node_helper(node->left, value);
        }
    }
}

void TREE::delete_node(int value) {
    /* Root case */
    if (Root->value == value) {
        if (Root->left != nullptr) {
            /* The left right node is blank, take the left node value */
            if (Root->left->right == nullptr) {
                if (Root->left->left != nullptr) {
                    Root->value = Root->left->left->value;
                    delete Root->left->left;
                    Root->left->left = nullptr;
                }
                else {
                    Root->value = Root->left->value;
                    delete Root->left;
                    Root->left = nullptr;
                }
            }
            /* Get the next biggest number */
            else {
                node_t* current = Root->left;
                while (current->right->right != nullptr) {
                    current = current->right;
                }
                Root->value = current->right->value;
                delete current->right;
                current->right = nullptr;
            }
        }
        else if (Root->right != NULL) {
            /* The right left node is blank take the right node value */
            if (Root->right->left == nullptr) {
                if (Root->right->right != nullptr) {
                    Root->value = Root->right->right->value;
                    delete Root->right->right;
                    Root->right->right = nullptr;
                }
                else {
                    Root->value = Root->right->value;
                    delete Root->right;
                    Root->right = nullptr;
                }
            }
            /* Get the next smallest number */
            else {
                node_t* current = Root->right;
                while (current->left->left != nullptr) {
                    current = current->left;
                }
                Root->value = current->left->value;
                delete current->left;
                current->left = nullptr;
            }
        }
        else {
            delete Root;
            Root = nullptr;
        }
    }
    /* Not the root, recurse until found */
    else {
        delete_node_helper(Root, value);
    }

    /* Should be balanced, but just as a precaution */
    rebalance();
}

void TREE::delete_node_helper(node_t* node, int value) {
    //assuming that node->value != value, should be handled before passing to that node
    //assuming node isn't nullptr

    if (node == nullptr) {
        return;
    }

    /* Right See Root Comments as well*/
    if (value > node->value) {
        if (node->right == nullptr) {
            write("1Couldn't delete value: " + to_string(value));
            return;
        }
        if (node->right->value == value) {
            //Found the value on the right
            //  case take the largest value on the left side
            if (node->right->left != nullptr) {
                if (node->right->left->right == nullptr) {
                    node->right->value = node->right->left->value;  //Store the new value
                    delete node->right->left;                       //Delete the memory allocation
                    node->right->left = nullptr;                       //Point the node pointer to nullptr
                }
                else { //node->right->left->right != nullptr
                    node_t* current = node->right->left;
                    while (current->right->right != nullptr) {
                        current = current->right;
                    }
                    node->right->value = current->right->value;
                    delete current->right;
                    current->right = nullptr;
                }
            }
            //  case take the smallest value from the right side
            else if (node->right->right != nullptr) {
                if (node->right->right->left == nullptr) {
                    node->right->value = node->right->right->value;
                    delete node->right->right;
                    node->right->right = nullptr;
                }
                else {
                    node_t* current = node->right->right;
                    while (current->left->left != nullptr) {
                        current = current->left;
                    }
                    node->right->value = current->left->value;
                    delete current->left;
                    current->left = nullptr;
                }
            }
            //  case no children, just delete it
            else {
                delete node->right;
                node->right = nullptr;
            }
        }
        else {
            delete_node_helper(node->right, value);
        }
    }
    /* Left, see Root comments as well*/
    else { //value < node->value
        if (node->left == nullptr) {
            write("2Couldn't delete value: " + to_string(value));
            return;
        }

        if (node->left->value == value) {
            //Found the value on the right
            //  case take the largest value on the left side
            if (node->left->left != nullptr) {
                if (node->left->left->right == nullptr) {
                    node->left->value = node->left->left->value;  //Store the new value
                    delete node->left->left;                       //Delete the memory allocation
                    node->left->left = nullptr;                       //Point the node pointer to nullptr
                }
                else { //node->left->left->right != nullptr
                    node_t* current = node->left->left;
                    while (current->right->right != nullptr) {
                        current = current->right;
                    }
                    node->left->value = current->right->value;
                    delete current->right;
                    current->right = nullptr;
                }
            }
            //  case take the smallest value from the right side
            else if (node->left->right != nullptr) {
                if (node->left->right->left == nullptr) {
                    node->left->value = node->left->right->value;
                    delete node->left->right;
                    node->left->right = nullptr;
                }
                else {
                    node_t* current = node->left->right;
                    while (current->left->left != nullptr) {
                        current = current->left;
                    }
                    node->left->value = current->left->value;
                    delete current->left;
                    current->left = nullptr;
                }
            }
            //  case no children, just delete it
            else {
                delete node->left;
                node->left = nullptr;
            }
        }
        else {
            delete_node_helper(node->left, value);
        }
    }
}

bool TREE::find_node(int value) {
    /* Get the node */
    node_t temp = *get_node(Root, value);
    
    /* Compare the node */
    if (temp.value == value) {
        return true;
    }
    else {
        return false;
    }
}

bool TREE::check_balance() {
    /* Guaruntee that heights are correct */
    reset_heights(Root);
    /* Check the balance */
    return check_balance_helper(Root);
}

bool TREE::check_balance_helper(node_t* node) {
    /* If we accidentally passed to a nullptr */
    if (node == nullptr) {
        return true;
    }

    /* Store the heights */
    int left_height = get_height(node->left);
    int right_height = get_height(node->right);

    /* Compare the heights */
    if ((abs(node->height - left_height) > 2) || (abs(node->height - right_height) > 2)) {
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
    if (Root != nullptr) {
        traverse_pre_helper(Root, "");
    }
    else {
        write("No nodes in tree\n");
    }
}

void TREE::traverse_pre_helper(node_t* node, string tabs) {
    //self
    write(to_string(node->value) + ' ');

    //left
    if (node->left != nullptr) {
        traverse_pre_helper(node->left, tabs + white_space);
    }

    //right
    if (node->right != nullptr) {
        traverse_pre_helper(node->right, tabs + white_space);
    }
}

void TREE::traverse_in() {
    if (Root != nullptr) {
        traverse_in_helper(Root, "");
    }
    else {
        write("No nodes in tree\n");
    }
}

void TREE::traverse_in_helper(node_t* node, string tabs) {
    //left
    if (node->left != nullptr) {
        traverse_in_helper(node->left, tabs + white_space);
    }
    
    //self
    write(to_string(node->value) + ' ');

    //right
    if (node->right != nullptr) {
        traverse_in_helper(node->right, tabs + white_space);
    }
}

void TREE::traverse_post() {
    if (Root != nullptr) {
        traverse_post_helper(Root, "");
    }
    else {
        write("No nodes in tree\n");
    }
}

void TREE::traverse_post_helper(node_t* node, string tabs) {
    //left
    if (node->left != nullptr) {
        traverse_post_helper(node->left, tabs + white_space);
    }

    //right
    if (node->right != nullptr) {
        traverse_post_helper(node->right, tabs + white_space);
    }

    //self
    write(to_string(node->value) + ' ');
}

void TREE::rotate_right(node_t* node) {
    // node_t* y = node;
    node_t* x = node->left;
    // node_t* x_r = node->left->right;

    node->left = node->left->right;
    x->right = node;
    
    reset_heights(node);

    node = x;
}

void TREE::rotate_left(node_t* node) {
    // node_t* x = node;
    node_t* y = node->right;
    // node_t* y_l = node->right->left;

    node->right = node->right->left;
    y->left = node;

    reset_heights(node);

    node = y;
}

node_t* TREE::get_node(node_t* node, int value) {
    /* Check self */
    if (value == node->value) {
        return node;
    }
    /* Check right */
    else if (value > node->value) {
        if (node->right == nullptr) {
            return node;
        }
        else {
            return get_node(node->right, value);
        }
    }
    /* Check left */
    else {  //value < node->value
        if (node->left == nullptr) {
            return node;
        }
        else {
            return get_node(node->left, value);
        }
    }
}

int TREE::get_height(node_t* node) {
    if (node == nullptr) {
        return 0;
    }
    else {
        return node->height;
    }
}

void TREE::reset_heights(node_t* node) {
    if (node == nullptr) {
        return;
    }
    //No children
    else if (node->right == nullptr && node->left == nullptr) {
        node->height = 1;
    }
    //Two children
    else if (node->right != nullptr && node->left != nullptr) {
        reset_heights(node->right);
        reset_heights(node->left);
        node->height = max(node->right->height, node->left->height) + 1;
    }
    //Right
    else if (node->right != nullptr && node->left == nullptr) {
        reset_heights(node->right);
        node->height = node->right->height + 1;
    }
    //Left
    else if (node->right == nullptr && node->left != nullptr) {
        reset_heights(node->left);
        node->height = node->left->height + 1;
    }
    else {
        write("Unexpected error while calculating heights");
    }
}

void TREE::rebalance() {
    reset_heights(Root);
    // rebalance_helper(Root);
}

void TREE::rebalance_helper(node_t* node) {
    if (node == nullptr) {
        return;
    }

    /* If right has a height too large, rotate */
    if (node->right != nullptr) {
        if ((node->height - node->right->height) > 2) {
            rotate_left(node);
        }
        else {
            rebalance_helper(node->right);
        }
    }
    else { //node->right == nullptr
        if (node->height > 2) {
            rotate_right(node);
        }
    }

    /* If left has a height too large, rotate */
    if (node->left != nullptr) {
        if ((node->height - node->left->height) > 2) {
            rotate_right(node);
        }
        else {
            rebalance_helper(node->left);
        }
    }
    else {  //node->left == nullptr
        if (node->height > 2) {
            rotate_left(node);
        }
    }
}

void TREE::print_heights() {
    reset_heights(Root);
    print_heights_helper(Root,"");
}

void TREE::print_heights_helper(node_t* node, string tabs) {
    if (node == nullptr) {
        return;
    }
    //self
    write(tabs + to_string(node->value) + ' ' + to_string(node->height) + '\n');

    //left
    if (node->left != nullptr) {
        print_heights_helper(node->left, tabs + white_space);
    }

    //right
    if (node->right != nullptr) {
        print_heights_helper(node->right, tabs + white_space);
    }
}