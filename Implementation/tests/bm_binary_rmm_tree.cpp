#include <iostream>
#include <benchmark/benchmark.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-classic/rmMTreeClassic.h"
#include "read_bp/read_bp.h"

using namespace sdsl;
using namespace std;

int iterations;
int_vector<1> v;
int sizeBlock;
int w;
int order;
   
class Bin_RMMTree_FixtureBM: public benchmark::Fixture{
	public:
		RMMTree *t;
		vector<int> args_fOpen;
		vector<int> args_fClose;

		Bin_RMMTree_FixtureBM(){}
		~Bin_RMMTree_FixtureBM(){}

		void SetUp(const ::benchmark::State& state){
            t = new RMMTree(v,sizeBlock,w);	
			t->buildingTree();

			srand(t->size);
			ArgumentsFindClose();
			ArgumentsFindOpen();
		}

		void TearDown(const ::benchmark::State& state) {
			delete t;
  		}

		void ArgumentsFindClose(){
			int k,i=0;
			while(i<iterations){
				k = rand()%(t->size);
				if(v[k]==1){
					args_fClose.push_back(i);
					i++;
				}
			}
		}

		void ArgumentsFindOpen(){
			int k,i=0;
			while(i<iterations){
				k = rand()%(t->size);
				if(v[k]==0){
					args_fOpen.push_back(i);
					i++;
				}
			}
		}
};

BENCHMARK_F(Bin_RMMTree_FixtureBM, findClose_b)(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_fClose.size();i++)
			t->findClose(args_fClose[i]);
	}
}


BENCHMARK_F(Bin_RMMTree_FixtureBM, findOpen_b)(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_fClose.size();i++)
			t->findOpen(args_fOpen[i]);
	}
}

int main(int argc, char **argv){
	parentheses_to_bits(argv[1],v);
	iterations = atoi(argv[2]);
	sizeBlock= atoi(argv[3]);
	order = atoi(argv[4]);	
	w = sizeBlock/2;

    benchmark::Initialize(&argc,argv);
    benchmark::RunSpecifiedBenchmarks();
}
