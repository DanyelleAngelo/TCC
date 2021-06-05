#include <iostream>
#include "RMMTreeClassic.h"

using namespace std;

int main(){
	//v[4] =0 //colocar para teste simples.
	int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
	RMMTree t(v,4);
	t.buildingTree();
	t.printTree();
	t.printTableC();
	return 0;
}
