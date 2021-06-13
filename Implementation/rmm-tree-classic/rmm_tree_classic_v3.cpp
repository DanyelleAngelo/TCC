#include <iostream>
#include <assert.h>
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



RMMTree::RMMTree(int_vector<1> &bv, int sizeBlock,int w){
	this->bv = bv;
	this->b_rank = rank_support_v<1>(&bv);
	this->sizeBlock = sizeBlock;
	this->w =  w;
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

int RMMTree::min(int a , int b){
	return (a < b )? a:b;
}

int RMMTree::bitsread(int s,int e){
	int value=0;
	for(int j=s;j<=e;j++)value = (value << 1) + bv[j] ;
	return value;
}

int RMMTree::leafInTree(int k){
	int t = pow(2,ceil((double)log2(numberLeaves)));
	if(k < (2*numberLeaves) - t)return t-1+k;
	else return t-1-numberLeaves+k;
}

int RMMTree::numLeaf(int v){
	/*Considere-se a numeração das folhas de 1 até r*/
	int t = pow(2,ceil((double)log2(numberLeaves))) - 1;

	if(v >= t )return v - t + 1;
	else return v - t  + numberLeaves + 1;
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
	assert(sizeBlock%w ==0);
	
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

void RMMTree::printNode(vector<Node> vector, int i){
	cout << "v[" << i<< "].e = " << vector[i].excess << "; "
		 << "v[" << i<< "].m = " << vector[i].excessMin << "; "
		 << "v[" << i<< "].M = " << vector[i].excessMax << "; "
	     << "v[" << i << "].n = " << vector[i].numberExcessMin << ".\n"
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
	int f = ceil((double)(i)/w);
	int t = (ceil((double)(i+1)/sizeBlock) * (sizeBlock/w));

	//varre o sub-bloco a qual i+1 pertebce
	for(int j=i+1;j<=(f*w)+1;j++){
		*dr += (bv[j] == 1)? 1 : -1;
		if(*dr == d)return j;
	}

	//Verifica se "d" está contido no bloco subsequente
	for(p=f; p<=t;p++){
		int x = bitsread((p+1)*w, ((p+1)*w)+1);
		if(*dr + tableC[x].excessMin <= d && *dr + tableC[x].excessMax >= d){
			break;
		}
		*dr += tableC[x].excess;
	}

	if(p > t)return t*w;//d não está no bloco subsequente

	//Finalmente faz a varredura do subbloco subsquente ao de i+1, onde se encontra d
	for(int j= (p+1)*w; j <= ((p+1)*w)+1;j++){
		*dr += (bv[j] ==1)? 1:-1;
		if(*dr == d)return j;
	}
    return bv.size();
}

int RMMTree::bwdBlock(int i,int d,int *dr){
	int p,x,j;
	int f = floor((double)i/w);
	int t = (floor((double)i/sizeBlock) * (sizeBlock/w));
	
	for(j=i;j>=f*w;j--){
		*dr += (bv[j] == 1)? -1 : 1;
		if(*dr == d)return j-1;
	}
	
	for(p=f-1;p>=t;p--){
		x = bitsread((p*w),(p*w)+1);
		if( (*dr - tableC[x].excess + tableC[x].excessMin <= d)&& (*dr - tableC[x].excess + tableC[x].excessMax >= d) ){
			break;
		}
		*dr -= tableC[x].excess;
	}

	if(p < t)return (t*w)-1;

	for(j=(p*w)+1; j>=(p*w);j--){
		*dr += (bv[j] == 1)? -1 : 1;
		if(*dr == d)return j-1;
	}
	return bv.size();
}

int RMMTree::minBlock(int i,int j, int *d){
	int p,x, m=w;
	int fb = ceil((double)(i+1)/w);//para calcular o limite do primeiro bloquinho (de i)
	int lb = floor((double)j/w);//limite do bloco de j
	int lim = min(j,(fb*w)-1);

	for(p=i; p <= lim;p++){
		*d += (bv[p] == 1)? 1 : -1;
		if(*d < m)m=*d;
	}
	
	if(j<=lim)return m;//j está no mesmo bloco de i, e o excesso máximo foi encontrado
	
	//varre os próximos blocos de tamanho w.
	for(p=fb+1; p <=lb;p++){
		x = bitsread(p*w, (p*w)+1);

		if((*d + tableC[x].excessMin) < m)m = *d + tableC[x].excessMin;
		*d += tableC[x].excess;
	}
	
	//varremos os blocos anteriores à j, mas ainda não passamos por j
	for(p=lb*w;p<=j;p++){
		*d += (bv[p] == 1)? 1 : -1;
		if(*d < m)m=*d;
	}
	
	return m;
}

int RMMTree::maxBlock(int i,int j, int *d){
	int p,x, eM=-w;
	int fb = ceil((double)(i+1)/w);//para calcular o limite do primeiro bloquinho (de i)
	int lb = floor((double)j/w);//limite do bloco de j
	int lim = min(j,(fb*w)-1);
	
	for(p=i; p <= lim;p++){
		*d += (bv[p] == 1)? 1 : -1;
		if(*d > eM)eM=*d;
	}
	
	if(j<=lim)return eM;//j está no mesmo bloco de i, e o excesso máximo foi encontrado
	
	//varre os próximos blocos de tamanho w.
	for(p=fb+1; p <=lb;p++){
		x = bitsread(p*w, (p*w)+1);

		if((*d + tableC[x].excessMax) > eM)eM = *d + tableC[x].excessMax;
		*d += tableC[x].excess;
	}
	
	//varremos os blocos anteriores à j, mas ainda não passamos por j
	for(p=lb*w;p<=j;p++){
		*d += (bv[p] == 1)? 1 : -1;
		if(*d > eM)eM=*d;
	}

	return eM;
}

int RMMTree::fwdSearch(int i,int d){
	int j,k,v, dr=0;

	j= fwdBlock(i,d,&dr);
	
    if(dr == d) return j;

	k = ceil((double)(i+1)/sizeBlock);//calcula a k-th folha em que se encontra i+1
	v = leafInTree(k);//índice da RMM-tree onde ocorre a k-th folha

	/* -----Subindo a RMM-tree ------*/
	while( ((v+1)&(v+2))!=0 && ( (dr+tree[v+1].excessMin > d) && (dr+tree[v+1].excessMax > d) )){
		if(v%2 !=0)dr += tree[v+1].excess;
		v = floor((double)(v-1)/2);
	}
	
	if(((v+1)&(v+2)) ==0)return bv.size();//verifica se o nó a que chegamos é a última do seu nível
	
	v++;/*o excesso procurado está no nó à direita do último verificado*/

	/* ----- Descendo a RMM-tree ------*/
	while(v < numberLeaves-1){
		if((dr + tree[(2*v)+1].excessMin <= d) && (dr + tree[(2*v)+1].excessMax >= d)){
			v = (2*v)+1;
		}
		else{
			dr += tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
	}

	k = numLeaf(v); 
	j = fwdBlock(((k-1)*sizeBlock)-1,d,&dr);
	return (dr == d)? j : bv.size();/*Varre o boclo da folha anterior*/
}

int RMMTree::bwdSearch(int i,int d){
	int j,k,v, dr=0;

	if(i==0){
		return (d==0)? -1 : bv.size();
	}
	
	j= bwdBlock(i,d,&dr);
    if(dr == d) return j;

	k = floor((double)i/sizeBlock);
	v = leafInTree(k);
	
	/* -----Subindo a RMM-tree ------*/
	while( ((v+1)&v) !=0  &&  ((dr - tree[v-1].excess + tree[v-1].excessMin > d) && (dr - tree[v-1].excess + tree[v-1].excessMax > d)) ){
		if(v%2 == 0){
			dr-=tree[v-1].excess;
		}
		v = floor((double)(v-1)/2);
	}

	if( ((v+1)&v) ==0 )return bv.size();/*estamos nos nós mais a esquerda da árvore, e estes ainda não contém o excesso desejado*/

	v--;/*o excesso procurado está no nó à esquerda do último verificado*/
	
	/* ----- Descendo a RMM-tree ------*/
	while(v < numberLeaves-1){
		if( (dr - tree[(2*v)+2].excess +tree[(2*v)+2].excessMin <= d)&&(dr - tree[(2*v)+2].excess +tree[(2*v)+2].excessMax >= d)){
			v = (2*v) + 2;
		}else{
			dr -= tree[(2*v)+2].excess;
			v = (2*v) +1;
		}
	}

	k = numLeaf(v);
	if(dr == d)return (k*sizeBlock)-1 ;
	j=bwdBlock((k*sizeBlock)-1,d,&dr);/*Ao chamar bwdBlock a ideia é varrer do último bit da folha k até o seu primeiro bit.*/

	return (dr==d)? j : bv.size();
}

int RMMTree::minExcess(int i,int j){
	int d=0;
	int k_i = floor((double)i/sizeBlock);//onde termina a folha que cobre i.
	int k_j = floor((double)j/sizeBlock);
	int m   = minBlock(i,min(((k_i+1)*sizeBlock)-1,j),&d);
	
	if(j <= (k_i+1)*sizeBlock)return m;//j e i pertencem ao mesmo bloco

	int v   = leafInTree(k_i);//índice da folha 	que cobre a área de i
	int v_j = leafInTree(k_j);//índice da folha que cobre a área de j

	//inicia a subida na árvore, paramos quando estamos prestes a encontrar um nó que não está dentro do intervalo B[i,j]
	/** TODO: segunda condição de parada apresenta problema para alguns intervalos. ver [0,10]*/
	while(v+1 > v_j ||  (int)ceil((double)v_j/(1<< (int)(ceil(log2(v_j) - log2(v+1))))) !=v+1){

		if(v%2==1){//filho da esquerda, verificamos o excesso mínimo do filho da direita
			if(d+tree[v+1].excessMin < m)m = d+tree[v+1].excessMin;
			d+= tree[v+1].excess; 
		}
		v = floor((double)(v-1)/2);;
	}
	v++;
	//iniciamos a descida na árvore
	while(v < numberLeaves-1){

		if(d+tree[v].excessMin >=m)return m;
		if( floor(v_j/(1 << ((int)floor(log2(v_j) - log2((2*v)+1)))))-1!= (2*v)+1){
			if(d+tree[(2*v)+1].excessMin < m)m= d+tree[(2*v)+1].excessMin;
			d+= tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
		else v = (2*v)+1;
	}
	
	if(d+tree[v].excessMin >=m)return m;

	int dr=0;
	int mr = minBlock((k_j)*sizeBlock,j,&dr);
	return (d + mr < m) ? (d + mr) : m;
}

int RMMTree::maxExcess(int i,int j){
	int d=0;
	int k_i = floor((double)i/sizeBlock);//onde termina a folha que cobre i.
	int k_j = floor((double)j/sizeBlock);
	int eM   = maxBlock(i,min(((k_i+1)*sizeBlock)-1,j),&d);
	
	if(j <= (k_i+1)*sizeBlock)return eM;//j e i pertencem ao mesmo bloco

	int v   = leafInTree(k_i);//índice da folha 	que cobre a área de i
	int v_j = leafInTree(k_j);//índice da folha que cobre a área de j
	
	//inicia a subida na árvore, paramos quando estamos prestes a encontrar um nó que não está dentro do intervalo B[i,j]
	while(v+1 > v_j || (int)ceil((double)v_j/(1<< (int)(ceil(log2(v_j) - log2(v+1))))) !=v+1){

		if(v%2==1){//filho da esquerda, verificamos o excesso mínimo do filho da direita
			if(d+tree[v+1].excessMax > eM)eM = d+tree[v+1].excessMax;
			d+= tree[v+1].excess; 
		}
		v = floor((double)(v-1)/2);;
	}

	v++;

	//iniciamos a descida na árvore
	while(v < numberLeaves-1){

		if(d+tree[v].excessMax <=eM)return eM;
		if( floor(v_j/(1 << ((int)floor(log2(v_j) - log2((2*v)+1)))))-1!= (2*v)+1){
			if(d+tree[(2*v)+1].excessMax > eM)eM= d+tree[(2*v)+1].excessMax;
			d+= tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
		else v = (2*v)+1;
	}

	if(d+tree[v].excessMax <=eM)return eM;

	int dr=0;
	int mr = maxBlock((k_j)*sizeBlock,j,&dr);
	return (d + mr > eM) ? (d + mr) : eM;
}

int RMMTree::rmq(int i,int j){
	/** TODO: verificar o caso em que i=0. O excesso mínimo para esse caso é 1 (o que acontece em 0). e bps->rmq retorna 39. */
	int m = minExcess(i,j);
	
	return fwdSearch(i-1,m);
}

int RMMTree::rMq(int i,int j){
	int m = maxExcess(i,j);
	
	if(i==0){
		m += (bv[i]==1)? -1 : 1;
		return fwdSearch(0,m);
	}
	return fwdSearch(i-1,m);
}

int RMMTree::findclose(int i){
	if((i == 0) && bv[i]==1)return bv.size() -1;
	return (bv[i] == 0) ? i : fwdSearch(i,-1);
}

int RMMTree::findopen(int i){
	if((i == (int)(bv.size()-1)) && bv[i]==0)return 0; /** TODO: não funciona se tivermos uma floresta com mais de uma árvore */
	return (bv[i] == 1) ?  i : bwdSearch(i,0)+1;
}

int RMMTree::enclose(int i){
	if(bv[i]==0)return findopen(i);
	int j = bwdSearch(i,-2);
	return (j == (int)bv.size()) ? j : j+1;
}