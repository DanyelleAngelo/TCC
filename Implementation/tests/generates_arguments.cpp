#include "gtest/gtest.h" 
#include <iostream>
#include <string>
#include <vector>
#include "read_bp/read_bp.h" 
#include <sdsl/bp_support_sada.hpp>
#include "generates_arguments.h"

using namespace sdsl; 
using namespace std; 

void ArgumentsParOpen(){
	srand(v.size()/24);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size());
		if(v[k]==1){
			args_par_open.push_back(i);
			i++;
		}
	}
}

void ArgumentsParClose(){
	srand(v.size()/20);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size());
	    if(v[k]==0){
			args_par_close.push_back(i);
			i++;
		}
	}
}

void ArgumentsIsLeaf(){
	srand(v.size()/2);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size());
	    if(v[k]==1){
			args_isLeaf.push_back(i);
			i++;
		}
	}
}

void ArgumentsIsAncestor(){
    srand(v.size()/4);
	int k_i,k_j,i=0;
    while(i<iterations){
        k_i = rand()%(v.size());
        k_j = rand()%(v.size());
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
		k = rand()%(v.size());
	    args_rand_I.push_back(i);
		i++;
	}
}

void ArgumentsRand_II(){
	srand(v.size()/16);
	int k,i=0;
	while(i<iterations){
		k = rand()%(v.size());
	    args_rand_II.push_back(i);
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
}