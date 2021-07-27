
#include <iostream>
#include <benchmark/benchmark.h>
#include <memory>
#include <sdsl/int_vector.hpp>
#include "../rmm-tree-classic/rmMTreeClassic.h"

using namespace sdsl;
using namespace std;
RMMTree *t;
int size;

class Bin_RMMTree_FixtureBM: public benchmark::Fixture{
	public:
		int sizeBlock =4;
		int w=2;
		int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
	Bin_RMMTree_FixtureBM(){
		t = new RMMTree(v,sizeBlock,w);
		size = v.size();		
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

BENCHMARK_DEFINE_F(Bin_RMMTree_FixtureBM, findClose_binary)(benchmark::State& st){
	for(auto _ :st){
		t->findClose(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Bin_RMMTree_FixtureBM,findClose_binary)->Apply(ArgumentsFindClose);


BENCHMARK_DEFINE_F(Bin_RMMTree_FixtureBM, findOpen_binary)(benchmark::State& st){
	for(auto _ :st){
		t->findOpen(st.range(0));
	}
}
BENCHMARK_REGISTER_F(Bin_RMMTree_FixtureBM,findOpen_binary)->Apply(ArgumentsFindOpen);

int main(int argc, char **argv){ 
	benchmark::Initialize(&argc,argv); 
  	 benchmark::RunSpecifiedBenchmarks();
}
