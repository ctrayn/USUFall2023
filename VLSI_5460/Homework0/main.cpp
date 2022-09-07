/*************************************************
 * Calvin Passmore
 * ECE 5460
 * Homework 0
 * September 8, 2023
 *************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include "write.h"
#include "tree.h"
using namespace std;

ofstream out_file;

void print_menu() {
    write("Choose a number from the following options, followed by any arguments:");
    write("\t(1) Create_Tree");
    write("\t(2) Insertion");
    write("\t(3) Deletion");
    write("\t(4) Search");
    write("\t(5) Traversal");
    write("\t(6) Delete_Tree");
    write("\t(7) Check_Balance");
}

void print_prompt() {
    write("tree $");
}

void print_help() {
    write("Please enter choices in the following manner");
    write("\t<option number> <arguments>");
    write("\tie: 1 input.txt");
}

int main() {
    string selection;
    char key;
    string value;
    string line;
    ifstream in_file;
    TREE tree;
    int num;

    out_file.open("output.txt");

    while (true) {
        print_menu();
        print_prompt();
        std::getline(cin, selection);
        key = selection[0];
        value = selection.substr(selection.find(' ') + 1);
        write("Got input: " + selection + " key: " + key + " value: " + value);

        switch(key) {
            case '1':   // Create Tree
                write("Create Tree");
                if (value == "") {
                    write("Please list the input file");
                }
                else {
                    in_file.open(value);
                    write("Opening file");
                    if (in_file.is_open()) {
                        write("File is open");
                        tree = TREE();
                        while (std::getline(in_file, line)) {
                            try {
                                write("Inserting node " + line);
                                tree.insert_node(stoi(line));
                            }
                            catch(std::invalid_argument& e) {
                                write(e.what());
                            }
                        }
                    }
                    else {
                        write("Couldn't open file: " + value);
                    }
                }
                break;

            case '2':
                write("Insertion");
                try {
                    write("Inserting node " + value);
                    tree.insert_node(stoi(value));
                }
                catch(std::invalid_argument& e) {
                    write(e.what());
                }
                break;
            
            case '3':
                write("Deletion");
                try {
                    tree.delete_node(stoi(value));
                }
                catch(std::invalid_argument& e) {
                    write(e.what());
                }
                break;

            case '4':
                write("Search");
                try {
                    tree.find_node(stoi(value));
                }
                catch(std::invalid_argument& e) {
                    write(e.what());
                }
                break;

            case '5':
                write("Traversal");
                write("Please choose from the following options");
                write("\t(1) Pre_order");
                write("\t(2) Post_order");
                write("\t(3) In_Order");
                print_prompt();
                std::getline(cin, selection);
                switch(selection[0]) {
                    case '1':
                        write("Pre_order");
                        tree.traverse_pre();
                        break;

                    case '2':
                        write("Post_order");
                        tree.traverse_post();
                        break;

                    case '3':
                        write("In_order");
                        tree.traverse_in();
                        break;
                }
                break;

            case '6':
                write("Delete Tree");
                tree = TREE();
                break;

            case '7':
                write("Check Balance");
                break;

            default:
                write("Input not recognized: " + selection);
                print_help();
                break;
        }
    }

    return 0;
}