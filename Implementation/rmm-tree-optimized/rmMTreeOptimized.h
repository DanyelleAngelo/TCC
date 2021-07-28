#ifndef RMMTREEO_H
#define RMMTREEO_H

#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <vector>

using namespace sdsl;
using std::vector;

typedef struct Key{
    int excess;
    int excessMax;
    int excessMin;
    int numberExcessMin;
}Key;

typedef struct Node{
    int nKeys;
    vector<Key> keys;
}Node;

class RMMTree{
    public:
        bit_vector bv;	// Vetor de bits que armazena a sequência de parênteses balanceados
		int size;							// Tamanho da sequência de parênteses balanceados.
		
		/*!
		*	@brief Construtor
		*/
        RMMTree(int_vector<1> &bv, int sizeBlock, int w, int order);

        //métodos públicos 

		unsigned long long cLog_m(unsigned long long  n,unsigned long long  m);
		unsigned long long fLog_m(unsigned long long  n,unsigned long long  m);
		/*!
		*	@brief ler os bits de s até e de bv, e contabiliza o inteiro correspondente a esta sequência binária
		*	@param s: ponto de partida da leitura
		*	@param e: ponto final da leitura
		*	@return inteiro correspondente aos bits lidos.
		*/
		int bitsread(int s,int e);
	
		/*!
		*	@brief verifica se a k-th folha está no último ou penúltimo nível da árvore e calcula sua posição.
		*	A ordem das folhas aqui vai de 0 à numberLeaves-1.
		*	@param k = k-th folha
		*	@return índice da k-th folha na rmM-tree
		*/
		int leafInTree(int k);

		/*!
		*	@brief Dado um índice na rmM-tree,verifica o nível em que a folha está, e calcula a sua ordem.
		*	A ordem das folhas vai de 0 à numberLeaves-1
		*	@param v : índice da folha na rmM-tree
		*	@return ordem da folha
		*/
		int numLeaf(int v);

		/*!
		*	@brief Dado a k-th folha da rmM-tree e um índice i em bp, calcula o ínicio do bloco da k-th folha
		*	e então computa a que chave i pertence.
		*	@param k : k-th folha da rmM-tree
		*	@param i: índice em bp
		*	@return chave em v onde i está localizado.
		*/
		int numKey(int k, int i);

		/*!
		*	@brief constrói a estrutura da rmM-tree, chamando primeiro a função para pré-computar
		*	uma tabela que irá acelerar o processo, e chamando depois a função para construir as folhas
		*	da rmM-tree e por último seus nós internos.
		*/
		void buildingTree();

		/*!
		*	@brief Realiza um percusor de subida e depois desciada na rmM-tree,  nos nós a direita
		* 	da folha (e na própria folha)  que contém a posição i+1 em busca do excesso d.
		*	@param i: Posição a partir da qual quero realizar a busca (será incrementado de 1)
		*	@param d: excesso de "1" buscado (profundidade)
		*	@return primeira posição j > i em bv onde ocorre o excesso d.
		*/
		int fwdSearch(int i,int d);

		/*!
		*	@brief Realiza um percusor de subida e depois desciada na rmM-tree através dos nos nós 
		* 	a esquerda da folha (e na própria folha)  que contém a posição i em busca do excesso d.
		*	@param i: Posição a partir da qual quero realizar a busca.
		*	@param d: excesso de "1" buscado (profundidade)
		*	@return primeira posição j < i em bv onde ocorre o excesso d.
		*/
		int bwdSearch(int i,int d);

		/*!
		*	@brief busca o parênteses de fechamento, corresponde ao parênteses  de abertura i.
		*	para isso, busca através da função fwdSearch() a primeira posição j >i onde ocorre
		*	o excesso -1.
		*	@param i: índice de um parênteses de abertura em bv, que cofica um nó
		*	@return índice j do parênteses de fechamento corresponde ao parênteses de abertura i, ou i, caso
		*	i codifique um parênteses de fechamento.
		*/
		int findClose(int i);

		/*!
		*	@brief busca o parênteses de abertura, corresponde ao parênteses  de fechamento i.
		*	para isso, busca através da função bwdSearch() a primeira posição j < i onde ocorre
		*	o excesso 0.
		*	@param i: índice de um parênteses de fechamento em bv
		*	@return índice j do parênteses de abertura corresponde ao parênteses de fechamento i, ou i, caso
		*	i codifique um parênteses de abertura.
		*/		
		int findOpen(int i);
		
		/*!
		*	@brief Imprime os valores da tabela de aceleração através da função printNode()
		*/
		void printTableC();

		/*!
		*	@brief Imprime a raíz, depois nós internos e por último folhas da rmM-tree através da 
		*	função printNode()
		*/
		void printTree();

		/*!
		*	@brief Imprime as informações básicas da rmM-tree, número de nós, número de folhas, altura
		*	e tamanho de cobertura de bloco.
		*/
		void printInfoTree();


    private:
		rank_support_v<1> b_rank1;			// Fornece suporte a operaçãop  rank, tendo como alvo bit 1
		rank_support_v<0> b_rank0;			// Fornece suporte a operaçãop  rank, tendo como alvo bit 0
		rank_support_v<10,2> b_rank10;		// Fornece suporte a operaçãop  rank, tendo como alvo a ocorrência do bit 1,seguido do bit 0
		select_support_mcl<1> b_sel1;		// Fornece suporte a operaçãop  select, tendo como alvo bit 1
		select_support_mcl<0> b_sel0;		// Fornece suporte a operaçãop  select, tendo como alvo bit 0
		select_support_mcl<10,2> b_sel10;	// Fornece suporte a operaçãop  select, tendo como alvo a ocorrência do bit 1,seguido do bit 0
		int sizeBlock;						// Tamanho do intervalocoberto por um nó folha
		int w;								// Divisor de sizeBlock. usado para pecorrer os bits de bv, de w em w, e assim acelerar o processo
		int numberLeaves;					// Quantidade de folhas na rmM-tree
		int numberNodes;					// Número de nós da rmM-tree
		int order;                          // Ordem da rmM-tree
        int height;							// Altura da rmM-tree
		vector<Node> tree;					// Vetor do tipo Node, usado para armazenar a Range-min-max tree
		vector<Key> tableC;				// Tabela de bits, com valores de excesso pré-computados,usados para acelar a construção da rmM-tree

        //métodos privados
		/*!
		*	@return retorna a se a < b e b se b <=a. 
		*/
		int min(int a , int b);

		/*!
		*	@param n: número a ser retirado
		*	@return chão do logarítmo na base 2 de n
		*/
		unsigned long long fLog_2(unsigned long long  n);

		/*!
		*	@param n: número a ser retirado
		*	@return teto do logarítmo na base 2 de n
		*/
		unsigned long long cLog_2(unsigned long long  n);

		/*!
		*	@brief Pré-computa uma tabela de excessos C, para agilizar a construção e as operações da RMM-tree.
		*/
		void buildingTableC();

		/*!
		*	@brief Constroí as folhas de cada nível da RmM-tree.
		*/
		void buildingLeaves();
		
		/*!
		*	@brief Constroí os nós internos e a raíz da RmM-tree
		*/
		void buildingInternalNodesRoot();

		/*!
		*	@brief Pecorre as chaves da folha v (a qual i pertence) mais a esquerda de i.
		*	e executa a varredura bloco a bloco procurando pelo excesso d.
		*	@param i: Posição a partir da qual devo buscar o excesso.
		*	@param nKeys: Número de chaves na folha v.
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		int fwdKey(int i,int key,int k,int nKeys,int d,int &dr);

		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo buscar o excesso (i é adicionado de 1)
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		int fwdBlock(int i,int d,int &dr);

		/*!
		*	@brief Calcula o pai do nó v, obtém o número de filhos deste nó, e a partir daí
		*	verifica se os irmãos mais à esquerda de v, compreendem o excesso d nos seus intervalos de
		*	máximo e mínimo.
		*	@param v: nó da rmM-tree investigado na passagem pela árvore
		*	@param dr: excesso relativo dr
		*	@param d: excesso procurado
		*	@return a chave que contém o excesso d desejado ou size caso a resposta não seja encontrada nos irmãos de v.
		*/
		int fwdVerifySibling(int &v,int &dr, int d);

		/*!
		*	@brief Pecorre as chaves da folha v (a qual i pertence) mais a direita de i.
		*	e executa a varredura bloco a bloco procurando pelo excesso d.
		*	@param i: Posição a partir da qual devo buscar o excesso.
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou -1 caso o excesso não se encontre neste bloco.
		*/
		int bwdKey(int i,int key,int k,int d,int &dr);

		/*!
		*	@brief Pecorre para trás cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo iniciar a busca para trás do excesso
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição (subtraída de 1) onde ocorre o excesso d ou ou -1 caso o excesso não se encontre neste bloco.
		*/
		int bwdBlock(int i,int d,int &dr);

		/*!
		*	@brief Calcula o pai do nó v, obtém o número de filhos deste nó, e a partir daí
		*	verifica se os irmãos mais à direita de v, compreendem o excesso d nos seus intervalos de
		*	máximo e mínimo.
		*	@param v: nó da rmM-tree investigado na passagem pela árvore
		*	@param dr: excesso relativo dr
		*	@param d: excesso procurado
		*	@return a chave que contém o excesso d desejado, e -1 caso a resposta não seja encontrada nos irmãos de v.
		*/
		int bwdVerifySibling(int &v,int &dr, int d);


		/*!
		*	@brief Imprime as informações de excesso de uma chave ou elemento de tabela
		*	@param vector: estrutura (árvore ou tabela) que terá seu nó impresso
		*	@param i: índice do elemento da estrutura.
		*/
		void printNode(vector<Key> vector, int i);
};

#endif