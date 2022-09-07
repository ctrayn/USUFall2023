#include <string>
#include <iostream>
#include <fstream>
using namespace std;

extern ofstream out_file;

void write(string text) {
    cout << text << endl;
    out_file << text << endl;
}