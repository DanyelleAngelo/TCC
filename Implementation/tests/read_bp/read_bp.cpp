#include <iostream>
#include <fstream>
#include <sdsl/int_vector.hpp>
#include "read_bp.h"

using namespace std;
using namespace sdsl;

void parentheses_to_bits(const char* fn, int_vector<1> &v) {
    char parenthesis;
    long count = 0;
    
    FILE* fp = fopen(fn, "r");
    if (!fp) {
        exit(EXIT_FAILURE);
    }
    
    fseek(fp, 0L, SEEK_END);
    int n = ftell(fp);  
    fseek(fp, 0L, SEEK_SET);

    v.resize(n);
    while (fread(&parenthesis,sizeof(char),1,fp)==1) {
        if(parenthesis == '(')v[count]=1;
        else v[count]=0;
        count++;
    }
    fclose(fp);
}