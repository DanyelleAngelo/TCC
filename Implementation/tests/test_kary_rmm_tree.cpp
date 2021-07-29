#include <iostream>
#include <vector>
#include <benchmark/benchmark.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "read_bp/read_bp.h"

using namespace sdsl;
using namespace std;

RMMTree *t;

class Kary_RMMTree_FixtureBM: public benchmark::Fixture{
	public:
		int sizeBlock =8;
		int w=4;
		int order=32;
		int size;
		int_vector<1> v;
		vector<int> input_close; 
		vector<int> input_open;

	void SetUp(){
		//parentheses_to_bits("wiki.par",v);
		size = v.size();
		t = new RMMTree(v,sizeBlock,w,order);
		t->buildingTree();
		argumentsFindClose(10,size);
		argumentsFindOpen(10,size);
		srand(size);
		cout << "oiii\n";
	}
	void TearDown(){
		delete t;
	}

	void argumentsFindClose(int n,int lim){
		int i=0, k;
		while(i<n){
			k = rand()%lim;
			if(v[k]==1){
				input_close.push_back(k);
				i++;
			}
		}
	}

	void argumentsFindOpen(int n,int lim){
		int i=0, k;
		while(i<n){
			k = rand()%lim;
			if(v[k]==0){
				input_open.push_back(k);
				i++;
			}
		}
	}
};

BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findClose_kary)(benchmark::State& st){
	for(auto _ :st){
		for(int i=0;i<10;i++){
			t->findClose(input_close[i]);
		}
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findClose_kary);


BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findOpen_kary)(benchmark::State& st){
	for(auto _ :st){
		for(int i=0;i<10;i++){
			t->findOpen(input_open[i]);
		}
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findOpen_kary);


int main(int argc, char **argv){
	benchmark::Initialize(&argc,argv); 
  	benchmark::RunSpecifiedBenchmarks();
}
