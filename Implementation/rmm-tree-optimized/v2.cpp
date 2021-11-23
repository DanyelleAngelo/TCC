#include <iostream>
#include <assert.h>
#include <vector>
#include <math.h>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "v1.h"
#include "v2.h"

using namespace std;
using namespace sdsl;

RMMTree_Kary_V2::RMMTree_Kary_V2(int_vector<1> &bv, int sizeBlock,  int w, int order) : RMMTree_Kary_V1(bv,sizeBlock,w,order){

}


//@Override
long long int RMMTree_Kary_V2::fwdSearch(long long int i, int d){
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
    
	key = fwdVerifySibling(v,dr,d);//verifica os irmãos do nó folha

	/* -----Subindo a RMM-tree ------*/
	bool target = false;
	while(v!=0 && !target){
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

long long int RMMTree_Kary_V2::bwdSearch(long long int i,int d){
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