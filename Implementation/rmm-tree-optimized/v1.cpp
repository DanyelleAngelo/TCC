#include <iostream>
#include <assert.h>
#include <vector>
#include <math.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "rmMTreeOptimized.h"
#include "v1.h"

using namespace std;
using namespace sdsl;

RMMTree_Kary_V1::RMMTree_Kary_V1(int_vector<1> &bv, int sizeBlock,  int w, int order) : RMMTree_Kary(bv,sizeBlock,w,order){

}

bool RMMTree_Kary_V1::fwdVerifyParent(long long int &v, int &dr, int d){
	long long int parent = (v-1)/order;
	int key = v - (parent*order);//marca a chave em parent correspondente ao primeiro irmão à direita de v.
	v = parent;

    if(key==tree[v].nKeys)return false;

	for(; key < tree[v].nKeys; key++){
		if((dr + tree[v].keys[key].excessMin <= d) && (d<= dr +tree[v].keys[key].excessMax)){
			v = (v*order) + 1 + key;
			return true;
		}
		dr +=  tree[v].keys[key].excess;
	}
	return false;//a resposta não está nos irmãos de v
}

//@Override
long long int RMMTree_Kary_V1::fwdSearch(long long int i, int d){
   assert((i+1)>=0 && (i+1)< size);

	int dr=0;
	long long int k = (i+1)/(sizeBlock*order);//calcula a k-th folha em que se encontra i+1
	long long int v = leafInTree(k);//índice da RMM-tree onde ocorre a k-th folha


	long long int j= fwdBlock(i,d,dr);//faz a varredura primeiramente na chave de i
	if(dr == d) return j;

	//se a resposta não estver na chave do indice passado, busca nas chaves sequentes desta folha
	int key = numKey(k,i+1);
	if(key < tree[v].nKeys-1){
		i = (order*k+key+1)*sizeBlock -1;
		j = fwdKey(i,v,key+1, k,d,dr);
		if(dr == d)return j;
	}
	if(numLeaf(v) == numberLeaves-1)return size;//varremos a última folha, mas não encontramos a resposta
    
	key=0;
	/* -----Subindo a RMM-tree ------*/
	bool target = false;
	while(v!=0 && target==false){
		target = fwdVerifyParent(v,dr,d);
	} 
	
	if(v==0 && target==false)return size;//varremos todas as chaves do nó 0, mas mesmo assim não encontramos o excesso.

	/* ----- Descendo a RMM-tree ------*/
	while(v < numberNodes - numberLeaves){
		/*pecorre todas a chaves do nó pelo qual estamos descendo, para encontrar a chave em que ocorre
		o excesso e descer pelo o seu nó.
		*/
		for(key=0;key<tree[v].nKeys;key++){
			if((dr+tree[v].keys[key].excessMin <=d)&& (dr + tree[v].keys[key].excessMax >=d) ){
				v = (v*order)+1+key;
				break;
			}
			else{
				dr += tree[v].keys[key].excess;
			}
		}
	}

	k = numLeaf(v);
	j = fwdKey((order*k+key)*sizeBlock -1,v,key, k,d,dr);
	return (dr == d)? j : size;
}

bool RMMTree_Kary_V1::bwdVerifyParent(long long int &v, int &dr, int d){
	long long int parent = (v-1)/order;
	int key =v - (parent*order+1)-1;
	v =parent;

    if(key < 1)return false;
    
	for( ; key>=0;key--){
		if((dr - tree[v].keys[key].excess + tree[v].keys[key].excessMin <= d) && (d <= dr - tree[v].keys[key].excess + tree[v].keys[key].excessMax)){
			v = (v*order)+1+key;
			return true;
		}
		dr-=tree[v].keys[key].excess;
	}
	return false;
}

long long int RMMTree_Kary_V1::bwdSearch(long long int i,int d){
	assert(i>=0 && i < size);
	if(i==0)return -1;

	int dr=0;
	long long int k = i/(sizeBlock*order);
	long long int v = leafInTree(k);
	
	long long int j = bwdBlock(i,d,dr);
	if(dr==d)return j;

	//se a resposta não estver na chave do indice passado, busca nas chaves anteriores desta folha
	int key = numKey(k,i);
	if(key>0){
		i= (order*k+key)*sizeBlock -1;
		j = bwdKey(i,v,key-1,k,d,dr);
		if(dr==d) return j;
	}

	if(numLeaf(v) == 0)return -1;
	key=0;
	/* -----Subindo a RMM-tree ------*/
	bool target = false;
	while(v!=0 && !target){
		target = bwdVerifyParent(v,dr,d);
	}

	if(v==0 && target==false)return -1;//varremos todas as chaves do nó 0, mas mesmo assim não encontramos o excesso.
	
	/* ----- Descendo a RMM-tree ------*/
	while(v < numberNodes - numberLeaves){
		for(key = tree[v].nKeys -1; key>=0; key--){
			if( (dr - tree[v].keys[key].excess +tree[v].keys[key].excessMin <= d)&&(dr - tree[v].keys[key].excess +tree[v].keys[key].excessMax >= d)){
				v = (v*order)+1+key;
				key = tree[v].nKeys -1;
				break;
			}
			dr -= tree[v].keys[key].excess;
		}
	}

	k = numLeaf(v);
	if(dr == d)return (k*order*sizeBlock) + ((key+1)*sizeBlock) -1;
	j = bwdKey(k*(order*sizeBlock) + ((key+1)*sizeBlock) -1,v,key,k,d,dr);
	return (dr==d)? j : -1;
}