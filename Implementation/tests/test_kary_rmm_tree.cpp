#include <iostream>
#include <benchmark/benchmark.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "read_bp/read_bp.h"

using namespace sdsl;
using namespace std;

RMMTree *t;
int size;

class Kary_RMMTree_FixtureBM: public benchmark::Fixture{
	public:
		int sizeBlock =4;
		int w=2;
		int_vector<1> v;
		int order=4;
	Kary_RMMTree_FixtureBM(){
		parentheses_to_bits("wiki.par",v);
		t = new RMMTree(v,sizeBlock,w,order);
		size =(int) v.size();		
		t->buildingTree();
		srand(size);
	}
};

static void ArgumentsFindClose(benchmark::internal::Benchmark *v){
	int k,i=0;
	while(i<5){
		k = rand()%size;
		if(t->bv[k]==1){
			v->Args({k});
			i++;
		}
	}
}

static void ArgumentsFindOpen(benchmark::internal::Benchmark *v){
	int k,i=0;
	while(i<5){
		k = rand()%size;
		if(t->bv[k]==0){
			v->Args({k});
			i++;
		}
	}
}

BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findClose_kary)(benchmark::State& st){
	for(auto _ :st){
		t->findClose(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findClose_kary)->Apply(ArgumentsFindClose);


BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findOpen_kary)(benchmark::State& st){
	for(auto _ :st){
		t->findOpen(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findOpen_kary)->Apply(ArgumentsFindOpen);

int main(int argc, char **argv){
	benchmark::Initialize(&argc,argv); 
  	benchmark::RunSpecifiedBenchmarks();
}
