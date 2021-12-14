#include <iostream>
#include "../rmm-tree-classic/rmMTreeClassic.h"
#include "generates_arguments.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>

using namespace std;
using namespace sdsl;

RMMTree *t;
int eM=0;
int order;

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

void buildTree_k(){
    cout << "Building Tree\n";	
	t = new RMMTree(v,sizeBlock,w);	
	t->buildingTree();
	
}

/*Não leve em consideração os resultados desse teste, o mesmo deve ser usado apenas para computar o excesso máximo do vetor, haja vista
*que precisamos do resultado em outras funções, e não temos ele pronto, nem mesmo a função para isso*/
void computaExcessMax_E(){
	eM= t->tree[0].excessMax;
}

void findClose_k(){
	cout << "Find Close\n";	
		for(int i=0; i < args_par_open.size();i++)
			t->findClose(args_par_open[i]);
}

void findOpen_k(){
	cout << "Find Open\n";	
		for(int i=0; i < args_par_close.size();i++)
			t->findOpen(args_par_close[i]);
}

void enclose_k(){
	cout << "Enclose\n";	
		for(int i=0; i < args_rand_I.size();i++)
			t->enclose(args_rand_I[i]);
}

void isLeaf_k(){
	cout << "Is Leaf\n";	
		for(int i=0; i < args_isLeaf.size();i++)
			t->isLeaf(args_isLeaf[i]);
}

void isAncestor_k(){
	cout << "Is Ancestor\n";	

		for(int i=0; i < args_ancestor.size();i+=2)
			t->isAncestor(args_ancestor[i],args_ancestor[i+1]);
}

void depth_k(){
	cout << "Depth\n";	

		for(int i=0; i < args_excluding0.size();i++)
			t->depth(args_excluding0[i]);
}

void parent_k(){
	cout << "Parent\n";	

		for(int i=0; i < args_excluding0.size();i++){
			t->parent(args_excluding0[i]);
		}
}

void nextSibling_k(){
	cout << "Next Sibling\n";	
		for(int i=0; i < args_excluding0.size();i++)
			t->nextSibling(args_excluding0[i]);
}

void prevSibling_k(){
	cout << "Prev Sibling\n";	
		for(int i=0; i < args_excluding0.size();i++)
			t->prevSibling(args_excluding0[i]);
}

void lastChild_k(){
	cout << "Last Child\n";	
		for(int i=0; i < args_rand_I.size();i++)
			t->lastChild(args_rand_I[i]);
}

void firstChild_k(){
	cout << "First Child\n";	
		for(int i=0; i < args_rand_II.size();i++)
			t->firstChild(args_rand_II[i]);
}

void subTreeSize_k(){
	cout << "SubTree size\n";	
		for(int i=0; i < args_par_open.size();i++)
			t->subtreeSize(args_par_open[i]);
	
}

void levelAncestor_k(){
	cout << "Level Ancestor\n";	
	int d;
		for(int i=0; i < args_ancestor.size();i+=2){
			d= rand()%eM;
			t->levelAncestor(args_ancestor[i],d);
		}
}

void levelNext_k(){
	cout << "Level Next\n";	
		for(int i=0; i < args_excluding0.size();i++){
			t->levelNext(args_excluding0[i]);
		}
}

void levelPrev_k(){
	cout << "Level prev\n";	
	for(int i=0; i < args_excluding0.size();i++)
			t->levelPrev(args_excluding0[i]);
} 


void levelLeftMost_k(){
	cout << "Level Left Most\n";
		for(int i=0; i < eM;i++)
			t->levelLeftMost(rand()%eM);
}

void levelRightMost_k(){
	cout << "Level right most\n";
	for(int i=0; i < eM;i++)
			t->levelRightMost(rand()%eM);
}

void rightMostLeaf_k(){
	cout << "Right Most Leaf\n";	

		for(int i=0; i < args_rand_I.size();i++)
			t->rightMostLeaf(args_rand_I[i]);
}

void postRank_k(){
	cout << "Post Rank\n";		
	for(int i=0; i < args_par_open.size();i++)
			t->postRank(args_par_open[i]);
}

void postSelect_k(){
	cout << "Post Select\n";	
	for(int i=0; i < args_select.size();i++)
			t->postSelect(args_select[i]);
}

int main(int argc, char **argv){
	if(argc<3){
		cout << "Número de argumentos inválidos" << endl;
		exit(EXIT_FAILURE);
	}

	iterations = atoi(argv[2]);
	order = atoi(argv[3]);	
	Initialize(argv[1]);

	buildTree_k();
	computaExcessMax_E();
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

}
