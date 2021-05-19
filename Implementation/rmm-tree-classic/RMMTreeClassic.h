#ifndef RMMTREEC_H
#define RMMTREEC_H

#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>

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
		int numberLeaves;
		int numberNodes;
		int height;
		vector<Node> tree;
		vector<Node> tableC;

        RMMTree();

		int bitsread(int s,int e);
		void buildingTree();
		void buildingTableC();
		void buildingLeaves(int s, int e, int k);
		void buildingInternalNodesRoot();
		void printNode(vector<Node> v, int i);
		void printTree();
		void printTableC();
		void printInfoTree();
};

#endif