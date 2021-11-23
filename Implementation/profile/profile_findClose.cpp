#include "../rmm-tree-classic/rmMTreeClassic.h"
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "../rmm-tree-optimized/v1.h"
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
RMMTree_Bin *tBin;
RMMTree_Kary *tKary;
RMMTree_Kary_V1 *tKary_v1;

void generateArguments(vector<int> &vArgs){
    srand(size/32);
	int k;

    for(int i=0; i < iterations;){
        k = rand()%(size-2);
        if(tBin->bv[k]==1){
            vArgs.push_back(k);
            i++;
        }
    }
}

void findClose_binary_rmMTree(vector<int> vArgs){
    int j;
	for(int i=0;i<iterations;i++){
        j = tBin->findClose(vArgs[i]);
	}
}

void findClose_kary_rmMTree(vector<int> vArgs){
    int j;
    for(int i=0;i<iterations;i++){
        j = tKary->findClose(vArgs[i]);
    }
}

void findClose_kary_rmMTree_v1(vector<int> vArgs){
    int j;
    for(int i=0;i<iterations;i++){
        j = tKary_v1->findClose(vArgs[i]);
    }
}

int main(){
    vector<int> vArgs;
    int_vector<1> v;
    parentheses_to_bits("../dataset/prot.par", v);
    //parentheses_to_bits("../dataset/data.txt", v);
    size = v.size();

    /*building binary rmM-tree*/
    tBin = new RMMTree_Bin(v,sizeBlock,w);
    tBin->buildingTree();
    
    
    /*building k-ary rmM-tree*/
    tKary = new RMMTree_Kary(v,sizeBlock,w,4);
    tKary->buildingTree();

    /*building k-ary rmM-tree*/
    tKary_v1 = new RMMTree_Kary_V1(v,sizeBlock,w,4);
    tKary_v1->buildingTree();

    /*create arguments*/
    generateArguments(vArgs);

    /*Inicia o profile*/
    cout<< "--------------------Iniciando o profile de findClose para a estrutura binária."<<endl;
    ProfilerStart("profile_findClose.prof");
    findClose_binary_rmMTree(vArgs);

    cout<< "--------------------Iniciando o profile de findClose para a estrutura k-ária."<<endl;
    findClose_kary_rmMTree(vArgs);

    cout<< "--------------------Iniciando o profile de findClose para a estrutura k-ária - versao 1."<<endl;
    findClose_kary_rmMTree_v1(vArgs);
	ProfilerStop();
    return 0;
}
