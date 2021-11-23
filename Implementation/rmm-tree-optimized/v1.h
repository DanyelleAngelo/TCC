#ifndef RMMTREE_KARY_V1_H
#define RMMTREE_KARY_V1_H


#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <vector>
#include "rmMTreeOptimized.h"

using namespace sdsl;

class RMMTree_Kary_V1 : public RMMTree_Kary{
    public: 
        RMMTree_Kary_V1(int_vector<1> &bv, int sizeBlock,  int w, int order);   //Construtor classe derivada

		/*!
		*	@brief Realiza um percusor de subida e depois desciada na rmM-tree,  nos nós a direita
		* 	da folha (e na própria folha)  que contém a posição i+1 em busca do excesso d.
		*	@param i: Posição a partir da qual quero realizar a busca (será incrementado de 1)
		*	@param d: excesso de "1" buscado (profundidade)
		*	@return primeira posição j > i em bv onde ocorre o excesso d.
		*/
		long long int fwdSearch(long long int i,int d);

		/*!
		*	@brief Realiza um percusor de subida e depois desciada na rmM-tree através dos nos nós 
		* 	a esquerda da folha (e na própria folha)  que contém a posição i em busca do excesso d.
		*	@param i: Posição a partir da qual quero realizar a busca.
		*	@param d: excesso de "1" buscado (profundidade)
		*	@return primeira posição j < i em bv onde ocorre o excesso d.
		*/
		long long int bwdSearch(long long int i,int d);

    protected:
		/*!
		*	@brief auxilia no processo de subida da árvore, verificando se o excesso procurado está nos intervalos do nó pai
		*	@param v: nó da rmM-tree investigado na passagem pela árvore
		*	@param dr: excesso relativo dr
		*	@param d: excesso procurado
		*	@return veradeiro caso a resposta seja encontrada no pai de v, e falso caso contrário.
		*/
		bool fwdVerifyParent(long long int &v, int &dr, int d);

		/*!
		*	@brief auxilia no processo de subida da árvore, verificando se o excesso procurado está nos intervalos do nó pai
		*	@param v: nó da rmM-tree investigado na passagem pela árvore
		*	@param dr: excesso relativo dr
		*	@param d: excesso procurado
		*	@return  verdadeiro caso a resposta seja encontrada no pai de v, e falso caso contrário.
		*/
		bool bwdVerifyParent(long long int &v, int &dr, int d);
};

#endif