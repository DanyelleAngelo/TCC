#include <iostream>
#include <algorithm>
#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <math.h>

using namespace std;
using namespace sdsl;

typedef struct Node{
	int excess;
	int excessMax;
	int excessMin;
	int numberExcesMin;
}Node;

class RMMTree{
	public:
		bit_vector bv;
		rank_support_v<> b_rank;
		int sizeBlock;
		int numberLeaves;
		int numberNodes;
		int height;
		vector<Node> tree;

	//constructor
	RMMTree(int_vector<1> &bv, int sizeBlock){
		this->bv = bv;
		this->b_rank = rank_support_v<1>(&bv);
		this->sizeBlock = sizeBlock;
		this->numberLeaves = ceil((double)(bv.size()/sizeBlock));
		this->numberNodes = (2*this->numberLeaves) -1;//árvore binária completa
		this->height = ceil(log2(this->numberLeaves));
		this->tree.resize(this->numberNodes);
	}

	/*
	*Calcula o excesso global de uma área: excesso global no intervalo (a,b) é excess(b)-excess(a-1) 
	*@param s limite inferior 
	*@param e limite superior
	*@return diferença de excesso no intervalo  "s" e "e"
	*/
	int fn_excess(int s,int e){
		// excess(e) - excess(s)
		// excess(i) = 2*rank1(i) -i
		if(s<1)return (2*b_rank(e))-e;
		s-=1;
		return ((2*b_rank(e))-e) - ((2*b_rank(s))-s);
	}

	/*
	*Calcula o menor excesso da área
	*@param v: vetor de excessos de cada posição i
	*@return o menor valor de v
	*/
	int fn_minExcess(vector<int> v){
		return *min_element(v.begin(), v.end());
	}

	/*
	*Calcula o maior excesso da área
	*@param v: vetor de excessos de cada posição i
	*@return maior valor de v
	*/
	int fn_maxExcess(vector<int> v){
		return *max_element(v.begin(), v.end());
	}

	/*
	*Retorna a quantidade de vezes que o excesso minimo ocorre na área
	*@param v: vetor de excessos de cada posição i
	*@param m: excesso mínimo da área
	*@return: quantidade de vezes que m aparece em vector
	*/
	int fn_nMinExcess(vector<int> v,int i){
		return count(v.begin(),v.end(),tree[i].excessMin);
	}

	void computeRegistries(int i, int s,int e){
		//computa um vetor de excessos para o intervalo
		vector<int> vecEx;
		vecEx.clear();

		for(int l=s;l<=e;l++){
			vecEx.push_back(fn_excess(s,l));
		}

		tree[i].excess = fn_excess(s,e);
		tree[i].excessMin = fn_minExcess(vecEx);
		tree[i].excessMax = fn_maxExcess(vecEx);
		tree[i].numberExcesMin = fn_nMinExcess(vecEx,i);
	}

	void buildingTree(){
		//constrói as folas
		buildingLeaves();

		//inicia a construção dos nós internos e raíz
		for(int i=numberNodes - numberLeaves-1;i>=0;i--){
			int vL = (2*i)+1;
			int vR = (2*i)+2;

			tree[i].excess = tree[vL].excess + tree[vR].excess;

			int excessTemp = tree[vL].excess + tree[vR].excessMax;
			tree[i].excessMax = (tree[vL].excessMax >= excessTemp) ? tree[vL].excessMax : excessTemp;

			excessTemp = tree[vL].excess + tree[vR].excessMin;
			tree[i].excessMin = (tree[vL].excessMin <= excessTemp) ? tree[vL].excessMin: excessTemp;

			if(tree[vL].excessMin < excessTemp){
				tree[i].numberExcesMin = tree[vL].numberExcesMin;
			}else if(tree[vL].excessMin > excessTemp){
				tree[i].numberExcesMin = tree[vR].numberExcesMin;
			}else{
				tree[i].numberExcesMin = tree[vL].numberExcesMin + tree[vR].numberExcesMin; 
			}
		}
	}

	//TODO: usar log do número de folhas para pegar diretamente a posição da folha ao invés da quantidade em cada nível
	//TODO: as folhas estão agrupadas em um nível <-> o número de folhas for uma potência de 2
	void buildingLeaves(){
		//calcula a quantidade de folhas no último nível e no anterior (se houver)
		int lastLevelLeaves = (2*numberLeaves) -  pow(2,height);
		int prevLevelLeaves = numberLeaves - lastLevelLeaves;
		int kth,i,j=1,s,e;
		

		//verifica o índice da primeira folha mais a esquerda
		if(1 <= lastLevelLeaves)kth = pow(2,height)-1; 
		else kth = pow(2,height) - lastLevelLeaves-1;

		//constroí o último nível de folhas da árvore
		for(i=kth;i<numberNodes;i++){
			//calcula o intervalo de cobertura
			s = ((j-1)*sizeBlock)+1;
			e = (j*sizeBlock);
			j++;
			computeRegistries(i,s,e);
		}
		
		//constroí o nível anterior de folhas da árvore (se houver)
		if(prevLevelLeaves > 0){
			for(i=numberNodes-numberLeaves;i<kth;i++){
			//i controla o indice da folha
				s = ((j-1)*sizeBlock)+1;
				e = (j*sizeBlock);
				j++;
				computeRegistries(i,s,e);
			}
		}
	}

	void printNode(int i){
		cout << "v[" << i+1 << "].e = " << tree[i].excess << "\n"
			 << "v[" << i+1 << "].m = " << tree[i].excessMin << "\n"
			 << "v[" << i+1 << "].M = " << tree[i].excessMax << "\n"
			 << "v[" << i+1 << "].n = " << tree[i].numberExcesMin << "\n"
			 << endl;
	}

	void printTree(){
		int i=0;
		cout << "Root:\n";
		for(;i<numberNodes-numberLeaves;i++)printNode(i);
		
		cout << " ----- Folhas -----" << endl;
		for(;i<numberNodes;i++)printNode(i);
	}

	void printInfoTree(){
		cout << "BPS: " << this->bv << '\n'
			 << "Tamanho de bloco: " << this->sizeBlock << '\n'
			 << "Quantidade de folhas: " << this->numberLeaves << '\n'
			 << "Quantidade de nós: " <<this->numberNodes << '\n'
			 << "Altura da árvore: " << this->height << '\n'
			 << endl;
	}
};

int main(){
	int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
	RMMTree t(v,4);
	t.buildingTree();
	t.printTree();
	return 0;
}
