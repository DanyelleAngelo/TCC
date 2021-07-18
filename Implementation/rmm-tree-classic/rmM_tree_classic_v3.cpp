#include <iostream>
#include <assert.h>
#include "rmMTreeClassic.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bits.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <algorithm>
#include <vector>
#include <math.h>


using namespace std;
using namespace sdsl;

RMMTree::RMMTree(int_vector<1> &bv, int sizeBlock,int w){
	this->bv = bv;
	this->b_rank1 = rank_support_v<1>(&bv);
	this->b_rank0 = rank_support_v<0>(&bv);
	this->b_rank10 = rank_support_v<10,2>(&bv);
	this->b_sel1 = select_support_mcl<1>(&bv);
	this->b_sel0 = select_support_mcl<0>(&bv);
	this->b_sel10 = select_support_mcl<10,2>(&bv);
	this->sizeBlock = sizeBlock;
	this->w =  w;
	this->numberLeaves = ceil((double)(bv.size()/sizeBlock));
	this->numberNodes = (2*this->numberLeaves) -1;//árvore binária completa
	this->height = cLog_2(this->numberLeaves);
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

unsigned long long RMMTree::fLog_2(unsigned long long  n){
	return  (8*sizeof (unsigned long long) - __builtin_clzll(n) - 1);
}

unsigned long long RMMTree::cLog_2(unsigned long long  n){
	return  fLog_2(2*n -1);
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
	int t = 1 << height;
	if(k < (2*numberLeaves) - t)return t-1+k;
	else return t-1-numberLeaves+k;
}

int RMMTree::numLeaf(int v){
	int t = 1 << height;

	if(v >= t-1 )return v - t  + 1 ;
	else return v - t  + numberLeaves + 1 ;
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

	/*constrói as folhas da árvore*/
	buildingLeaves();
	/*Inicia a construção dos nós internos e raíz*/
	buildingInternalNodesRoot();
}

void RMMTree::buildingLeaves(){
	int x,v;
	
	for(int k=0;k<numberLeaves;k++){
		v = leafInTree(k);
		tree[v].excess = 0;
		tree[v].excessMax = 0 - w;
		tree[v].excessMin =w;
		tree[v].numberExcessMin =0;
		
		for(int p = (k*(sizeBlock/w))+1;p<=((k+1)*sizeBlock)/w;p++){
			x = bitsread((p-1)*w,(p*w)-1);
			if(tree[v].excess + tableC[x].excessMax > tree[v].excessMax){
				tree[v].excessMax = tree[v].excess + tableC[x].excessMax;
			}
			if(tree[v].excess + tableC[x].excessMin < tree[v].excessMin){
				tree[v].excessMin = tree[v].excess + tableC[x].excessMin;
				tree[v].numberExcessMin = tableC[x].numberExcessMin;
			}else if(tree[v].excess + tableC[x].excessMin == tree[v].excessMin){
				tree[v].numberExcessMin = tree[v].numberExcessMin + tableC[x].numberExcessMin;
			}
			tree[v].excess += tableC[x].excess;
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
	for(int i=0;i<(1 << w);i++)printNode(tableC,i);
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
	int fb = ceil((double)(i+1)/w);//para calcular o limite do primeiro bloquinho (de i)
	int lb = ceil((double)(i+2)/sizeBlock)* (sizeBlock/w);//limite do bloco 
	
	//varre o sub-bloco a qual i+1 pertebce
	for(int j=i+1;j<=(fb*w)-1;j++){
		*dr += (bv[j] == 1)? 1 : -1;
		if(*dr == d)return j;
	}
	
	//Verifica se "d" está contido no bloco subsequente
	for(p=fb+1; p<=lb;p++){
		int x = bitsread((p-1)*w, (p*w)-1);
		if(*dr + tableC[x].excessMin <= d && *dr + tableC[x].excessMax >= d){
			break;
		}
		*dr += tableC[x].excess;
	}
	
	if(p > lb)return lb*sizeBlock;//d não está no bloco subsequente

	//Finalmente faz a varredura do subbloco subsquente ao de i+1, onde se encontra d
	for(int j= (p-1)*w; j <= (p*w)-1;j++){
		*dr += (bv[j] ==1)? 1:-1;
		if(*dr == d)return j;
	}
    return bv.size();
}

int RMMTree::bwdBlock(int i,int d,int *dr){
	int p,x,j;
	int fb = floor((double)i/w);
	int lb = (floor((double)i/sizeBlock) * (sizeBlock/w));
	
	for(j=i;j>=fb*w;j--){
		*dr += (bv[j] == 1)? -1 : 1;
		if(*dr == d)return j-1;
	}
	
	for(p=fb-1;p>=lb;p--){
		x = bitsread((p*w),(p+1)*w-1);
		if( (*dr - tableC[x].excess + tableC[x].excessMin <= d)&& (*dr - tableC[x].excess + tableC[x].excessMax >= d) ){
			break;
		}
		*dr -= tableC[x].excess;
	}

	if(p < lb)return (lb*w)-1;

	for(j=(p+1)*w-1; j>=p*w; j--){
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

int RMMTree::minCountBlock(int i,int j,int m,int *d){
	int fb = ceil((double)(i+1)/w);//para calcular o limite do primeiro bloquinho (de i)
	int lb = floor((double)j/w);//limite do bloco de j
	int lim = min(j,(fb*w)-1);
	int n=0,x;

	for(int p=i; p<=lim; p++){
		*d += (bv[p] ==1)? 1 : -1;
		if(*d==m)n++;
	}

	if(j<=lim) return n;

	for(int p=fb+1; p<=lb; p++){
		x = bitsread(p*w,(p*w)+1);
		if((*d + tableC[x].excessMin) == m)n++;
		*d += tableC[x].excess;
	}

	//varremos os blocos anteriores à j, mas ainda não passamos por j
	for(int p=lb*w;p<=j;p++){
		*d += (bv[p] == 1)? 1 : -1;
		if(*d == m)n++;
	}
	return n;
}

int RMMTree::minSelectBlock(int i,int j,int m, int *t,int *d){	
	int fb = ceil((double)(i+1)/w);//para calcular o limite do primeiro bloquinho (de i)
	int lb = floor((double)j/w);//limite do bloco de j
	int lim = min(j,(fb*w)-1);
	int p,x;
	
	for(p=i; p<=lim; p++){
		*d += (bv[p] ==1)? 1 : -1;
		if(*d==m){
			*t-=1;
			if(*t==0)return p;
		}
	}
	
	if(j<=lim) return p;
	
	for(p=fb; p<=lb; p++){
		x = bitsread(p*w,(p+1)*w-1);
		
		if((*d + tableC[x].excessMin) <=  m)break;
		
		*d += tableC[x].excess;
	}

	if(p>lb)return lb*w;
	
	//varremos os blocos anteriores à j, mas ainda não passamos por j
	for(p=lb*w;p<=j;p++){
		*d += (bv[p] == 1)? 1 : -1;
		if(*d == m){
			*t-=1;
			if(*t==0)return p;
		}
	}
	
	return p;
}

int RMMTree::fwdSearch(int i,int d){
	assert((i+1)>=0 && (i+1)< bv.size());
	
	int j,k,v, dr=0;
	
	j= fwdBlock(i,d,&dr);
	
    if(dr == d) return j;

	k = floor((i+1)/sizeBlock);//calcula a k-th folha em que se encontra i+1
	v = leafInTree(k);//índice da RMM-tree onde ocorre a k-th folha
	
	/* -----Subindo a RMM-tree ------*/
	while( ((v+1)&(v+2))!=0 && !( (dr+tree[v+1].excessMin <= d) && (d<=dr+tree[v+1].excessMax) )){

		if((v&1)==1)dr += tree[v+1].excess;//já processamos todos os filhos do pai de v. guardar seu excesso.
		v = floor((double)(v-1)/2);
	}
	
	if(((v+1)&(v+2)) ==0)return bv.size();//verifica se nó a que chegamos é a última do seu nível
	
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
	
	j = fwdBlock((k*sizeBlock)-1,d,&dr);
	
	return (dr == d)? j : bv.size();/*Varre o boclo da folha anterior*/
}

int RMMTree::bwdSearch(int i,int d){
	assert(i>=0 && i < bv.size());
	int j,k,v, dr=0;

	if(i==0)return -1;

	j= bwdBlock(i,d,&dr);
    if(dr == d) return j;
	k = floor((double)i/sizeBlock);
	v = leafInTree(k);

	/* -----Subindo a RMM-tree ------*/
	while( ((v+1)&v) !=0  &&  !((dr - tree[v-1].excess + tree[v-1].excessMin <= d) && (d <= dr - tree[v-1].excess + tree[v-1].excessMax)) ){
		
		if((v&1)== 0){
			dr-=tree[v-1].excess;
		}
		v = floor((double)(v-1)/2);
	}
	
	if( ((v+1)&v) ==0 )return -1;/*estamos nos nós mais a esquerda da árvore, e estes ainda não contém o excesso desejado*/
	
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
	
	if(dr == d)return ((k+1)*sizeBlock)-1;
	
	j=bwdBlock( ((k+1)*sizeBlock)-1,d,&dr);/*Ao chamar bwdBlock a ideia é varrer do último bit da folha k até o seu primeiro bit.*/

	return (dr==d)? j : -1;
}

int RMMTree::minExcess(int i,int j){
	assert(	(i<=j) && (i>=0 && j < bv.size()));

	int d=0;
	int k_i = floor((double)i/sizeBlock);//onde termina a folha que cobre i.
	int k_j = floor((double)(j+1)/sizeBlock);
	int m   = minBlock(i,min(((k_i+1)*sizeBlock)-1,j),&d);
	
	if(j <= ((k_i+1)*sizeBlock)-1)return m;//j e i pertencem ao mesmo bloco

	int v   = leafInTree(k_i);//índice da folha 	que cobre a área de i
	int v_j = leafInTree(k_j)+1;//índice da folha que cobre a área de j
	
	//inicia a subida na árvore, paramos quando estamos prestes a encontrar um nó que não está dentro do intervalo B[i,j]
	while(v+1 > v_j ||  (int)((v_j)/ (1<< (int)(fLog_2(v_j) - (int)fLog_2(v+2)) ) ) !=v+2){
		
		if((v&1)==1){//filho da esquerda, verificamos o excesso mínimo do filho da direita
			if(d+tree[v+1].excessMin < m)m = d+tree[v+1].excessMin;
			d+= tree[v+1].excess; 
		}
		v = floor((double)(v-1)/2);
		if(v==-1)break;
		
	}
	
	v++;
	//iniciamos a descida na árvore
	while(v < numberLeaves-1){

		if(d+tree[v].excessMin >=m)return m;
		
		if( (int)((v_j)/ (1<<  (int)(fLog_2(v_j) - (int)fLog_2((2*v)+2)) ))  != (2*v)+2){
			if(d+tree[(2*v)+1].excessMin < m)m= d+tree[(2*v)+1].excessMin;
			d+= tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
		else v = (2*v)+1;
	}

	if(d+tree[v].excessMin >=m)return m;

	int dr=0;
	int mr = minBlock(k_j*sizeBlock,j,&dr);
	return (d + mr < m) ? (d + mr) : m;
}

int RMMTree::maxExcess(int i,int j){
	assert(	(i<=j) && (i>=0 && j < bv.size()));

	int d=0;
	int k_i = floor((double)i/sizeBlock);
	int k_j = floor((double)(j+1)/sizeBlock);
	int eM   = maxBlock(i,min(((k_i+1)*sizeBlock)-1,j),&d);

	if(j <= (k_i+1)*sizeBlock)return eM;//j e i pertencem ao mesmo bloco

	int v   = leafInTree(k_i);//índice da folha 	que cobre a área de i
	int v_j = leafInTree(k_j)+1;//índice da folha que cobre a área de j

	//inicia a subida na árvore, paramos quando estamos prestes a encontrar um nó que não está dentro do intervalo B[i,j]
	while(v+1 > v_j ||   (int)((v_j)/ (1<< (int)(fLog_2(v_j) - fLog_2(v+2)) ) ) !=v+2){
		if((v&1)==1){//filho da esquerda, verificamos o excesso mínimo do filho da direita
			if(d+tree[v+1].excessMax > eM)eM = d+tree[v+1].excessMax;
			d+= tree[v+1].excess; 
		}
		v = floor((double)(v-1)/2);;
	}

	v++;

	//iniciamos a descida na árvore
	while(v < numberLeaves-1){

		if(d+tree[v].excessMax <=eM)return eM;
		if( (int)((v_j)/ (1<<  (int)(fLog_2(v_j) - fLog_2((2*v)+2)) ))  != (2*v)+2 ){
			if(d+tree[(2*v)+1].excessMax > eM)eM= d+tree[(2*v)+1].excessMax;
			d+= tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
		else v = (2*v)+1;
	}

	if(d+tree[v].excessMax <=eM)return eM;

	int dr=0;
	int mr = maxBlock(k_j*sizeBlock,j,&dr);
	return (d + mr > eM) ? (d + mr) : eM;
}

int RMMTree::minCount(int i,int j){
	assert(	(i<=j) && (i>=0 && j < bv.size()));

	int m = minExcess(i,j);
	int d = 0;
	int k_i = floor((double)i/sizeBlock);
	int k_j = floor((double)(j+1)/sizeBlock);
	int n = minCountBlock(i,min(((k_i+1)*sizeBlock)-1,j),m,&d);
	
	if(j<=((k_i+1)*sizeBlock)-1)return n;

	int v = leafInTree(k_i);
	int v_j = leafInTree(k_j)+1;

	while(v+1 > v_j ||  (int)((v_j)/ (1<< (int)(fLog_2(v_j) - fLog_2(v+2)) ) ) !=v+2){

		if((v&1)==1){//filho da esquerda, verificamos o excesso mínimo do filho da direita
			if(d+tree[v+1].excessMin == m)n += tree[v+1].numberExcessMin;
			d+= tree[v+1].excess; 
		}
		v = floor((double)(v-1)/2);;
	}
	
	v++;
	
	//iniciamos a descida na árvore
	while(v < numberLeaves-1){

		if(d+tree[v].excessMin > m)return n;
		if( (int)((v_j)/ (1<<  (int)(fLog_2(v_j) - fLog_2((2*v)+2)) ))  != (2*v)+2 ){
			if(d+tree[(2*v)+1].excessMin == m )n+= tree[(2*v)+1].numberExcessMin;
			d+= tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
		else v = (2*v)+1;
	}
	
	if(d+tree[v].excessMin > m)return n;
	
	return n+minCountBlock(k_j*sizeBlock,j,m,&d);
}

int RMMTree::minSelectExcess(int i,int j, int t){
	assert(	(i<=j) && (i>=0 && j < bv.size()));

	int m = minExcess(i,j);
	
	int d = 0;
	int k_i = floor((double)i/sizeBlock);
	int k_j = floor((double)j/sizeBlock);

	int p = minSelectBlock(i,min(((k_i+1)*sizeBlock)-1,j),m,&t,&d);
	
	if(j<=(k_i+1)*sizeBlock || t == 0)return p;

	int v = leafInTree(k_i);
	int v_j = leafInTree(k_j)+1;
	
	while(v+2 > v_j ||  (int)((v_j)/ (1<< (int)(fLog_2(v_j) - fLog_2(v+2)) ) ) !=v+2 ){

		if((v&1)==1){//filho da esquerda, verificamos o excesso mínimo do filho da direita
			if(d+tree[v+1].excessMin == m){
				if(t<=tree[v+1].numberExcessMin)break;
				t -= tree[v+1].numberExcessMin;
			}
			d+= tree[v+1].excess; 
		}
		v = floor((double)(v-1)/2);;
	}

	v++;
	
	//iniciamos a descida na árvore
	while(v < numberLeaves-1){
		if( (int)((v_j)/ (1<<  (int)(fLog_2(v_j) - fLog_2((2*v)+2))))== (2*v)+1 ){
			v= (2*v)+1;
		}
		else if(d + tree[(2*v)+1].excessMin > m){
			d += tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
		else if(t<=tree[(2*v)+1].numberExcessMin){
			v = (2*v)+1;
		}
		else{
			t-=tree[(2*v)+1].numberExcessMin;
			d += tree[(2*v)+1].excess;
			v = (2*v)+2;
		}
	}
	
	int k = numLeaf(v);
	
	p = minSelectBlock(k*sizeBlock,min(((k+1)*sizeBlock)-1,j),m,&t,&d);
	return (t==0)? p : bv.size();
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

int RMMTree::findClose(int i){
	assert(i>=0 && i<bv.size());

	if((i == 0) && bv[i]==1)return bv.size() -1;
	return (bv[i] == 0) ? i : fwdSearch(i,-1);
}

int RMMTree::findOpen(int i){
	assert(i>=0 && i < bv.size());

	if((i == (int)(bv.size()-1)) && bv[i]==0)return 0;
	return (bv[i] == 1) ?  i : bwdSearch(i,0)+1;
}

int RMMTree::enclose(int i){
	assert(i>=0 && i < bv.size());

	if(i==0)return bv.size();
	if(bv[i]==0)return findOpen(i);
	return bwdSearch(i,-2) + 1;
}

bool RMMTree::isLeaf(int x){
	assert(x>=0 && x < bv.size());

	return  (bv[x] == 1 && bv[x+1]==0);
}

bool RMMTree::isAncestor(int x, int y){
	assert(x >=0 && y < bv.size() );

	return (x <= y && y< findClose(x));
}

int RMMTree::depth(int x){
	assert(x>=0 && x<bv.size());

	return (2*b_rank1(x))-x;
}

int RMMTree::parent(int x){
	return enclose(x);
}

int RMMTree::nextSibling(int x){
	int i = findClose(x)+1;
	return (bv[i]==1)? i : bv.size();
}

int RMMTree::prevSibling(int x){
	assert(x>0 && x < bv.size()-1);

	return (bv[x-1]==0)? findOpen(x-1) : bv.size();
}

int RMMTree::child(int x,int t){
	if(t==1)return firstChild(x);

	int j = findClose(x);
	
	int p = minSelectExcess(x+1,j-1,t-1)+1; /*O segredo é obter o t-1 filho, para não precisar chamar findopen*/
	
	return (p >=j) ? bv.size() : p;
}

int RMMTree::lastChild(int x){
	return (!isLeaf(x))? findOpen(findClose(x)-1) : bv.size();
}

int RMMTree::firstChild(int x){
	return (!isLeaf(x) )? x+1 : bv.size();
}

int RMMTree::childRank(int x){
	if(x-1 ==0) return 1;
	return minCount(parent(x)+1,x);
}

int RMMTree::subtreeSize(int x){
	assert(x >=0 && x<bv.size());

	return (bv[x]==1)? ceil((findClose(x)-x+1)/2) : bv.size();
}

int RMMTree::levelAncestor(int x,int d){
	assert(x >=0 && x<bv.size());

	return (bv[x]==1)? bwdSearch(x,-d-1)+1 : bv.size();
}

int RMMTree::lca(int x,int y){
	if(isAncestor(x,y))return x;
	if(isAncestor(y,x))return y;

	return enclose(rmq(x,y)+1);
}

int RMMTree::levelNext(int x){
	return fwdSearch(findClose(x),1);
}

int RMMTree::levelPrev(int x){
	return findOpen(bwdSearch(x,0)+1);
}

int RMMTree::levelLeftMost(int d){
	return (d==1) ? 0 : fwdSearch(0,d-1);
}

int RMMTree::levelRightMost(int d){
	return (d==1) ? 0 : findOpen(bwdSearch(bv.size()-1,d)+1);
}

int RMMTree::deepestNode(int x){
	return rMq(x, findClose(x));
}

int  RMMTree::degree(int x){
	return minCount(x+1, findClose(x)-1);
}

int  RMMTree::leafRank(int x){
	assert(x>=0 && x <bv.size());

	return b_rank10(x);
}

int  RMMTree::leafSelect(int t){
	return b_sel10(t)-1;
}

int RMMTree::leftMostLeaf(int x){
	return (!isLeaf(x))? leafSelect(leafRank(x-1)+1) : x;
}

int RMMTree::rightMostLeaf(int x){
	return (!isLeaf(x))? leafSelect(leafRank(findClose(x))) : x;
}

int RMMTree::preRank(int x){
	assert(x>=0 && x<bv.size());
	return b_rank1(x);
}

int RMMTree::postRank(int x){

	return b_rank0(findClose(x));
}

int RMMTree::preSelect(int t){
	return b_sel1(t);
}

int RMMTree::postSelect(int t){
	return findOpen(b_sel0(t));
}