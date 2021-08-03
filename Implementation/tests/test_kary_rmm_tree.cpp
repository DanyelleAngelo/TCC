#include <iostream>
#include <benchmark/benchmark.h>
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "read_bp/read_bp.h"


using namespace sdsl;
using namespace std;

RMMTree *t;
   
class Kary_RMMTree_FixtureBM: public benchmark::Fixture{
	public:
		int sizeBlock =16;
		int w=8;
		int order = 1024;
		int_vector<1> v;
		
	Kary_RMMTree_FixtureBM(){
		parentheses_to_bits("dna.par",v);
		t = new RMMTree(v,sizeBlock,w,order);
		t->buildingTree();
		srand(t->size);
	}
};

static void ArgumentsFindClose(benchmark::internal::Benchmark *b){
	int k,i=0;
	while(i<1000){
		k = rand()%(t->size);
		if(t->bv[k]==1){
			b->Args({k});
			i++;
		}
	}
}

static void ArgumentsFindOpen(benchmark::internal::Benchmark *b){
	int k,i=0;
	while(i<1000){
		k = rand()%(t->size);
		if(t->bv[k]==0){
			b->Args({k});
			i++;
		}
	}
}

BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findClose_k)(benchmark::State& st){
	for(auto _ :st){
		t->findClose(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findClose_k)->Apply(ArgumentsFindClose);


BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findOpen_k)(benchmark::State& st){
	for(auto _ :st){
		t->findOpen(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findOpen_k)->Apply(ArgumentsFindOpen);

int main(int argc, char **argv){
	benchmark::Initialize(&argc,argv); 
  	benchmark::RunSpecifiedBenchmarks();
}
