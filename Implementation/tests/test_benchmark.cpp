#include <iostream>
#include <benchmark/benchmark.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-optimized/rmMTreeOptimized.h"
#include "generates_arguments.h"

using namespace sdsl;
using namespace std;

RMMTree *t;
int eM=0;
int order;

static void BM_BuildTree_k(benchmark::State& st){
	for(auto _ :st){
        t = new RMMTree(v,sizeBlock,w,order);	
		t->buildingTree();
	}
}
BENCHMARK(BM_BuildTree_k);

static void BM_DISABLED_E_MAX_k(benchmark::State& st){
	/*
	*	não está sendo usado para medir o desempenho da estrutura, apenas pra sabermos, qual é o
	*	excesso máximp, haja vista que essa estrutura não tem a função maxExcess
	*/
	vector<int> exc;
	int e=0;
	
	for(int i=0; i<t->tree[0].nKeys;i++){
		exc.push_back(e+t->tree[0].keys[i].excessMax);
		e+=t->tree[0].keys[i].excess;
	}
	eM= *max_element(exc.begin(), exc.end());
}
BENCHMARK(BM_DISABLED_E_MAX_k);



static void BM_FindClose_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->findClose(args_par_open[i]);
	}
}
BENCHMARK(BM_FindClose_k);

static void BM_FindOpen_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_close.size();i++)
			t->findOpen(args_par_close[i]);
	}
}
BENCHMARK(BM_FindOpen_k);

static void BM_Enclose_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_I.size();i++)
			t->enclose(args_rand_I[i]);
	}
}
BENCHMARK(BM_Enclose_k);

static void BM_IsLeaf_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_isLeaf.size();i++)
			t->isLeaf(args_isLeaf[i]);
	}
}
BENCHMARK(BM_IsLeaf_k);

static void BM_IsAncestor_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_ancestor.size();i+=2)
			t->isAncestor(args_ancestor[i],args_ancestor[i+1]);
	}
}
BENCHMARK(BM_IsAncestor_k);

static void BM_Depth_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_I.size();i++)
			t->depth(args_rand_I[i]);
	}
}
BENCHMARK(BM_Depth_k);

static void BM_Parent_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->parent(args_par_open[i]);
	}
}
BENCHMARK(BM_Parent_k);

static void BM_NextSibling_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->nextSibling(args_par_open[i]);
	}
}
BENCHMARK(BM_NextSibling_k);

static void BM_PrevSibling_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_close.size();i++)
			t->prevSibling(args_par_close[i]);
	}
}
BENCHMARK(BM_PrevSibling_k);

static void BM_LastChild_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_I.size();i++)
			t->lastChild(args_rand_I[i]);
	}
}
BENCHMARK(BM_LastChild_k);

static void BM_FirstChild_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_II.size();i++)
			t->firstChild(args_rand_II[i]);
	}
}
BENCHMARK(BM_FirstChild_k);

static void BM_SubTreeSize_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->subtreeSize(args_par_open[i]);
	}
}
BENCHMARK(BM_SubTreeSize_k);

static void BM_LevelAncestor_k(benchmark::State& st){
	for(auto _ :st){
		//lembrando que args_ancestor, tem o dobro de iterações que setamos
		for(int i=0; i < args_ancestor.size();i+=2)
			t->levelAncestor(args_ancestor[i],rand()%eM);
	}
}
BENCHMARK(BM_LevelAncestor_k);

static void BM_LevelNext_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_II.size();i++)
			t->levelNext(args_rand_II[i]);
	}
}
BENCHMARK(BM_LevelNext_k);

static void BM_LevelPrev_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_II.size();i++)
			t->levelPrev(args_rand_II[i]);
	}
}
BENCHMARK(BM_LevelPrev_k);

static void BM_LevelLeftMost_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < eM;i++)
			t->levelLeftMost(rand()%eM);
	}
}
BENCHMARK(BM_LevelLeftMost_k);

static void BM_LevelRightMost_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < eM;i++)
			t->levelRightMost(rand()%eM);
	}
}
BENCHMARK(BM_LevelRightMost_k);

static void BM_RightMostLeaf_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_I.size();i++)
			t->rightMostLeaf(args_rand_I[i]);
	}
}
BENCHMARK(BM_RightMostLeaf_k);

static void BM_PostRank_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->postRank(args_par_open[i]);
	}
}
BENCHMARK(BM_PostRank_k);

static void BM_PostSelect_k(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_close.size();i++)
			t->postSelect(args_par_close[i]);
	}
}
BENCHMARK(BM_PostSelect_k);

int main(int argc, char **argv){
	if(argc<6){
		cout << "Número de argumentos inválidos" << endl;
		exit(EXIT_FAILURE);
	}
	
	Initialize(argv[1]);
	iterations = atoi(argv[2]);
	order = atoi(argv[3]);	

    benchmark::Initialize(&argc,argv);
    benchmark::RunSpecifiedBenchmarks();
}
