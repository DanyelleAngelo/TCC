#include <iostream>
#include <fstream>
#include <sdsl/int_vector.hpp>
#include "read_bp.h"

using namespace std;
using namespace sdsl;

int main(int argc, char *argv[]){
    int_vector<1> v;
    parentheses_to_bits(argv[1], v);
    cout << v << "\n";
    cout << v.size() << "\n";
    return 0;
}