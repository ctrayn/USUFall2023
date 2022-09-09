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
    cout << "Choose a number from the following options, followed by any arguments:" << endl;
    cout << "\t(1) Create_Tree" << endl;
    cout << "\t(2) Insertion" << endl;
    cout << "\t(3) Deletion" << endl;
    cout << "\t(4) Search" << endl;
    cout << "\t(5) Traversal" << endl;
    cout << "\t(6) Delete_Tree" << endl;
    cout << "\t(7) Check_Balance" << endl;
    cout << "\t(q) Quit" << endl;
}

void print_prompt() {
    cout << "tree $";
}

void print_help() {
    cout << "Please enter choices in the following manner" << endl;
    cout << "\t<option number> <arguments>" << endl;
    cout << "\tie: 1 input.txt" << endl;
}

int main() {
    /* Set up variables */
    string selection;
    char key;
    string value;
    string line;
    ifstream in_file;
    TREE tree;

    /* Open the output file as a global variable */
    out_file.open("output.txt");
    print_menu();

    while (true) {
        /* Capture input */
        print_prompt();
        std::getline(cin, selection);
        
        /* Parse input */
        key = selection[0];

        if (selection.find(' ') == string::npos) {
            value = "";
        }
        else {
            value = selection.substr(selection.find(' ') + 1);
        }

        /* Execute commands */
        switch(key) {
            case '1':   // Create Tree
                if (value == "") {
                    cout << "Please list the input file" << endl;
                }
                else {
                    in_file.open(value);
                    // write("Opening file");
                    if (in_file.is_open()) {
                        // write("File is open");
                        write("Create Tree: ");
                        tree = TREE();
                        while (std::getline(in_file, line)) {
                            try {
                                // write("Inserting node " + line);
                                tree.insert_node(stoi(line));
                            }
                            catch(std::invalid_argument& e) {
                                write("\nError: ");
                                write(e.what());
                                write(" with value : " + line + "\n");
                            }
                        }
                        in_file.close();
                        write("\n");
                        tree.traverse_pre();
                        write("\n");
                    }
                    else {
                        write("Couldn't open file: " + value + "\n");
                    }
                }
                break;

            case '2':   //Insertion
                try {
                    write("Insertion: ");
                    tree.insert_node(stoi(value));  //Try catch to verify that input value is a number
                    write("\n");
                    tree.traverse_pre();
                    write("\n");
                }
                catch(std::invalid_argument& e) {
                    write("\nError: ");
                    write(e.what());
                    write(" with value : " + value + "\n");
                }
                break;
            
            case '3':   //Deletion
                try {
                    write("Delete " + value);
                    tree.delete_node(stoi(value));  //Try catch to verify that input value is a number
                    write(": success\n");
                    tree.traverse_pre();
                    write("\n");
                }
                catch(std::invalid_argument& e) {
                    write("\nError: ");
                    write(e.what());
                    write(" with value : " + value + "\n");
                }
                break;

            case '4':   //Search
                try {
                    write("Search " + value);
                    if(tree.find_node(stoi(value))) {
                        write(": Found\n");
                    }
                    else {
                        write(": Not Found\n");
                    }
                }
                catch(std::invalid_argument& e) {
                    write("\nError: ");
                    write(e.what());
                    write(" with value : " + value + "\n");
                }
                break;

            case '5':   //Traversal
                if (value == "pre"){    //Pre_order
                    write("Pre_order: ");
                    tree.traverse_pre();
                    write("\n");
                }
                else if (value == "post") { //Post_order
                    write("Post_order: ");
                    tree.traverse_post();
                    write("\n");
                }
                else if (value == "in") {   //In_order
                    write("In_order: ");
                    tree.traverse_in();
                    write("\n");
                }
                break;

            case '6':   //Delete Tree
                write("Delete Tree:");
                tree = TREE();
                write(" Tree Deleted\n");
                break;

            case '7':   //Check Balance
                write("Check Balance: ");
                if (tree.check_balance()) {
                    write("1\n");
                }
                else {
                    write("0\n");
                }
                break;

            case 'q':   //Save file and close program
                out_file.close();
                return 0;
                break;

            default:
                write("Input not recognized: " + selection);
                print_help();
                print_menu();
                break;
        }
        // write("\n");
    }

    return 0;
}