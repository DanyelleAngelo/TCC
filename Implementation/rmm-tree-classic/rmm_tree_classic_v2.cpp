#include <iostream>
#include <algorithm>
#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <math.h>
#include <bitset>


using namespace std;
using namespace sdsl;

const int w=2; //o tamanho do bloco deve ser múltiplo de w

typedef struct Node{
	int excess;
	int excessMax;
	int excessMin;
	int numberExcessMin;
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
		vector<Node> tableC;

	RMMTree(int_vector<1> &bv, int sizeBlock){
		this->bv = bv;
		this->b_rank = rank_support_v<1>(&bv);
		this->sizeBlock = sizeBlock;
		this->numberLeaves = ceil((double)(bv.size()/sizeBlock));
		this->numberNodes = (2*this->numberLeaves) -1;//árvore binária completa
		this->height = ceil(log2(this->numberLeaves));
		this->tree.resize(this->numberNodes);
	}

	int bitsread(int s,int e){
		int pos=0;
		int value=0;
		for(int i=e;i>=s;i--){
			if(bv[i]==1){
				value+=1*pow(2,pos);
			}
			pos++;
		}
		return value;
	}

	int fn_excess(int s,int e){
		// excess(e) - excess(s)
		// excess(i) = 2*rank1(i) -i
		if(s==1)return (2*b_rank(e))-e;
		s-=1;
		return ((2*b_rank(e))-e) - ((2*b_rank(s))-s);
	}
	int leafInTree(int k){
		int t = pow(2,ceil(log2(numberLeaves)));

		if(k <= (2*numberLeaves) - t)return t-2+k;
		else return t-2-numberLeaves+k;
	}

	/*Procura o excesso no bloco*/
	int fwdBlock(int i,int d,int *dr){
		int subBlock = ceil((double)(i+1)/w);
		int t = ceil((double)(i+1)/sizeBlock)*(sizeBlock/w);
		int x,p;
		*dr=0;

		//a ideia é varrer os w bits (a partir de i+1) do subbloco de bv.
		for(int j=i+1;j<=subBlock*w;j++){
			*dr+= (bv[j]==1) ? 1 : -1;
			if(*dr== d)return j;
		}
		//se não encontrarmos d no pequeno bloco, precisamos varrer o restante do grande bloco a qual i pertence
		
		for(p=subBlock+1;p<=t;p++){
			x = bitsread((p-1)*w,(p*w)-1);
			if(*dr + tableC[x].excessMin <= d)break;//d está no grande bloco de i
			*dr += tableC[x].excess;
		}
		if(p > t)return (t*w)+1;//d não está nesse bloco, retorna o índice do primeiro elemento do próximo bloco
		
		//d está no grande bloco restante (depois de i+1+w) de i
		for(int j=((p-1)*w)+1;j<p*w;j++){
			*dr += (bv[j]==1) ? 1 : -1;
			if(*dr== d)return j;
		}
		return 0;
	}

	int fwdSearch(int i,int d){
		int dr,j,v;
		//verifica primeiro se d está no bloco a qual pertence "i"
		j = fwdBlock(i,d,&dr);
		if(dr == d) return j;

		//caso não esteja, inicia a subida na árvore
		v = leafInTree(i);
		
		//verifica se é uma folha esquerda e se o excesso relativo está no intervalo do elemento à esquerda 
		while(v>=1 && dr + tree[v+1].excessMin > d){
			if(v%2!=0)dr -= tree[v+1].excess;
			v = ceil((double)v/2);
		}
		//TODO verifica se achou nos super-intervalos

	
		//a ideia é descer na árvore até que cheguemos nas folhas, quando chegarmos lá, escaneamos o seu bloco
		while(v<numberLeaves){
			if(dr+tree[(2*v)+1].excessMin<=d)v=(2*v)+1;//excesso está no filho a esquerda
			else{//excesso está no filho a direita
				dr += tree[(2*v)+1].excess;
				v = (2*v)+2;
			}
		}
		int k =((v-1)*sizeBlock) +1; //indice onde a folha começa
		j = fwdBlock(k, d-dr, &dr);
		return j;
	}

	void buildingTree(){
		/*pré-computa tabela C, para acelerar a cosntruçã da RMM-tree*/
		buildingTableC();

		/*Inicia a construção das folhas, verificando se estas estão distribuídas em 2 níveis.*/
		int lastLevelLeaves = (2*numberLeaves) -  pow(2,height);
		int prevLevelLeaves = numberLeaves - lastLevelLeaves;
		int kth;

		if(1 <= lastLevelLeaves)kth = pow(2,height)-1; 
		else kth= pow(2,height) - lastLevelLeaves-1;
		buildingLeaves(kth,numberNodes,1);
		if(prevLevelLeaves > 0){
			buildingLeaves(numberNodes-numberLeaves,kth,prevLevelLeaves-1);
		}

		/*Inicia a construção dos nós internos e raíz*/
		buildingInternalNodesRoot();
	}

	void buildingTableC(){
		Node node;
		node.excess = 0;
		node.excessMax = 0 - w;
		node.excessMin = w;
		node.numberExcessMin = 0;
		
		for(int i=0; i < (1<<w); i++){
			tableC.push_back(node);

			for(int j=0; j<w;j++){
				tableC[i].excess += i & (1 << (w-j-1)) ? 1 : -1;
				
				if(tableC[i].excess > tableC[i].excessMax){
					tableC[i].excessMax = tableC[i].excess;
				}
				if(tableC[i].excess < tableC[i].excessMin){
					tableC[i].excessMin = tableC[i].excess;
					tableC[i].numberExcessMin = 1;
				}else if(tableC[i].excess == tableC[i].excessMin){
					tableC[i].numberExcessMin+=1;
				}
			}
		}
	}

	void buildingLeaves(int s, int e, int k){
		int x;

		for(int i=s;i<e;i++,k++){
			tree[i].excess = 0;
			tree[i].excessMax = 0 - w;
			tree[i].excessMin =w;
			tree[i].numberExcessMin =0;
			for(int p = ((k-1)*sizeBlock/w)+1;p<=k*sizeBlock/w;p++){
			 	x = bitsread((p-1)*w,((p-1)*w)+w-1);
				
				if(tree[i].excess + tableC[x].excessMax > tree[i].excessMax){
					tree[i].excessMax = tree[i].excess + tableC[x].excessMax;
				}
				if(tree[i].excess + tableC[x].excessMin < tree[i].excessMin){
					tree[i].excessMin = tree[i].excess + tableC[x].excessMin;
					tree[i].numberExcessMin = tableC[x].numberExcessMin;
				}else if(tree[i].excess + tableC[x].excessMin == tree[i].excessMin){
					tree[i].numberExcessMin = tree[i].numberExcessMin + tableC[x].numberExcessMin;
				}
				tree[i].excess += tableC[x].excess;
			} 
		}

	}

	void buildingInternalNodesRoot(){
		for(int i=numberNodes - numberLeaves-1;i>=0;i--){
			int vL = (2*i)+1;
			int vR = (2*i)+2;

			tree[i].excess = tree[vL].excess + tree[vR].excess;

			int excessTemp = tree[vL].excess + tree[vR].excessMax;
			tree[i].excessMax = (tree[vL].excessMax >= excessTemp) ? tree[vL].excessMax : excessTemp;

			excessTemp = tree[vL].excess + tree[vR].excessMin;
			if(tree[vL].excessMin < excessTemp){
				tree[i].excessMin = tree[vL].excessMin;
				tree[i].numberExcessMin = tree[vL].numberExcessMin;
			}else if(tree[vL].excessMin > excessTemp){
				tree[i].excessMin = excessTemp;
				tree[i].numberExcessMin = tree[vR].numberExcessMin;
			}else{
				tree[i].excessMin = tree[vL].excessMin;
				tree[i].numberExcessMin = tree[vL].numberExcessMin + tree[vR].numberExcessMin; 
			}
		}
	}

	void printNode(vector<Node> v, int i){
		cout << "v[" << i<< "].e = " << v[i].excess << "; "
			 << "v[" << i<< "].m = " << v[i].excessMin << "; "
			 << "v[" << i<< "].M = " << v[i].excessMax << "; "
			 << "v[" << i << "].n = " << v[i].numberExcessMin << ".\n"
			 << endl;
	}

	void printTree(){
		int i;
		cout << " ----- Root ----- \n";
		printNode(tree, 0);
		cout << " ----- Internal nodes ----- \n";
		for(i=1;i<numberNodes-numberLeaves;i++)printNode(tree, i);
		
		cout << " ----- Folhas -----" << endl;
		for(;i<numberNodes;i++)printNode(tree,i);
	}

	void printTableC(){
		cout << "---- Tabela C, para acelerar a construção da RMM-tree ----" << "\n\n";
		for(int i=0;i<tableC.size();i++)printNode(tableC,i);
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
	//v[4] =0 //colocar para teste simples.
	int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
	RMMTree t(v,4);
	t.buildingTree();
	int r = t.fwdSearch(2,-1);
	cout << "resposta=" << r <<"\n";
	//t.printTree();
	//t.printTableC();
	return 0;
}
