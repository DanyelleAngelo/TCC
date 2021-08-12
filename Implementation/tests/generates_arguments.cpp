#include "gtest/gtest.h" 
#include <iostream>
#include <vector>
#include "read_bp/read_bp.h" 
#include "generates_arguments.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>

using namespace std;
using namespace sdsl;

int_vector<1> v;
int iterations;
vector<long long int> args_par_close;
vector<long long int> args_par_open;
vector<long long int> args_isLeaf;
vector<long long int> args_ancestor;
vector<long long int> args_rand_I;
vector<long long int> args_rand_II;
vector<long long int> args_excluding0;
vector<long long int> args_select;

void ArgumentsParOpen(){
	srand(v.size()/24);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size()-2);
		if(v[k]==1){
			args_par_open.push_back(k);
			i++;
		}
	}
}

void ArgumentsParClose(){
	srand(v.size()/20);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size()-1);
	    if(v[k]==0){
			args_par_close.push_back(k);
			i++;
		}
	}
}

void ArgumentsIsLeaf(){
	srand(v.size()/2);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size()-1);
	    if(v[k]==1){
			args_isLeaf.push_back(k);
			i++;
		}
	}
}

void ArgumentsIsAncestor(){
    srand(v.size()/4);
	int k_i,k_j,i=0;
    while(i<iterations){
        k_i = rand()%(v.size()-1);
        k_j = rand()%(v.size()-1);
        if(k_i < k_j){
            args_ancestor.push_back(k_i);
            args_ancestor.push_back(k_j);
            i++;
        }
    }
}

void ArgumentsRand_I(){
	srand(v.size()/8);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size()-1);
	    args_rand_I.push_back(k);
		i++;
	}
}

void ArgumentsRand_II(){
	srand(v.size()/16);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size()-1);
	    args_rand_II.push_back(k);
		i++;
	}
}

void ArgumentsExcluding0(){
	srand(v.size()/28);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size()-2)+1;
	    args_excluding0.push_back(k);
		i++;
	}
}

void ArgumentsSelect(){
	int k,i=0;
	while(i<iterations){
		k = rand()%((v.size()/2)-1);
	    args_select.push_back(k);
		i++;
	}
}

void Initialize(char *file){
    parentheses_to_bits(file,v);
    ArgumentsParOpen();
    ArgumentsParClose();
	ArgumentsRand_I();
	ArgumentsRand_II();	
	ArgumentsIsLeaf();
	ArgumentsIsAncestor();
	ArgumentsExcluding0();
	ArgumentsSelect();
}