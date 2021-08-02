#include <iostream>
#include <assert.h>
#include <vector>
#include <math.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "rmMTreeOptimized.h"

using namespace std;
using namespace sdsl;
//https://github.com/google/benchmark para monitorar o tempo
long long int temp;

RMMTree::RMMTree(int_vector<1> &bv,  int sizeBlock,  int w, int order){
	assert(!(order&(order-1)));
	this->bv = bv;
	this->b_rank1 = rank_support_v<1>(&bv);
	this->b_rank0 = rank_support_v<0>(&bv);
	this->b_rank10 = rank_support_v<10,2>(&bv);
	this->b_sel1 = select_support_mcl<1>(&bv);
	this->b_sel0 = select_support_mcl<0>(&bv);
	this->b_sel10 = select_support_mcl<10,2>(&bv);
	this->sizeBlock = sizeBlock;
	this->w =  w;
	this->size = bv.size();
    this->order = order;
	this->numberLeaves = ceil( (double)bv.size()/(sizeBlock*order) );
	this->numberNodes = ceil((double)(order*numberLeaves-1)/(order-1));
	this->height = cLog_m(this->numberLeaves,this->order);
	this->tree.resize(this->numberNodes);
	temp = sqrt(order);
}

unsigned long long RMMTree::fLog_2(unsigned long long  n){
	return  (8*sizeof (unsigned long long) - __builtin_clzll(n) - 1);
}

unsigned long long RMMTree::cLog_2(unsigned long long  n){
	return  fLog_2(2*n -1);
}

unsigned long long RMMTree::cLog_m(unsigned long long  n,unsigned long long  m){
	return ceil(log2(n)/log2(m));
}

unsigned long long RMMTree::fLog_m(unsigned long long  n,unsigned long long  m){
	return log2(n)/log2(m);
}

long long int RMMTree::min(long long int a , long long int b){
	return (a < b )? a:b;
}

long long int RMMTree::bitsread(long long int s,long long int e){
	long long int value=0;
	for(long long int j=s;j<=e && j<size;j++)value = (value << 1) + bv[j] ;
	return value;
}


long long int RMMTree::leafInTree(long long int k){
	//se number Leaves é potência de order, todas as folhas estão no mesmo nível

	if(height==0)return 0;

	long long int nNodesPrevLevel = 1 << (temp*(height-1));
	long long int totalNodesExcludingLastLevel = ceil((order*nNodesPrevLevel -1)/(order-1));
	if(k < numberNodes -totalNodesExcludingLastLevel){//último nível
		return totalNodesExcludingLastLevel+k; 
	}
	return totalNodesExcludingLastLevel  - numberLeaves + k;
}

long long int RMMTree::numLeaf(long long int v){
	//se number Leaves é potência de order, todas as folhas estão no mesmo nível
	long long int nNodesPrevLevel = 1 << (temp*(height-1));
	long long int totalNodesExcludingLastLevel = (order*nNodesPrevLevel -1)/(order-1);


	if(v >= totalNodesExcludingLastLevel){
		return v - totalNodesExcludingLastLevel;
	}
	return v - totalNodesExcludingLastLevel + numberLeaves;
}


int RMMTree::numKey(int k,long long int i){
	long int startBlock= k*sizeBlock*order;
	return (i-startBlock)/sizeBlock;
}

void RMMTree::buildingTree(){
    /*pré-computa tabela C, para acelerar a cosntruçã da RMM-tree*/
	buildingTableC();

	/*constrói as folhas da árvore*/
	buildingLeaves();
	/*Inicia a construção dos nós internos e raíz*/
	buildingInternalNodesRoot();
}

void RMMTree::buildingTableC(){
	Key key;
	key.excess = 0;
	key.excessMax = 0 - w;
	key.excessMin = w;
	key.numberExcessMin = 0;

	for(long long int i=0; i < (1<<w); i++){
		tableC.push_back(key);

		for(long long int j=0; j<w;j++){
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

void RMMTree::buildingLeaves(){
	long long int x,v;
	int numKey=0;
	Key key;
	for(long long int k=0; k < numberLeaves;k++){
		v = leafInTree(k);
		while (tree[v].nKeys < order && numKey < ceil(size/sizeBlock)){
			key.excess = 0;
			key.excessMax = 0 -w;
			key.excessMin = 0 +w;
			key.numberExcessMin = 0;
			for(long long int p = (numKey*(sizeBlock/w))+1;p<=((numKey+1)*sizeBlock)/w;p++){
				x = bitsread((p-1)*w,(p*w)-1);
				if(key.excess + tableC[x].excessMax > key.excessMax){
					key.excessMax = key.excess + tableC[x].excessMax;
				}
				if(key.excess + tableC[x].excessMin < key.excessMin){
					key.excessMin = key.excess + tableC[x].excessMin;
					key.numberExcessMin = tableC[x].numberExcessMin;
				}else if(key.excess + tableC[x].excessMin == key.excessMin){
					key.numberExcessMin = key.numberExcessMin + tableC[x].numberExcessMin;
				}
				key.excess += tableC[x].excess;
			}
			tree[v].keys.push_back(key);
			tree[v].nKeys++;
			numKey++;
		}
	}
}

void RMMTree::buildingInternalNodesRoot(){
	Key key;
	for(long long int v= numberNodes - numberLeaves -1; v>=0;v--){
		for(long long int child=1; child <= order && (v*order+child)<numberNodes;child++){
			key.excess = 0;
			key.excessMax = 0 -w;
			key.excessMin = 0+w;
			key.numberExcessMin = 0;
			//define o filho alvo
			long long int targetSon = (order*v)+child;
			
			//cria laço para pecorrer as chaves do filho alvo
			for(long long int i=0; i < tree[targetSon].nKeys;i++){
				if(key.excessMax < key.excess + tree[targetSon].keys[i].excessMax){
					key.excessMax = key.excess + tree[targetSon].keys[i].excessMax;
				}
				if(key.excessMin > key.excess + tree[targetSon].keys[i].excessMin){
					key.excessMin = key.excess + tree[targetSon].keys[i].excessMin;
					key.numberExcessMin = tree[targetSon].keys[i].numberExcessMin;
				}else if(key.excessMin == key.excess + tree[targetSon].keys[i].excessMin){
					key.numberExcessMin+= tree[targetSon].keys[i].numberExcessMin;
				}
				key.excess += tree[targetSon].keys[i].excess;
			}
			tree[v].keys.push_back(key);
			tree[v].nKeys++;
		}
	}
}

long long int RMMTree::fwdKey(long long int i,int key,long long int k,int nKeys,int d,int &dr){
	long long int j;
	
	for(;key < nKeys;key++){
		j= fwdBlock(i,d,dr);
		if(dr == d) return j;

		i = (order*k+key+1)*sizeBlock -1;//obtém o fim da chave atual, para começar a varrer próxima chave a partir do seu início
	}
	return size;
}

long long int RMMTree::fwdBlock(long long int i,int d,int &dr){
	long long int p;
	long long int fb = ceil((double)(i+1)/w);//para calcular o limite do primeiro bloquinho (de i)
	long long int lb = ceil((double)(i+2)/sizeBlock)* (sizeBlock/w);//limite do bloco 
	
	//varre o sub-bloco a qual i+1 pertebce
	for(long long int j=i+1;j<=(fb*w)-1 && j<size;j++){
		dr += (bv[j] == 1)? 1 : -1;
		if(dr == d)return j;
	}
	
	//Verifica se "d" está contido no bloco subsequente
	for(p=fb+1; p<=lb;p++){
		long long int x = bitsread((p-1)*w, (p*w)-1);
		if(dr + tableC[x].excessMin <= d && dr + tableC[x].excessMax >= d){
			break;
		}
		dr += tableC[x].excess;
	}
	
	if(p > lb)return lb*sizeBlock;//d não está no bloco subsequente

	//Finalmente faz a varredura do subbloco subsquente ao de i+1, onde se encontra d
	for(long long int j= (p-1)*w; j <= (p*w)-1 && j<size;j++){
		dr += (bv[j] ==1)? 1:-1;
		if(dr == d)return j;
	}
    return size;
}

long long int RMMTree::fwdVerifySibling(long long int &v, int &dr, int d){
	
	//calcula parent a fim de verificar quantos írmãos de v existem a sua esquerda
	long long int parent = (v-1)/order;
	long long int child = v - (parent*order);//obtém o número de irmãos a direita de v
	cout << " numero de irmaos de v eh " <<child << endl;
	if(child==order)return size;
	v++;
	cout << " v eh " << v << " seu pai eh " << parent << " e ele tem " << child << " irmaos\n";
	//pecorre os írmãos de v
	for( ;child < tree[parent].nKeys && v<numberNodes;child++){
		
		for(long long int key=0;key<tree[v].nKeys;key++){	
			if((dr + tree[v].keys[key].excessMin <= d) && (d<= dr +tree[v].keys[key].excessMax))return key;
			dr +=  tree[v].keys[key].excess;
			//if(dr==d)return key;
		}
		v++;
	}
	
	return size;//a resposta não está nos irmãos de v
}

long long int RMMTree::fwdSearch(long long int i, int d){
	assert((i+1)>=0 && (i+1)< size);

	int dr=0;
	long long int k = (i+1)/(sizeBlock*order);//calcula a k-th folha em que se encontra i+1
	long long int v = leafInTree(k);//índice da RMM-tree onde ocorre a k-th folha
	int key = numKey(k,i+1); 
	cout << " nó v eh " << v << " k eh " << k << " key " << key << endl;
	long long int j = fwdKey(i,key, k ,tree[v].nKeys,d,dr);
	if(dr == d)return j;
	cout << " no v eh " << v << " e dr eh " << dr << endl;
	/* -----Subindo a RMM-tree ------*/
	while( v!=0 && (key=fwdVerifySibling(v,dr,d))==size){
		v = (v-1)/order;
	} 
	
	
	/* ----- Descendo a RMM-tree ------*/
	while(v < numberNodes - numberLeaves){
		/*pecorre todas achavaes do nó pelo qual estamos descendo, para encontrar a chave em que ocorre
		o excesso e descer pelo o seu nó.
		*/
		for(key=0;key<tree[v].nKeys;key++){
			if((dr+tree[v].keys[key].excessMin <=d)&& (dr + tree[v].keys[key].excessMax >=d) ){
				v = (v*order)+1+key;
				key=0;
				break;
			}
			else{
				dr += tree[v].keys[key].excess;
			}
		}
	}
 
	k = numLeaf(v);
	
	j = fwdKey((order*k+key)*sizeBlock -1,0, k ,tree[v].nKeys,d,dr);

	return (dr == d)? j : size;
}

long long int RMMTree::bwdKey(long long int i,int key,long long int k,int d, int &dr){
	long long int j;
	for(; key>=0;key--){ 
		j = bwdBlock(i,d,dr);
		if(dr==d)return j;
		i =  (order*k+key)*sizeBlock -1;//obtém o inicio da chave atual subtraído de 1, para começar a varrer a próxima chave a partir do seu final
	}
	return -1;
}

long long int RMMTree::bwdBlock(long long int i,int d, int &dr){
	long long int p,x,j;
	long long int fb = i/w;
	long long int lb = (i/sizeBlock) * (sizeBlock/w);
	for(j=min(i,size);j>=fb*w;j--){
		dr += (bv[j] == 1)? -1 : 1;
		if(dr == d)return j-1;
	}
	for(p=fb-1;p>=lb;p--){
		x = bitsread((p*w),(p+1)*w-1);
		if( (dr - tableC[x].excess + tableC[x].excessMin <= d)&& (dr - tableC[x].excess + tableC[x].excessMax >= d) ){
			break;
		}
		dr -= tableC[x].excess;
	}

	if(p < lb)return (lb*w)-1;
	for(j=(p+1)*w-1; j>=p*w; j--){
		dr += (bv[j] == 1)? -1 : 1;
		if(dr == d)return j-1;
	}
	
	return size;
}

long long int RMMTree::bwdVerifySibling(long long int &v, int &dr, int d){
	
	long long int parent = (v-1)/order;
	long long int child = v - (parent*order)-1; // obtém o número de irmãos a esquerda de v
	v--;

	for(; child >0 && v >0;child--){
		for(long long int key =tree[v].nKeys-1; key>=0;key--){
			if((dr - tree[v].keys[key].excess + tree[v].keys[key].excessMin <= d) && (d <= dr - tree[v].keys[key].excess + tree[v].keys[key].excessMax))return key;
			dr-=tree[v].keys[key].excess;
			if(dr == d)return key;
		}
		v--;
	}
	return -1;
}

long long int RMMTree::bwdSearch(long long int i,int d){
	assert(i>=0 && i < size);
	if(i==0)return -1;

	int dr=0;
	long long int k = i/(sizeBlock*order);
	long long int v = leafInTree(k);
	int key = numKey(k,i);

	long long int j = bwdKey(i,key,k,d,dr);
	if(dr==d) return j;
	
	/* -----Subindo a RMM-tree ------*/
	while(v!=0 && (key=bwdVerifySibling(v,dr,d))==-1){
		v = (v-1)/order;
	}

	/* ----- Descendo a RMM-tree ------*/
	while(v < numberNodes - numberLeaves){
		for(key = tree[v].nKeys-1;key>=0;key--){
			if( (dr - tree[v].keys[key].excess +tree[v].keys[key].excessMin <= d)&&(dr - tree[v].keys[key].excess +tree[v].keys[key].excessMax >= d)){
				v = (v*order)+1+key;
				key = tree[v].nKeys -1;
				break;
			}
			else{
				dr -= tree[v].keys[key].excess;
			}
		}
	}
	
	k = numLeaf(v);
	if(dr == d)return (k*sizeBlock*order)+(key*sizeBlock) -1;

	j = bwdKey(k*(order*sizeBlock) + ((key+1)*sizeBlock) -1,tree[v].nKeys-1,k,d,dr);

	return (dr==d)? j : -1;
}


long long int RMMTree::findClose(long long int i){
	assert(i>=0 && i<size);

	if((i == 0) && bv[i]==1)return size -1;
	return (bv[i] == 0) ? i : fwdSearch(i,-1);
}

long long int RMMTree::findOpen(long long int i){
	assert(i>=0 && i < size);

	if((i == (int)(size-1)) && bv[i]==0)return 0;
	return (bv[i] == 1) ?  i : bwdSearch(i,0)+1;
}

void RMMTree::printNode(vector<Key> vector, long long int i){
	cout << "v[" << i<< "].e = " << vector[i].excess << "; "
		 << "v[" << i<< "].m = " << vector[i].excessMin << "; "
		 << "v[" << i<< "].M = " << vector[i].excessMax << "; "
	     << "v[" << i << "].n = " << vector[i].numberExcessMin << ".\n"
		 << endl;
}

void RMMTree::printTableC(){
	cout << "---- Tabela C, para acelerar a construção da RMM-tree ----" << "\n\n";
	for(long long int i=0;i<(1 << w);i++)printNode(tableC,i);
}

void RMMTree::printTree(){
	long long int v,leaf;
	cout << " ----- Root and Internal nodes ----- \n";
	for(v=0;v<numberNodes-numberLeaves;v++){
		cout << " Nó " << v << "\n";
		for(long long int k=0;k<tree[v].nKeys; k++){
			cout << "Chave " << k << "\n";
			printNode(tree[v].keys , k);
		}
	}
	cout << " ----- Folhas -----" << endl;
	for(long long int k=0;v<numberNodes;v++,k++){
		leaf=numLeaf(v);
		cout << leaf << "-th folha " << " - nó " << v << ": área de cobertura: B[" << leaf*sizeBlock*order << "," << (leaf+1)*sizeBlock*order -1<< "]\n";
		for(long long int k=0;k<tree[v].nKeys; k++){
			cout << "Chave " << k << "\n";
			printNode(tree[v].keys , k);
		}
	}
}

void RMMTree::printInfoTree(){
    cout << "Tamanho de bloco: " << sizeBlock << '\n'
		 << "Ordem da árvore " << order << '\n'
		 << "Quantidade de folhas: " << numberLeaves << '\n'
		 << "Quantidade de nós: " << numberNodes << '\n'
		 << "Altura da árvore: " << height << '\n'
		 << endl;
}

