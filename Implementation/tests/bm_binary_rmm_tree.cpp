#include <iostream>
#include <benchmark/benchmark.h>
#include "../rmm-tree-classic/rmMTreeClassic.h"
#include "generates_arguments.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>

using namespace std;
using namespace sdsl;

RMMTree *t;
int eM=0;

extern int_vector<1> v;
extern int iterations;
extern vector<long long int> args_par_close;
extern vector<long long int> args_par_open;
extern vector<long long int> args_isLeaf;
extern vector<long long int> args_ancestor;
extern vector<long long int> args_rand_I;
extern vector<long long int> args_rand_II;
extern vector<long long int> args_excluding0;
extern vector<long long int> args_select;

static void BM_BuildTree_bin(benchmark::State& st){
	for(auto _ :st){
        t = new RMMTree(v,sizeBlock,w);	
		t->buildingTree();
	}
	eM = t->tree[0].excessMax;
}
BENCHMARK(BM_BuildTree_bin);


static void BM_FindClose_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->findClose(args_par_open[i]);
	}
}
BENCHMARK(BM_FindClose_bin);

static void BM_FindOpen_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_close.size();i++)
			t->findOpen(args_par_close[i]);
	}
}
BENCHMARK(BM_FindOpen_bin);

static void BM_Enclose_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_I.size();i++)
			t->enclose(args_rand_I[i]);
	}
}
BENCHMARK(BM_Enclose_bin);

static void BM_IsLeaf_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_isLeaf.size();i++)
			t->isLeaf(args_isLeaf[i]);
	}
}
BENCHMARK(BM_IsLeaf_bin);

static void BM_IsAncestor_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_ancestor.size();i+=2)
			t->isAncestor(args_ancestor[i],args_ancestor[i+1]);
	}
}
BENCHMARK(BM_IsAncestor_bin);

static void BM_Depth_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_excluding0.size();i++)
			t->depth(args_excluding0[i]);
	}
}
BENCHMARK(BM_Depth_bin);

static void BM_Parent_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_excluding0.size();i++){
			t->parent(args_excluding0[i]);
		}
	}
}
BENCHMARK(BM_Parent_bin);

static void BM_NextSibling_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_excluding0.size();i++)
			t->nextSibling(args_excluding0[i]);
	}
}
BENCHMARK(BM_NextSibling_bin);

static void BM_PrevSibling_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_excluding0.size();i++)
			t->prevSibling(args_excluding0[i]);
	}
}
BENCHMARK(BM_PrevSibling_bin);

static void BM_LastChild_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_I.size();i++)
			t->lastChild(args_rand_I[i]);
	}
}
BENCHMARK(BM_LastChild_bin);

static void BM_FirstChild_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_II.size();i++)
			t->firstChild(args_rand_II[i]);
	}
}
BENCHMARK(BM_FirstChild_bin);

static void BM_SubTreeSize_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->subtreeSize(args_par_open[i]);
	}
}
BENCHMARK(BM_SubTreeSize_bin);

static void BM_LevelAncestor_bin(benchmark::State& st){
	int d;
	for(auto _ :st){
		//lembrando que args_ancestor, tem o dobro de iterações que setamos
		for(int i=0; i < args_ancestor.size();i+=2){
			d= rand()%eM;
			t->levelAncestor(args_ancestor[i],d);
		}
	}
}
BENCHMARK(BM_LevelAncestor_bin);

static void BM_LevelNext_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_excluding0.size();i++){
			t->levelNext(args_excluding0[i]);
		}
	}
}
BENCHMARK(BM_LevelNext_bin);

static void BM_LevelPrev_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_excluding0.size();i++)
			t->levelPrev(args_excluding0[i]);
	}
} 
BENCHMARK(BM_LevelPrev_bin);


static void BM_LevelLeftMost_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < eM;i++)
			t->levelLeftMost(rand()%eM);
	}
}
BENCHMARK(BM_LevelLeftMost_bin);

static void BM_LevelRightMost_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < eM;i++)
			t->levelRightMost(rand()%eM);
	}
}
BENCHMARK(BM_LevelRightMost_bin);

static void BM_RightMostLeaf_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_rand_I.size();i++)
			t->rightMostLeaf(args_rand_I[i]);
	}
}
BENCHMARK(BM_RightMostLeaf_bin); 

static void BM_PostRank_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_par_open.size();i++)
			t->postRank(args_par_open[i]);
	}
}
BENCHMARK(BM_PostRank_bin);

static void BM_PostSelect_bin(benchmark::State& st){
	for(auto _ :st){
		for(int i=0; i < args_select.size();i++)
			t->postSelect(args_select[i]);
	}
}
BENCHMARK(BM_PostSelect_bin);

int main(int argc, char **argv){
	if(argc<5){
		cout << "Número de argumentos inválidos" << endl;
		exit(EXIT_FAILURE);
	}

	iterations = atoi(argv[2]);
	Initialize(argv[1]);

    benchmark::Initialize(&argc,argv);
    benchmark::RunSpecifiedBenchmarks();
	return 0;
}
