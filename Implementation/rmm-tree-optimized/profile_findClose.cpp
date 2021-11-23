#include "rmMTreeOptimized.h"
#include "../read_bp/read_bp.h"

#include <iostream>
#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include<gperftools/profiler.h>

using namespace std;
using namespace sdsl;

int iterations = 100000;
int sizeBlock=32;
int w=16;
int size =0;
RMMTree_Kary *tKary;

int calc2(int a){
	for(int i=0; i < 100000000;i++){ 
		a++;
	}
	return a;
}

void generateArguments(vector<int> &vArgs){
    srand(size/32);
	int k;

    for(int i=0; i < iterations;){
        k = rand()%(size-2);
        if(tKary->bv[k]==1){
            vArgs.push_back(k);
            i++;
        }
    }
}

void findClose_kary_rmMTree(vector<int> vArgs){
    int j;
    for(int i=0;i<iterations;i++){
        j = tKary->findClose(vArgs[i]);
    }
}

int main(){
    vector<int> vArgs;
    int_vector<1> v;
    parentheses_to_bits("../dataset/prot.par", v);
    //parentheses_to_bits("../../dataset/data.txt", v);
    size = v.size();
    
    /*building k-ary rmM-tree*/
    tKary = new RMMTree_Kary(v,sizeBlock,w,4);
    tKary->buildingTree();

    /*create arguments*/
    generateArguments(vArgs);

    /*Inicia o profile*/
    cout<< "--------------------Iniciando o profile de findClose para a estrutura k-ária."<<endl;
    ProfilerStart("profile_findClose.prof");
    findClose_kary_rmMTree(vArgs);
	ProfilerStop();
    return 0;
}
