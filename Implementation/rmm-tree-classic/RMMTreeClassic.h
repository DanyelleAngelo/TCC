#ifndef RMMTREEC_H
#define RMMTREEC_H

#include <sdsl/int_vector.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/bp_support_sada.hpp>
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
        RMMTree(int_vector<1> &bv, int sizeBlock, int w);
		//~RMMTree();
		uint64_t getInt(const uint8_t  idx, const uint8_t len);
		uint64_t readInt(const uint64_t* word, uint8_t offset, const uint8_t len);
		unsigned long long fLog_2(unsigned long long  n);
		unsigned long long cLog_2(unsigned long long  n);
		int bitsread(int s,int e);
		
		/*!
		*	@brief verifica se a k-th folha está no último ou penúltimo nível da árvore e calcula sua posição
		*	@param k = k-th folha
		*	@return índice da k-th folha
		*/
		int leafInTree(int k);

		/*!
		*	@brief verifica o nível em que a folha está, e calcula a sua ordem
		*	@param v : índice da folha na ávore
		*	@return ordem da folha
		*/
		int numLeaf(int v);
		void buildingTree();
		void printTree();
		void printTableC();
		void printInfoTree();

		int fwdSearch(int i,int d);
		int bwdSearch(int i,int d);
		int minExcess(int i,int j);
		int maxExcess(int i,int j);
		int minCount(int i,int j);
		int minSelectExcess(int i,int j, int t);

		int rmq(int i,int j);
		int rMq(int i,int j);
		int findClose(int i);
		int findOpen(int i);

		/*!
		*	@brief 
		*/
		int enclose(int i);

		/*!
		*	@brief  verifica se bv[x] é um bit 1 e se o elemento que o sucede é um bit 0 para decidir se este é um nó folha.
		*	@param x: índice do parêntes que codifica o nó analisado.
		*	@return: true se x codificar um nó folha, false caso contrário.
		*/
		bool isLeaf(int x);

		/*!
		*	@brief  verifica se o nó x é ancestral do nó y, checando se o primeiro contém o segundo.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@param y: índice do vetor de parêntese balanceados que codifica o nó
		*	@return: true se x é ancestral de y, false caso contrário.
		*/
		bool isAncestor(int x, int y);

		/*!
		*	@brief  contabiliza o excesso de 1 no intervalo bv[0,x] para saber a profundidade do nó.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: profundidade do nó x
		*/
		int depth(int x);

		/*!
		*	@brief  Busca o nó que contém o x, mais à esquerda de x.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: índice do nó pai de x
		*/
		int parent(int x);

		/*!
		*	@brief  busca o irmão à direita de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: retorna o índice do irmão à direita de x
		*/
		int nextSibling(int x);


		/*!
		*	@brief  busca o irmão à esquerda de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: retorna o índice do irmão à esquerda de x
		*/
		int prevSibling(int x);

		/*!
		* 	@brief: usa minSelect para computar o t-th filho do nó x (se houver)
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@param t: ordem do filho de x a ser buscado
		*	@return : índice do parênteses de abertura que codifica o t-th filho de x, se houver, e  bv,size() caso contrário
		*/
		int  child(int x,int t);

		/*!
		*	@brief  verifica se o elemento anterior a close de i, é um nó (se i não é folha)
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: retorna o índice do i-th filho de x, se houver, e  bv,size() caso contrário
		*/
		int lastChild(int x);

		/*!
		*	@brief chama função child, passando 1 como t para encontrar o primeiro filho de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: Primeiro filho do nó x, se houver, e  bv,size() caso contrário
		*/
		int firstChild(int x);

		/*!
		*	@brief  Calcula a quantidade de nós contidos em x (incluindo x)
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return tamanho da subárvore enraízada em x
		*/

		/*! 
		*	@brief contabiliza o número de irmãos a esquerda do nó x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return a quantidade de nós a esquerda de x
		*/
		int childRank(int x);

		/*!
		*	@brief contabiliza o número de bits 1 no intervalo b[x,close(x)]
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return tamanho da subárvore enraízada em x
		*/
		int subtreeSize(int x);

		/*!
		*	@brief  busca o ancestral de x que está d níveis acima dele
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		* 	@param d: quantidade de níveis acima de x onde teremos a resposta
		*	@return: índice j do ancestral de x tal que depth(j)=depth(x)-d
		*/
		int levelAncestor(int x,int d);

		/*!
		*	@brief  busca o ancestral comum (mais próximo) de x e y
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		* 	@param y: parênteses de abertura no vetor de parênteses balanceados que codifica o nó y
		*	@return: índice do ancestral comum de x e y.
		*/
		int lca(int x,int d);

		/*!
		*	@brief usa fwdSearch para encontrar o próximo nó a direita de x (não necessariamente o irmão de x,
		*	a ideia é pegar o próximo elemento do nível)
		* 	com a mesma profundidade de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : índice do primeiro nó a direita de x, com a mesma profundidade de x
		*/
		int levelNext(int x);

		/*!
		*	@brief usa bwdSearch para encontrar o elemnto a esquerda de x (não necessariamente o irmão de x,
		*	a ideia é pegar o  elemento anterior do memso nível nível)
		* 	com a mesma profundidade de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : índice do primeiro nó a esquerda de x, com a mesma profundidade de x
		*/
		int levelPrev(int x);

		/*!
		* 	@brief: usa fwdSearch para encontrar o nó mais a esquerda com profundidade d
		*	@param d: profundidade desejada
		*	@return índice do parênteses de abertura do nó mais a esquerda da árvore com profundidade d
		*/
		int levelLeftMost(int d);

		/*!
		* 	@brief: usa bwdSearch (a partir de bv.size()) para encontrar o nó mais a direita com profundidade d
		*	@param d: profundidade desejada
		*	@return índice do parênteses de abertura do nó mais a direita da árvore com profundidade d
		*/
		int levelRightMost(int d);

		/*!
		* 	@brief: usa rMq para buscar o filho de x que tenha a maior profundidade (mais a esquerda). 
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : índice do filho de x com maior profundidade
		*/
		int  deepestNode(int x);

		/*!
		* 	@brief: usa minCount para computar o número de vezes que o excesso mínimo (ocorre quando x engloba um filho)
		*	aparece em x. 
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : quantidade de filhos de x
		*/
		int  degree(int x);

		/*!
		*	@brief: contabiliza a quantida de ocorrências do bit 1 seguido do bit 0, no intervalo B[0,x].
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return: quantidade de folhas a esquerda de x
		*/
		int leafRank(int x);

		/*!
		*	@brief: busca o índice da t-th ocorrência do par de bits 10.
		*	@param t: ordem da folha buscada
		*	@return: índice da t-th folha.
		*/
		int leafSelect(int t);

		/*!
		* 	@brief: usa leafSelect para encontrar a folha mais a esquerda de x.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return índice da folha mais à esquerda de x. Se x for uma folha retorna seu irmão mais a esquerda,
		*	se ele não existir, retorna o próprio x. Se x for um nṍ pai, retorna o seu filho com maior profundidade,mais a esqueda.
		*/
		int leftMostLeaf(int x);


		/*!
		* 	@brief: usa leafSelect para encontrar a folha mais a direita de x.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return índice da folha mais à direita de x. Se x for uma folha retorna seu irmão mais a direita,
		*	se ele não existir, retorna o próprio x. Se x for um nṍ pai, retorna o seu filho com maior profundidade,mais a direita.
		*/
		int rightMostLeaf(int x);

		/*!
		*	@brief: visita x usando um percurso pré-ordem
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return rank1 no intervalo B[0,x]
		*/
		int preRank(int x);

		/*!
		*	@brief: calcula o rank de x, a partir de um percurso pos-order
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return rank0 no intervalo B[0,close(x)]
		*/
		int postRank(int x);

		/*!
		*	@brief: calcula a posição do t-th parênteses de abertura
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return t-th parênteses de abertura
		*/
		int preSelect(int t);

		int postSelect(int t);

	private:
		rank_support_v<1> b_rank1;
		rank_support_v<0> b_rank0;
		rank_support_v<10,2> b_rank10;
		select_support_mcl<1> b_sel1;
		select_support_mcl<0> b_sel0;
		select_support_mcl<10,2> b_sel10;
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
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i", em busca do maior excesso na área.
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca. Varremos até "j" ou até chegarmos ao limite do bloco de i, o que vier primeiro.
		*	@param d: Excesso relativo.
		*	@return o excesso máximo no intervalo definido.
		*/
		int maxBlock(int i,int j,int *d);

		/*!
		*	@brief Contabiliza a quantidade de vezes que o excesso mínimo ocorre no intervalo i e j
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca (limite superior da folha de i).
		*	@param m : excesso mínimo a ser computado.
		*	@param d : excesso relativo (contabilizado a cada bit varrido), parâmetro passado por referência
		*	@return o número de vezes que o excesso mínimo aparece no intervalo definido.
		*/
		int minCountBlock(int i,int j,int m,int *d);

		/*!
		*	@brief Procura a posição em que o excesso mínimo ocorre pela t-th no intervalo i e j
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca (limite superior da folha de i).
		*	@param m : excesso mínimo a ser computado.
		*	@param t: t-th excesso mínimo que quero encontrar
		*	@param d : excesso relativo (contabilizado a cada bit varrido), parâmetro passado por referência
		*	@return a posição p em que o corre o t-th excesso mínimo m.
		*/
		int minSelectBlock(int i,int j,int m,int *t,int *d);

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