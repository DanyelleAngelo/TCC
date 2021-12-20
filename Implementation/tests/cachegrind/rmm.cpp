#include <iostream>
#include "../../rmm-tree-classic/rmMTreeClassic.h"
#include "../../rmm-tree-optimized/rmMTreeOptimized.h"
#include "../generates_arguments.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>

using namespace std;
using namespace sdsl;

RMMTree_Bin *tBin;
RMMTree_Kary *tKary;
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

/*Não leve em consideração os resultados desse teste, o mesmo deve ser usado apenas para computar o excesso máximo do vetor, haja vista
*que precisamos do resultado em outras funções, e não temos ele pronto, nem mesmo a função para isso*/
void computaExcessMax_E(){
	eM= tBin->tree[0].excessMax;
}

void findClose_b(){
	cout << "Find Close\n";	
		for(int i=0; i < args_par_open.size();i++)
			tBin->findClose(args_par_open[i]);
}

void findOpen_b(){
	cout << "Find Open\n";	
		for(int i=0; i < args_par_close.size();i++)
			tBin->findOpen(args_par_close[i]);
}

void enclose_b(){
	cout << "Enclose\n";	
		for(int i=0; i < args_rand_I.size();i++)
			tBin->enclose(args_rand_I[i]);
}

void isLeaf_b(){
	cout << "Is Leaf\n";	
		for(int i=0; i < args_isLeaf.size();i++)
			tBin->isLeaf(args_isLeaf[i]);
}

void isAncestor_b(){
	cout << "Is Ancestor\n";	

		for(int i=0; i < args_ancestor.size();i+=2)
			tBin->isAncestor(args_ancestor[i],args_ancestor[i+1]);
}

void depth_b(){
	cout << "Depth\n";	

		for(int i=0; i < args_excluding0.size();i++)
			tBin->depth(args_excluding0[i]);
}

void parent_b(){
	cout << "Parent\n";	

		for(int i=0; i < args_excluding0.size();i++){
			tBin->parent(args_excluding0[i]);
		}
}

void nextSibling_b(){
	cout << "Next Sibling\n";	
		for(int i=0; i < args_excluding0.size();i++)
			tBin->nextSibling(args_excluding0[i]);
}

void prevSibling_b(){
	cout << "Prev Sibling\n";	
		for(int i=0; i < args_excluding0.size();i++)
			tBin->prevSibling(args_excluding0[i]);
}

void lastChild_b(){
	cout << "Last Child\n";	
		for(int i=0; i < args_rand_I.size();i++)
			tBin->lastChild(args_rand_I[i]);
}

void firstChild_b(){
	cout << "First Child\n";	
		for(int i=0; i < args_rand_II.size();i++)
			tBin->firstChild(args_rand_II[i]);
}

void subTreeSize_b(){
	cout << "SubTree size\n";	
		for(int i=0; i < args_par_open.size();i++)
			tBin->subtreeSize(args_par_open[i]);
	
}

void levelAncestor_b(){
	cout << "Level Ancestor\n";	
	int d;
		for(int i=0; i < args_ancestor.size();i+=2){
			d= rand()%eM;
			tBin->levelAncestor(args_ancestor[i],d);
		}
}

void levelNext_b(){
	cout << "Level Next\n";	
		for(int i=0; i < args_excluding0.size();i++){
			tBin->levelNext(args_excluding0[i]);
		}
}

void levelPrev_b(){
	cout << "Level prev\n";	
	for(int i=0; i < args_excluding0.size();i++)
			tBin->levelPrev(args_excluding0[i]);
} 


void levelLeftMost_b(){
	cout << "Level Left Most\n";
		for(int i=0; i < eM;i++)
			tBin->levelLeftMost(rand()%eM);
}

void levelRightMost_b(){
	cout << "Level right most\n";
	for(int i=0; i < eM;i++)
			tBin->levelRightMost(rand()%eM);
}

void rightMostLeaf_b(){
	cout << "Right Most Leaf\n";	

		for(int i=0; i < args_rand_I.size();i++)
			tBin->rightMostLeaf(args_rand_I[i]);
}

void postRank_b(){
	cout << "Post Rank\n";		
	for(int i=0; i < args_par_open.size();i++)
			tBin->postRank(args_par_open[i]);
}

void postSelect_b(){
	cout << "Post Select\n";	
	for(int i=0; i < args_select.size();i++)
			tBin->postSelect(args_select[i]);
}

void findClose_k(){
	cout << "Find Close\n";	
		for(int i=0; i < args_par_open.size();i++)
			tKary->findClose(args_par_open[i]);
}

void findOpen_k(){
	cout << "Find Open\n";	
		for(int i=0; i < args_par_close.size();i++)
			tKary->findOpen(args_par_close[i]);
}

void enclose_k(){
	cout << "Enclose\n";	
		for(int i=0; i < args_rand_I.size();i++)
			tKary->enclose(args_rand_I[i]);
}

void isLeaf_k(){
	cout << "Is Leaf\n";	
		for(int i=0; i < args_isLeaf.size();i++)
			tKary->isLeaf(args_isLeaf[i]);
}

void isAncestor_k(){
	cout << "Is Ancestor\n";	

		for(int i=0; i < args_ancestor.size();i+=2)
			tKary->isAncestor(args_ancestor[i],args_ancestor[i+1]);
}

void depth_k(){
	cout << "Depth\n";	

		for(int i=0; i < args_excluding0.size();i++)
			tKary->depth(args_excluding0[i]);
}

void parent_k(){
	cout << "Parent\n";	

		for(int i=0; i < args_excluding0.size();i++){
			tKary->parent(args_excluding0[i]);
		}
}

void nextSibling_k(){
	cout << "Next Sibling\n";	
		for(int i=0; i < args_excluding0.size();i++)
			tKary->nextSibling(args_excluding0[i]);
}

void prevSibling_k(){
	cout << "Prev Sibling\n";	
		for(int i=0; i < args_excluding0.size();i++)
			tKary->prevSibling(args_excluding0[i]);
}

void lastChild_k(){
	cout << "Last Child\n";	
		for(int i=0; i < args_rand_I.size();i++)
			tKary->lastChild(args_rand_I[i]);
}

void firstChild_k(){
	cout << "First Child\n";	
		for(int i=0; i < args_rand_II.size();i++)
			tKary->firstChild(args_rand_II[i]);
}

void subTreeSize_k(){
	cout << "SubTree size\n";	
		for(int i=0; i < args_par_open.size();i++)
			tKary->subtreeSize(args_par_open[i]);
	
}

void levelAncestor_k(){
	cout << "Level Ancestor\n";	
	int d;
		for(int i=0; i < args_ancestor.size();i+=2){
			d= rand()%eM;
			tKary->levelAncestor(args_ancestor[i],d);
		}
}

void levelNext_k(){
	cout << "Level Next\n";	
		for(int i=0; i < args_excluding0.size();i++){
			tKary->levelNext(args_excluding0[i]);
		}
}

void levelPrev_k(){
	cout << "Level prev\n";	
	for(int i=0; i < args_excluding0.size();i++)
			tKary->levelPrev(args_excluding0[i]);
} 


void levelLeftMost_k(){
	cout << "Level Left Most\n";
		for(int i=0; i < eM;i++)
			tKary->levelLeftMost(rand()%eM);
}

void levelRightMost_k(){
	cout << "Level right most\n";
	for(int i=0; i < eM;i++)
			tKary->levelRightMost(rand()%eM);
}

void rightMostLeaf_k(){
	cout << "Right Most Leaf\n";	

		for(int i=0; i < args_rand_I.size();i++)
			tKary->rightMostLeaf(args_rand_I[i]);
}

void postRank_k(){
	cout << "Post Rank\n";		
	for(int i=0; i < args_par_open.size();i++)
			tKary->postRank(args_par_open[i]);
}

void postSelect_k(){
	cout << "Post Select\n";	
	for(int i=0; i < args_select.size();i++)
			tKary->postSelect(args_select[i]);
}

void bin(){
	cout << "\n-------------binary rmM-tree\n";
	tBin = new RMMTree_Bin(v,sizeBlock,w);	
	tBin->buildingTree();
	computaExcessMax_E();findClose_b();
	findOpen_b();
	enclose_b();
	isLeaf_b();
	isAncestor_b();
	depth_b();
	parent_b();
	nextSibling_b();
	prevSibling_b();
	lastChild_b();
	firstChild_b();
	subTreeSize_b();
	levelAncestor_b();
	levelNext_b();
	levelPrev_b();
	levelLeftMost_b();
	levelRightMost_b();
	rightMostLeaf_b();
	postRank_b();
	postSelect_b();
	delete tBin;
}

void kary(int order){	
	cout << "\n------------- " << order << "-ary rmM-tree\n";
	tKary = new RMMTree_Kary(v,sizeBlock,w,order);	
	tKary->buildingTree();
	findClose_k();
	findOpen_k();
	enclose_k();
	isLeaf_k();
	isAncestor_k();
	depth_k();
	parent_k();
	nextSibling_k();
	prevSibling_k();
	lastChild_k();
	firstChild_k();
	subTreeSize_k();
	levelAncestor_k();
	levelNext_k();
	levelPrev_k();
	levelLeftMost_k();
	levelRightMost_k();
	rightMostLeaf_k();
	postRank_k();
	postSelect_k();
	delete tKary;
}
int main(int argc, char **argv){
	if(argc<3){
		cout << "Número de argumentos inválidos" << endl;
		exit(EXIT_FAILURE);
	}
	iterations = atoi(argv[2]);
	Initialize(argv[1]);
	bin();
	kary(4);
	kary(8);
	kary(16);

}
