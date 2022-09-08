#include <string>
#include <iostream>
#include <fstream>
using namespace std;

extern ofstream out_file;

void write(string text) {
    cout << text;
    out_file << text;
}