#include <iostream>

#include "RMMTreeClassic.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bits.hpp>

#include <sdsl/bit_vector_il.hpp>

#include <algorithm>
#include <vector>
#include <math.h>
#include <bitset>


using namespace std;
using namespace sdsl;



RMMTree::RMMTree(int_vector<1> &bv, int sizeBlock){
	this->bv = bv;
	this->b_rank = rank_support_v<1>(&bv);
	this->sizeBlock = sizeBlock;
	this->numberLeaves = ceil((double)(bv.size()/sizeBlock));
	this->numberNodes = (2*this->numberLeaves) -1;//árvore binária completa
	this->height = ceil(log2(this->numberLeaves));
	this->tree.resize(this->numberNodes);
}

uint64_t RMMTree::getInt(const uint8_t  idx, const uint8_t len){
	if(idx + len > bv.size())return -1;
	if(len > 64)return -1;

/* 	uint8_t offset = idx & 0x3F;
	const uint64_t* d = bv.data();
	d+=(idx >> 6);//dividindo por 64 */

	return readInt(bv.data() + (idx>>6), idx & 0x3F, len);

 	
	//return bits::read_int_and_move(d, offset, len);  
}

uint64_t RMMTree::readInt(const uint64_t* word, uint8_t offset, const uint8_t len){
	uint64_t w1 = (*word)& bits::lo_set[64-offset];
	
	if ((offset+len) > 64) {
		return (w1  << (offset+ len -64)) 
			|  ((*(word+1) & bits::lo_unset[64-((offset+len)&0x3F)]) >> (64-((offset+len)&0x3F)) ); 
	}
	return w1 >> (64-offset - len);
	
}

int RMMTree::bitsread(int s,int e){
	int value=0;
	for(int j=s;j<=e;j++)value = (value << 1) + bv[j] ;
	return value;
}

int RMMTree::leafInTree(int k){
	int t = pow(2,ceil((double)log2(numberLeaves)));
	if(k <= (2*numberLeaves) - t)return t-1+k;
	else return t-1-numberLeaves+k;
}

int RMMTree::numLeaf(int v){
	int t = pow(2,ceil((double)log2(numberLeaves)));
	v+=1;//ajuste do índice do vetor
	if(v >= t )return v - t +1;
	else return v - t +1 + numberLeaves;
}

void RMMTree::buildingTableC(){
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

void RMMTree::buildingTree(){
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

void RMMTree::buildingLeaves(int s, int e, int k){
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

void RMMTree::buildingInternalNodesRoot(){
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

void RMMTree::printNode(vector<Node> v, int i){
	cout << "v[" << i<< "].e = " << v[i].excess << "; "
		 << "v[" << i<< "].m = " << v[i].excessMin << "; "
		 << "v[" << i<< "].M = " << v[i].excessMax << "; "
	     << "v[" << i << "].n = " << v[i].numberExcessMin << ".\n"
		 << endl;
}

void RMMTree::printTree(){
	int i;
	cout << " ----- Root ----- \n";
	printNode(tree, 0);
	cout << " ----- Internal nodes ----- \n";
	for(i=1;i<numberNodes-numberLeaves;i++)printNode(tree, i);

	cout << " ----- Folhas -----" << endl;
	for(;i<numberNodes;i++)printNode(tree,i);
}

void RMMTree::printTableC(){
	cout << "---- Tabela C, para acelerar a construção da RMM-tree ----" << "\n\n";
	for(int i=0;i<pow(2,w);i++)printNode(tableC,i);
}

void RMMTree::printInfoTree(){
    cout << "BPS: " << this->bv << '\n'
		 << "Tamanho de bloco: " << sizeBlock << '\n'
		 << "Quantidade de folhas: " << numberLeaves << '\n'
		 << "Quantidade de nós: " << numberNodes << '\n'
		 << "Altura da árvore: " << height << '\n'
		 << endl;
}

int RMMTree::fwdBlock(int i,int d,int *dr){
	int p;
	int f = ceil((double)(i+1)/w)-1;
	int t = (ceil((double)(i+2)/sizeBlock) * (sizeBlock/w))-1;

	*dr=0;
	
	//varre a partir do próximo elemento, chegando ao limite
	for(int j=i+1;j<=(f*w)+1;j++){
		*dr += (bv[j] == 1)? 1 : -1;
		if(*dr == d)return j;
	}

	//Verifica se "d" está no bloco de tamanho "b" ao qual "i" pertence
	//A ideia é verificar o excesso local dos próximos bloquinhos
	for(p=f+1; p<=t;p++){
		int x = bitsread(p*w, (p*w)+1);//se alterar a função bitsread, passar apenas w, q é a quantidade de bits q devo ler

		if(*dr + tableC[x].excessMin <= d && *dr + tableC[x].excessMax >= d){
			break;
		}
		*dr += tableC[x].excess;
	}

	if(p > t)return -1;
	
	//Varre outro subbloco, onde provavelmente está "d"
	for(int j= p*w; j <= (p*w)+1;j++){
		*dr += (bv[j] ==1)? 1:-1;
		if(*dr == d)return j;
	}
    return -1;
}

int RMMTree::bwdBlock(int i,int d,int *dr){
	int p,x,j;
	int f = ceil((double)(i+1)/w)-1;//marca o início do grande bloco
	int t = (ceil((double)(i+2)/sizeBlock) * (sizeBlock/w))-1;//término do primeiro sub-bloco

	*dr=0;
	
	for(j=i;j>=f*w;j--){
		*dr += (bv[j] == 1)? -1 : 1;
		if(*dr == d)return j;
	}
	
	//TODO cálculo de p e t estão errados: vê
	//lê os b bits (em grupo), fazendo uso da tabela C. para verificar se o excesso está no bloco de i
	for(p=f;p>=t;p--){
		x = bitsread((p*w)-2,(p*w)-1);
		if( (*dr - tableC[x].excess + tableC[x].excessMin <= d)&& (*dr + tableC[x].excessMax >= d) ){
			break;
		}
		*dr -= tableC[x].excess;
	}

	if(p < t)return -1;
	for(j=(p*w)-1; j>=(p*w)-2;j--){
		*dr += (bv[j] == 1)? -1 : 1;
		if(*dr == d)return j;
	}
	return -1;
}

int RMMTree::fwdSearch(int i,int d){
	int j,k,v, dr=0;

	j= fwdBlock(i,d,&dr);
    if(dr == d) return j;

	k = ceil((double)(i+2)/sizeBlock);
	v = leafInTree(k)-1;
	
	/*O primeiro teste verifica se o nó é o último do seu nível, o segundo é o que que queremos
	propriamente dito o nó a direita obre o excesso mínimo "d"?
	Para isso fazemos um processo de subida na árvore.
	*/
	while( ((v+1)&(v+2))!=0 && (dr+tree[v+1].excessMin > d && dr+tree[v+1].excessMax > d)){
		if(v%2 !=0)dr += tree[v+1].excess;
		v = floor((double)(v-1)/2);
	}

	if(((v+1)&(v+2)) ==0)return -1;//verifica se o nó a que chegamos é a última do seu nível

	/*
		Aqui iniciamos a descida na árvore.
		A ideia é verificar se o excesso mínimo se encontra no filho mais a esquerda ou a direita, 
		e seguir por esse filho em busca do excesso relativo desejado.
	*/
	v++;
	while(v < numberLeaves-1){/*lembre que o nosso vetor inicia em zero*/
		if((dr + tree[(2*v)+1].excessMin <= d)&&(dr + tree[(2*v)+1].excessMax >= d)) v = (2*v)+1;
		else{
			dr += tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
	}
	k = numLeaf(v);
	j = fwdBlock(((k-1)*sizeBlock)-1,d-dr,&dr);
	return j;
}

int RMMTree::bwdSearch(int i,int d){
	int j,k,v, dr=0;

	j= bwdBlock(i,d,&dr);
    if(dr == d) return j;

	k = ceil((double)(i+1)/sizeBlock);
	v = leafInTree(k)-1;

	//inicia a subida na árvore
	while( ((v+1)&v) !=0  &&  (dr - tree[v-1].excess + tree[v-1].excessMin > d) ){
		//eu preciso excluir o excesso global da folha anterior, e considerar apenas o mínimo deste
		if(v%2 == 0){
			dr-=tree[v-1].excess;
		}
		v = floor((double)(v-1)/2);
	}
	
	if( ((v+1)&v) ==0  && v!=0)return -1;/*estamos nos nós mais a esquerda da árvore, e estes ainda não contém o excesso desejado*/

	v--;
	//inicia a descida na árvore
	while(v < numberLeaves-1){
		if((dr - tree[(2*v)+2].excess +tree[(2*v)+2].excessMin) <= d){
			//descendo pelo filho à direita
			v = (2*v) + 2;
		}else{
			//descendo pelo filho à esquerda
			dr -= tree[(2*v)+2].excess;
			v = (2*v) +1;
		}
	}

	//varre o bloco bit à bit afim de encontrar a posição "j" exata.
	k = numLeaf(v)-1;
	cout << "v " << v << "\n";
	cout << "i " << ((k+1)*sizeBlock)-1 << "\n";
	j = bwdBlock(((k+1)*sizeBlock)-1,d-dr,&dr);
	return j;
}