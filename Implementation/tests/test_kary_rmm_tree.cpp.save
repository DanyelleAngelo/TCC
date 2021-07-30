#include <iostream>
#include <benchmark/benchmark.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "read_bp/read_bp.h"


using namespace sdsl;
using namespace std;


class Kary_RMMTree_FixtureBM: public benchmark::Fixture{
	public:
		RMMTree *t;
		int sizeBlock =4;
		int w=4;
		int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0};
        int order=4;
		int sizeInput;
		vector<int> args_fOpen;
		vector<int> args_fClose;
		Kary_RMMTree_FixtureBM(){}
		void SetUp(const ::benchmark::State& state){
			//parentheses_to_bits("wiki.par",v);
            t = new RMMTree(v,sizeBlock,w,order);	
			t->buildingTree();
			sizeInput=10;
			//PauseTiming();
			srand(t->size);
			ArgumentsFindClose();
			ArgumentsFindOpen();
			//ResumeTiming();
		}
		void TearDown(const ::benchmark::State& state) {
			delete t;
  		}
		~Kary_RMMTree_FixtureBM(){}
		void ArgumentsFindClose(){
			int k,i=0;
			while(i<sizeInput){
				k = rand()%(t->size);
				if(v[k]==1){
					args_fClose.push_back(i);
					i++;
				}
			}
		}

		void ArgumentsFindOpen(){
			int k,i=0;
			while(i<sizeInput){
				k = rand()%(t->size);
				if(v[k]==0){
					args_fOpen.push_back(i);
					i++;
				}
			}
		}
};


BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findClose_k)(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < sizeInput;i++)
			t->findClose(args_fClose[i]);
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findClose_k);


BENCHMARK_DEFINE_F(Kary_RMMTree_FixtureBM, findOpen_k)(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < sizeInput;i++)
			t->findOpen(args_fOpen[i]);
	}
}
BENCHMARK_REGISTER_F(Kary_RMMTree_FixtureBM,findOpen_k);

BENCHMARK_MAIN();