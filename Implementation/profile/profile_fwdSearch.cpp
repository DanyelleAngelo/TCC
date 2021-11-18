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

int iterations = 100000;
int sizeBlock=32;
int w=16;
int size =0;
RMMTree_Bin *tBin;
RMMTree_Kary *tKary;

void generateArguments(vector<int> &vArgs, vector<int> &d){
    srand(size/8);
	int k,depth, close;

    for(int i=0; i < iterations; ){
        k = rand()%(size-2);
        if(tBin->bv[k]==1){
            vArgs.push_back(k);

            //computa a profundidade do nó k
            depth = tBin->depth(k);
            close = tBin->findClose(k);
            if(k+1 <= close-1 ){
                excessMax = tBin->maxExcess(k+1, close-1);
            }
            else{
                excessMax = depth;
            }

            d.push_back( (rand()% excessMax+depth) - depth);
            i++;
        }
    }
}

void fwdSearch_binary_rmMTree(vector<int> vArgs, vector<int> d){
    int j;
    for(int i=0;i<iterations;i++){
        j = tBin->fwdSearch(vArgs[i],d[i]);
    }
}

void fwdSearch_kary_rmMTree(vector<int> vArgs, vector<int> d){
    int j;
    for(int i=0;i<iterations;i++){
        j = tKary->fwdSearch(vArgs[i],d[i]);
    }
}

int main(){
    vector<int> vArgs, d;
    int_vector<1> v;
    parentheses_to_bits("../dataset/dna.par", v);
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
    ProfilerStart("profile_fwdSearch.prof");
    fwdSearch_binary_rmMTree(vArgs,d);

    cout<< "--------------------Iniciando o profile de fwdSearch para a estrutura k-ária."<<endl;
   
    fwdSearch_kary_rmMTree(vArgs,d);
	ProfilerStop();
    return 0;
}
