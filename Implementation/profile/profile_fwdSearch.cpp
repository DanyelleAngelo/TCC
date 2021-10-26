#include "../rmm-tree-classic/rmMTreeClassic.h"
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "../read_bp/read_bp.h"

#include <iostream>
#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include<gperftools/profiler.h>

using namespace std;
using namespace sdsl;

int iterations = 25000;
int sizeBlock=32;
int w=16;
int size =0;
RMMTree_Bin *tBin;
RMMTree_Kary *tKary;

int calc(int a){
	for(int i=0; i < 100000000;i++){
		a++;
	}
	return a;
}
int calc2(int a){
	for(int i=0; i < 100000000;i+=2){ 
		a++;
	}
	return a;
}

void generateArguments(vector<int> &vArgs, vector<int> &d){
    srand(size/8);
	int k,depthI;

    for(int i=0; i < iterations; i++){
        k = rand()%(size-2);
        vArgs.push_back(k);
        depthI = tBin->depth(vArgs[i]);
        d.push_back( (rand()%(tBin->tree[0].excessMax -1 - depthI)) - depthI);
    }
}

void fwdSearch_binary_rmMTree(vector<int> vArgs, vector<int> d){
    int j;
    for(int i=0;i<iterations;i++){
        j = tBin->fwdSearch(vArgs[i],d[i]);
    }
    //int h= calc(0);
}

void fwdSearch_kary_rmMTree(vector<int> vArgs, vector<int> d){
    int j;
    for(int i=0;i<iterations;i++){
        j = tKary->fwdSearch(vArgs[i],d[i]);
    }
    //int h= calc2(0);
}

int main(){
    vector<int> vArgs, d;
    int_vector<1> v;
    parentheses_to_bits("data.txt", v);
    size = v.size();

    /*building binary rmM-tree*/
    tBin = new RMMTree_Bin(v,sizeBlock,w);
    tBin->buildingTree();
    
    
    /*building k-ary rmM-tree*/
    tKary = new RMMTree_Kary(v,sizeBlock,w,4);
    tKary->buildingTree();

    /*create arguments*/
    generateArguments(vArgs,d);

    /*Inicia o profile*/
    cout<< "--------------------Iniciando o profile de fwdSearch para a estrutura binária."<<endl;
    ProfilerStart("fwdSearch_binary.prof");
    fwdSearch_binary_rmMTree(vArgs,d);

    cout<< "--------------------Iniciando o profile de fwdSearch para a estrutura k-ária."<<endl;
   
    fwdSearch_kary_rmMTree(vArgs,d);
	ProfilerStop();
    return 0;
}