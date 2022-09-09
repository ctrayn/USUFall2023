#include <string>
#include <iostream>
#include <fstream>
using namespace std;

extern ofstream out_file;

/* Wrote this to make writing to a file and console easier */
void write(string text) {
    cout << text;
    out_file << text;
}