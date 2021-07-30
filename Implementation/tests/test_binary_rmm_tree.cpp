#include <iostream>
#include <benchmark/benchmark.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-classic/rmMTreeClassic.h"
#include "read_bp/read_bp.h"

using namespace sdsl;
using namespace std;

RMMTree *t;
   
class Bin_RMMTree_FixtureBM: public benchmark::Fixture{
	public:
		int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0};
		int sizeBlock =4;
		int w=4;
		
	Bin_RMMTree_FixtureBM(){
		//parentheses_to_bits("wiki.par",v);
		t = new RMMTree(v,sizeBlock,w);	
		t->buildingTree();
		srand(t->size);
	}
};

static void ArgumentsFindClose(benchmark::internal::Benchmark *v){
	int k,i=0;
	while(i<10){
		k = rand()%(t->size);
		if(t->bv[k]==1){
			v->Args({k});
			i++;
		}
	}
}

static void ArgumentsFindOpen(benchmark::internal::Benchmark *v){
	int k,i=0;
	while(i<10){
		k = rand()%(t->size);
		if(t->bv[k]==0){
			v->Args({k});
			i++;
		}
	}
}

BENCHMARK_DEFINE_F(Bin_RMMTree_FixtureBM, findClose_b)(benchmark::State& st){
	for(auto _ :st){
		t->findClose(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Bin_RMMTree_FixtureBM,findClose_b)->Apply(ArgumentsFindClose);


BENCHMARK_DEFINE_F(Bin_RMMTree_FixtureBM, findOpen_b)(benchmark::State& st){
	for(auto _ :st){
		t->findOpen(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Bin_RMMTree_FixtureBM,findOpen_b)->Apply(ArgumentsFindOpen);

int main(int argc, char **argv){
	benchmark::Initialize(&argc,argv); 
  	benchmark::RunSpecifiedBenchmarks();
}
