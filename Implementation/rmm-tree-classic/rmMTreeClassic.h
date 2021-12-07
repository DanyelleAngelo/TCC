#ifndef RMMTREE_BIN_H
#define RMMTREE_BIN_H

#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include <vector>


using namespace sdsl;
using std::vector;

static const unsigned char BitReverseTable256_bin[] ={
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

typedef struct Node_bin{
	long int excess;
	long int excessMax;
	long int excessMin;
	long int numberExcessMin;
}Node_bin;

class RMMTree_Bin{
    public:
		bit_vector bv;	// Vetor de bits que armazena a sequência de parênteses balanceados
		long long int size;							// Tamanho da sequência de parênteses balanceados
		vector<Node_bin> tree;					// Vetor do tipo Node, usado para armazenar a Range-min-max tree

		/*!
		*	@brief Construtor
		*/
        RMMTree_Bin(int_vector<1> &bv, int sizeBlock, int w);
		
		//métodos públicos

		/*!
		*	@brief ler os bits de idx até idx+8, e contabiliza o inteiro correspondente a esta sequência binária
		*	@param idx: ponto de partida da leitura
		*	@return inteiro correspondente aos bits lidos.
		*/
		long long int bitsread(uint64_t idx);

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
		long long int getNumberLeaves();
		/*!
		*	@brief verifica se a k-th folha está no último ou penúltimo nível da árvore e calcula sua posição.
		*	A ordem das folhas aqui vai de 0 à numberLeaves-1.
		*	@param k = k-th folha
		*	@return índice da k-th folha na rmM-tree
		*/
		long long int leafInTree(long long int k);

		/*!
		*	@brief Dado um índice na rmM-tree,verifica o nível em que a folha está, e calcula a sua ordem.
		*	A ordem das folhas vai de 0 à numberLeaves-1
		*	@param v : índice da folha na rmM-tree
		*	@return ordem da folha
		*/
		long long int numLeaf(long long int v);

		/*!
		*	@brief constrói a estrutura da rmM-tree, chamando primeiro a função para pré-computar
		*	uma tabela que irá acelerar o processo, e chamando depois a função para construir as folhas
		*	da rmM-tree e por último seus nós internos.
		*/
		void buildingTree();

		/*!
		*	@brief Imprime a raíz, depois nós internos e por último folhas da rmM-tree através da 
		*	função printNode()
		*/
		void printTree();

		/*!
		*	@brief Imprime os valores da tabela de aceleração através da função printNode()
		*/
		void printTableC();

		/*!
		*	@brief Imprime as informações básicas da rmM-tree, número de nós, número de folhas, altura
		*	e tamanho de cobertura de bloco.
		*/
		void printInfoTree();

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

		/*!
		*	@brief Realiza um percurso de subida e descida na rmM-tree em busca do execesso mínimo
		*	presente no intervalo i,j. O percurso de subida termina ao encontrar o nó ancestral da
		*	folha que cobre a posição j.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return inteiro representando o excesso mínimo no intervalo i,j 
		*/
		long long int minExcess(long long int i,long long int j);

		/*!
		*	@brief Realiza um percurso de subida e descida na rmM-tree em busca do execesso máximo
		*	presente no intervalo i,j. O percurso de subida termina ao encontrar o nó ancestral da
		*	folha que cobre a posição j.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return inteiro representando o excesso máximo no intervalo i,j 
		*/
		long long int maxExcess(long long int i,long long int j);

		/*!
		*	@brief Chama minExcess() para contabilizar o excesso mínimo no intervalo i,j
		*	e depois realiza um percurso de subida e descida na árvore, afim de contabilizar o
		*	número de vezes que o excesso aparece na área
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return número de vezes que o excesso mínimo aparece em bv[i,j]
		*/
		long long int minCount(long long int i,long long int j);

		/*!
		*	@brief Chama minExcess() para contabilizar o excesso mínimo no intervalo i,j
		*	e depois realiza um percurso de subida e descida na árvore, afim de obter a posição exata
		*	em que o t-th execesso mínimo ocorre.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@param t: ordem em que o excesso mínimo deve ocorrer
		*	@return índice, p, com i <= p <= j, onde ocorre o t-th excesso mínimo 
		*/
		long long int minSelectExcess(long long int i,long long int j, long long int t);

		/*!
		*	@brief Chama minExcess() para contabilizar o excesso mínimo no intervalo i,j
		*	e depois chama a função fwdSearch(), passando como parâmetro o excesso mínimo computado,
		*	e a posição i-1, a fim de encontar a posição exata em que esse excesso ocorre.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return índice, p, com i <= p <= j, onde ocorre pela primeira vez o excesso mínimo
		*/
		long long int rmq(long long int i,long long int j);

		/*!
		*	@brief Chama maxExcess() para contabilizar o excesso máximo no intervalo i,j
		*	e depois chama a função fwdSearch(), passando como parâmetro o excesso máximo computado,
		*	e a posição i-1, a fim de encontar a posição exata em que esse excesso ocorre.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return índice, p, com i <= p <= j, onde ocorre pela primeira vez o excesso máximo
		*/
		long long int rMq(long long int i,long long int j);

		/*!
		*	@brief busca o parênteses de fechamento, corresponde ao parênteses  de abertura i.
		*	para isso, busca através da função fwdSearch() a primeira posição j >i onde ocorre
		*	o excesso -1.
		*	@param i: índice de um parênteses de abertura em bv, que cofica um nó
		*	@return índice j do parênteses de fechamento corresponde ao parênteses de abertura i, ou i, caso
		*	i codifique um parênteses de fechamento.
		*/
		long long int findClose(long long int i);

		/*!
		*	@brief busca o parênteses de abertura, corresponde ao parênteses  de fechamento i.
		*	para isso, busca através da função bwdSearch() a primeira posição j < i onde ocorre
		*	o excesso 0.
		*	@param i: índice de um parênteses de fechamento em bv
		*	@return índice j do parênteses de abertura corresponde ao parênteses de fechamento i, ou i, caso
		*	i codifique um parênteses de abertura.
		*/		
		long long int findOpen(long long int i);

		/*!
		*	@brief busca a posição j < i , tal que BV[j,findclose(j)] contenha a posição i.
		*	@param i: índice de um parênteses de abertura em bv, que cofica um nó
		*	@return bv.size() caso i=0, findOpen(i) caso bv[i]=1, ou nó pai de i (computado através da função bwdSearch(i,-2)+1)
		*/
		long long int enclose(long long int i);

		/*!
		*	@brief  verifica se bv[x] é um bit 1 e se o elemento que o sucede é um bit 0 para decidir se este é um nó folha.
		*	@param x: índice do parêntes que codifica o nó analisado.
		*	@return: true se x codificar um nó folha, false caso contrário.
		*/
		bool isLeaf(long long int x);

		/*!
		*	@brief  verifica se o nó x é ancestral do nó y, checando se o primeiro contém o segundo.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@param y: índice do vetor de parêntese balanceados que codifica o nó
		*	@return: true se x é ancestral de y, false caso contrário.
		*/
		bool isAncestor(long long int x, long long int y);

		/*!
		*	@brief  contabiliza o excesso de 1 no intervalo bv[0,x] para saber a profundidade do nó x.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: profundidade do nó x
		*/
		long long int depth(long long int x);

		/*!
		*	@brief  Busca o nó que contém o x, mais à esquerda de x.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: índice do nó pai de x
		*/
		long long int parent(long long int x);

		/*!
		*	@brief  busca o irmão à direita de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: retorna o índice do irmão à direita de x
		*/
		long long int nextSibling(long long int x);

		/*!
		*	@brief  busca o irmão à esquerda de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: retorna o índice do irmão à esquerda de x
		*/
		long long int prevSibling(long long int x);

		/*!
		* 	@brief usa minSelect para computar o t-th filho do nó x (se houver)
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@param t: ordem do filho de x a ser buscado
		*	@return : índice do parênteses de abertura que codifica o t-th filho de x, se houver, e  bv,size() caso contrário
		*/
		long long int  child(long long int x,long long int t);

		/*!
		*	@brief  verifica se o elemento anterior a close de i, é um nó (se i não é folha)
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: retorna o índice do i-th filho de x, se houver, e  bv,size() caso contrário
		*/
		long long int lastChild(long long int x);

		/*!
		*	@brief verifica se o nó é uma folha, se não for retorna o primeiro elemento contido por ele
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return: Primeiro filho do nó x, se houver, e  bv,size() caso contrário
		*/
		long long int firstChild(long long int x);

		/*! 
		*	@brief contabiliza o número de irmãos a esquerda do nó x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x 
		*	@return a quantidade de nós a esquerda de x
		*/
		long long int childRank(long long int x);

		/*!
		*	@brief contabiliza o número de bits 1 no intervalo b[x,close(x)]
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return tamanho da subárvore enraízada em x
		*/
		long long int subtreeSize(long long int x);

		/*!
		*	@brief  busca o ancestral de x que está d níveis acima dele
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		* 	@param d: quantidade de níveis acima de x onde teremos a resposta
		*	@return: índice j do ancestral de x tal que depth(j)=depth(x)-d
		*/
		long long int levelAncestor(long long int x,int d);

		/*!
		*	@brief  busca o ancestral comum (mais próximo) de x e y
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		* 	@param y: parênteses de abertura no vetor de parênteses balanceados que codifica o nó y
		*	@return: índice do ancestral comum de x e y.
		*/
		long long int lca(long long int x,long long int y);

		/*!
		*	@brief usa fwdSearch para encontrar o próximo nó a direita de x (não necessariamente o irmão de x,
		*	a ideia é pegar o próximo elemento do nível)
		* 	com a mesma profundidade de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : índice do primeiro nó a direita de x, com a mesma profundidade de x; ou size se a resposta não for encotrada
		*/
		long long int levelNext(long long int x);

		/*!
		*	@brief usa bwdSearch para encontrar o elemnto a esquerda de x (não necessariamente o irmão de x,
		*	a ideia é pegar o  elemento anterior do memso nível nível)
		* 	com a mesma profundidade de x
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : índice do primeiro nó a esquerda de x, com a mesma profundidade de x; ou menos 1 se a resposta não for encontrada.
		*/
		long long int levelPrev(long long int x);

		/*!
		* 	@brief usa fwdSearch para encontrar o nó mais a esquerda com profundidade d
		*	@param d: profundidade desejada
		*	@return índice do parênteses de abertura do nó mais a esquerda da árvore com profundidade d
		*/
		long long int levelLeftMost(int d);

		/*!
		* 	@brief usa bwdSearch (a partir de bv.size()) para encontrar o nó mais a direita com profundidade d
		*	@param d: profundidade desejada
		*	@return índice do parênteses de abertura do nó mais a direita da árvore com profundidade d
		*/
		long long int levelRightMost(int d);

		/*!
		* 	@brief usa rMq para buscar o filho de x que tenha a maior profundidade (mais a esquerda). 
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : índice do filho de x com maior profundidade
		*/
		long long int  deepestNode(long long int x);

		/*!
		* 	@brief usa minCount para computar o número de vezes que o excesso mínimo (ocorre quando x engloba um filho)
		*	aparece em x. 
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return : quantidade de filhos de x
		*/
		long long int  degree(long long int x);

		/*!
		*	@brief contabiliza a quantida de ocorrências do bit 1 seguido do bit 0, no intervalo B[0,x].
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return: quantidade de folhas a esquerda de x
		*/
		long long int leafRank(long long int x);

		/*!
		*	@brief busca o índice da t-th ocorrência do par de bits 10 (ou seja da t-th folha).
		*	@param t: ordem da folha buscada
		*	@return: índice da t-th folha.
		*/
		long long int leafSelect(long long int t);

		/*!
		* 	@brief usa leafSelect para encontrar a folha mais a esquerda de x.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return índice da folha mais à esquerda de x. Se x for uma folha retorna seu irmão mais a esquerda,
		*	se ele não existir, retorna o próprio x. Se x for um nṍ pai, retorna o seu filho com maior profundidade,mais a esqueda.
		*/
		long long int leftMostLeaf(long long int x);

		/*!
		* 	@brief usa leafSelect para encontrar a folha mais a direita de x.
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return índice da folha mais à direita de x. Se x for uma folha retorna seu irmão mais a direita,
		*	se ele não existir, retorna o próprio x. Se x for um nṍ pai, retorna o seu filho com maior profundidade,mais a direita.
		*/
		long long int rightMostLeaf(long long int x);

		/*!
		*	@brief visita x usando um percurso pré-ordem
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return rank1 no intervalo B[0,x]
		*/
		long long int preRank(long long int x);

		/*!
		*	@brief calcula o rank de x, a partir de um percurso pos-order
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return rank0 no intervalo B[0,close(x)]
		*/
		long long int postRank(long long int x);

		/*!
		*	@brief calcula a posição do t-th parênteses de abertura
		*	@param x: parênteses de abertura no vetor de parênteses balanceados que codifica o nó x
		*	@return t-th parênteses de abertura
		*/
		long long int preSelect(long long int t);

		long long int postSelect(long long int t);

	private:
		rank_support_v<1> b_rank1;			// Fornece suporte a operaçãop  rank, tendo como alvo bit 1
		rank_support_v<0> b_rank0;			// Fornece suporte a operaçãop  rank, tendo como alvo bit 0
		rank_support_v<10,2> b_rank10;		// Fornece suporte a operaçãop  rank, tendo como alvo a ocorrência do bit 1,seguido do bit 0
		select_support_mcl<1> b_sel1;		// Fornece suporte a operaçãop  select, tendo como alvo bit 1
		select_support_mcl<0> b_sel0;		// Fornece suporte a operaçãop  select, tendo como alvo bit 0
		select_support_mcl<10,2> b_sel10;	// Fornece suporte a operaçãop  select, tendo como alvo a ocorrência do bit 1,seguido do bit 0
		int sizeBlock;						// Tamanho do intervalocoberto por um nó folha
		int w;								// Divisor de sizeBlock. usado para pecorrer os bits de bv, de w em w, e assim acelerar o processo.
		long int numberLeaves;					// Quantidade de folhas na rmM-tree
		long int numberNodes;					// Número de nós da rmM-tree
		int height;							// Altura da rmM-tree
		vector<Node_bin> tableC;				// Tabela de bits, com valores de excesso pré-computados,usados para acelar a construção da rmM-tree
		
		//métodos privados
		/*!
		*	@brief reverte os bits lidos por bitsread
		*/
		uint16_t reverse_16(uint16_t x);

		/*!
		*	@return retorna a se a < b e b se b <=a. 
		*/
		long long int min(long long int a , long long int b);

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
		*	@brief Imprime as informações de excesso de um nó
		*	@param vector: estrutura (árvore ou tabela) que terá seu nó impresso
		*	@param i: índice do elemento da estrutura.
		*/
		void printNode(vector<Node_bin> vector, long long int i);
		
		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo buscar o excesso (i é adicionado de 1)
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		long long int fwdBlock(long long int i,int d,int &dr);

		/*!
		*	@brief Pecorre para trás cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo iniciar a busca para trás do excesso
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		long long int bwdBlock(long long int i,int d, int &dr);

		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i", em busca do menor excesso na área.
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca. Varremos até "j" ou até chegarmos ao limite do bloco de i, o que vier primeiro.
		*	@param d: Excesso relativo.
		*	@return o excesso mínimo no intervalo definido.
		*/
		long long int minBlock(long long int i,long long int j,int &d);

		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i", em busca do maior excesso na área.
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca. Varremos até "j" ou até chegarmos ao limite do bloco de i, o que vier primeiro.
		*	@param d: Excesso relativo.
		*	@return o excesso máximo no intervalo definido.
		*/
		long long int maxBlock(long long int i,long long int j,int &d);

		/*!
		*	@brief Contabiliza a quantidade de vezes que o excesso mínimo ocorre no intervalo i e j
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca (limite superior da folha de i).
		*	@param m : excesso mínimo a ser computado.
		*	@param d : excesso relativo (contabilizado a cada bit varrido), parâmetro passado por referência
		*	@return o número de vezes que o excesso mínimo aparece no intervalo definido.
		*/
		long long int minCountBlock(long long int i,long long int j,long long int m,int &d);

		/*!
		*	@brief Procura a posição em que o excesso mínimo ocorre pela t-th no intervalo i e j
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca (limite superior da folha de i).
		*	@param m : excesso mínimo a ser computado.
		*	@param t: t-th excesso mínimo que quero encontrar
		*	@param d : excesso relativo (contabilizado a cada bit varrido), parâmetro passado por referência
		*	@return a posição p em que o corre o t-th excesso mínimo m.
		*/
		long long int minSelectBlock(long long int i,long long int j,long long int m,long long int &t, int &d);

};

#endif
