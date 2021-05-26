#ifndef RMMTREEC_H
#define RMMTREEC_H

#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <vector>

using namespace sdsl;
using std::vector;

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

        RMMTree(int_vector<1> &bv, int sizeBlock);
		//~RMMTree();
		int leafInTree(int k);
		int numLeaf(int v);
		void buildingTree();
		void printNode(vector<Node> v, int i);
		void printTree();
		void printTableC();
		void printInfoTree();
		int fwdSearch(int i,int d);
		
	private:
		int numberLeaves;
		int numberNodes;
		int height;
		vector<Node> tree;
		vector<Node> tableC;

		int bitsread(int s,int e);

		/*
			@breif: Pecorre cada subbloco de tamanho "w" do bloco pertencente à "i" em busca do excesso d
			@param i: Posição a partir da qual devo buscar o excesso
			@param d: Excesso desejado
			@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
			@return: a posição em que ocorre o excesso ou -1 caso o excesso não se encontre neste bloco
		*/
		int fwdBlock(int i,int d,int *dr);
		void buildingTableC();
		void buildingLeaves(int s, int e, int k);
		void buildingInternalNodesRoot();
};

#endif