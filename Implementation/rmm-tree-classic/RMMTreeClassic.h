#ifndef RMMTREEC_H
#define RMMTREEC_H

#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <vector>

using namespace sdsl;
using std::vector;


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

        RMMTree(int_vector<1> &bv, int sizeBlock, int w);
		//~RMMTree();
		uint64_t getInt(const uint8_t  idx, const uint8_t len);
		uint64_t readInt(const uint64_t* word, uint8_t offset, const uint8_t len);
		int bitsread(int s,int e);
		int leafInTree(int k);
		int numLeaf(int v);
		void buildingTree();
		void printTree();
		void printTableC();
		void printInfoTree();
		int fwdSearch(int i,int d);
		int bwdSearch(int i,int d);
		int minExcess(int i,int j);
		int rmq(int i,int j);
		int findclose(int i);
		int findopen(int i);

		/*!
		*	@brief 
		*/
		int enclose(int i);
		
	private:
		int sizeBlock;
		int numberLeaves;
		int numberNodes;
		int height;
		int w;
		vector<Node> tree;
		vector<Node> tableC;
		
		/*!
		*	@return retorna a se a < b e b se b <=a. 
		*/
		int min(int a , int b);

		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo buscar o excesso (i é adicionado de 1)
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		int fwdBlock(int i,int d,int *dr);

		/*!
		*	@brief Pecorre para trás cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo iniciar a busca para trás do excesso
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		int bwdBlock(int i,int d,int *dr);

		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i", em busca do menor excesso na área.
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca. Varremos até "j" ou até chegarmos ao limite do bloco de i, o que vier primeiro.
		*	@param d: Excesso relativo.
		*	@return o excesso mínimo no intervalo definido.
		*/
		int minBlock(int i,int j,int *d);

		/*!
		*	@brief Pré-computa uma tabela de excessos C, para agilizar a construção e as operações da RMM-tree.
		*/
		void buildingTableC();

		/*!
		*	@brief Constroí as folhas de cada nível da RmM-tree.
		*	@param s: Posição onde começa a primeira folha do nível
		*	@param e: Posição da última folha do nível.
		*	@param k: Marca a posição ordinal das folhas na árvore.
		*/
		void buildingLeaves(int s, int e, int k);

		/*!
		*	@brief Constroí os nós internos e a raíz da RmM-tree
		*/
		void buildingInternalNodesRoot();

		/*!
		*	@brief Imprime as informações de excesso de um nó
		*	@param vector: estrutura (árvore ou tabela) que terá seu nó impresso
		*	@param i: índice do elemento da estrutura.
		*/
		void printNode(vector<Node> vector, int i);
};

#endif