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
		bit_vector bv;	// Vetor de bits que armazena a sequência de parênteses balanceados

		/*!
		*	@brief Construtor
		*/
        RMMTree(int_vector<1> &bv, int sizeBlock, int w);
		
		//métodos públicos

		uint64_t getInt(const uint8_t  idx, const uint8_t len);
		uint64_t readInt(const uint64_t* word, uint8_t offset, const uint8_t len);

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
		*	@brief Realiza um percurso de subida e descida na rmM-tree em busca do execesso mínimo
		*	presente no intervalo i,j. O percurso de subida termina ao encontrar o nó ancestral da
		*	folha que cobre a posição j.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return inteiro representando o excesso mínimo no intervalo i,j 
		*/
		int minExcess(int i,int j);

		/*!
		*	@brief Realiza um percurso de subida e descida na rmM-tree em busca do execesso máximo
		*	presente no intervalo i,j. O percurso de subida termina ao encontrar o nó ancestral da
		*	folha que cobre a posição j.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return inteiro representando o excesso máximo no intervalo i,j 
		*/
		int maxExcess(int i,int j);

		/*!
		*	@brief Chama minExcess() para contabilizar o excesso mínimo no intervalo i,j
		*	e depois realiza um percurso de subida e descida na árvore, afim de contabilizar o
		*	número de vezes que o excesso aparece na área
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return número de vezes que o excesso mínimo aparece em bv[i,j]
		*/
		int minCount(int i,int j);

		/*!
		*	@brief Chama minExcess() para contabilizar o excesso mínimo no intervalo i,j
		*	e depois realiza um percurso de subida e descida na árvore, afim de obter a posição exata
		*	em que o t-th execesso mínimo ocorre.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@param t: ordem em que o excesso mínimo deve ocorrer
		*	@return índice, p, com i <= p <= j, onde ocorre o t-th excesso mínimo 
		*/
		int minSelectExcess(int i,int j, int t);

		/*!
		*	@brief Chama minExcess() para contabilizar o excesso mínimo no intervalo i,j
		*	e depois chama a função fwdSearch(), passando como parâmetro o excesso mínimo computado,
		*	e a posição i-1, a fim de encontar a posição exata em que esse excesso ocorre.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return índice, p, com i <= p <= j, onde ocorre pela primeira vez o excesso mínimo
		*/
		int rmq(int i,int j);

		/*!
		*	@brief Chama maxExcess() para contabilizar o excesso máximo no intervalo i,j
		*	e depois chama a função fwdSearch(), passando como parâmetro o excesso máximo computado,
		*	e a posição i-1, a fim de encontar a posição exata em que esse excesso ocorre.
		*	@param i: índice a partir do qual deve-se realizar a busca
		*	@param j: índice onde devemos terminar a busca
		*	@return índice, p, com i <= p <= j, onde ocorre pela primeira vez o excesso máximo
		*/
		int rMq(int i,int j);

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
		*	@brief busca a posição j < i , tal que BV[j,findclose(j)] contenha a posição i.
		*	@param i: índice de um parênteses de abertura em bv, que cofica um nó
		*	@return bv.size() caso i=0, findOpen(i) caso bv[i]=1, ou nó pai de i (computado através da função bwdSearch(i,-2)+1)
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
		rank_support_v<1> b_rank1;			// Fornece suporte a operaçãop  rank, tendo como alvo bit 1
		rank_support_v<0> b_rank0;			// Fornece suporte a operaçãop  rank, tendo como alvo bit 0
		rank_support_v<10,2> b_rank10;		// Fornece suporte a operaçãop  rank, tendo como alvo a ocorrência do bit 1,seguido do bit 0
		select_support_mcl<1> b_sel1;		// Fornece suporte a operaçãop  select, tendo como alvo bit 1
		select_support_mcl<0> b_sel0;		// Fornece suporte a operaçãop  select, tendo como alvo bit 0
		select_support_mcl<10,2> b_sel10;	// Fornece suporte a operaçãop  select, tendo como alvo a ocorrência do bit 1,seguido do bit 0
		int sizeBlock;						// Tamanho do intervalocoberto por um nó folha
		int w;								// Divisor de sizeBlock. usado para pecorrer os bits de bv, de w em w, e assim acelerar o processo
		int size;							// Tamanho da sequência de parênteses balanceados
		int numberLeaves;					// Quantidade de folhas na rmM-tree
		int numberNodes;					// Número de nós da rmM-tree
		int height;							// Altura da rmM-tree
		vector<Node> tree;					// Vetor do tipo Node, usado para armazenar a Range-min-max tree
		vector<Node> tableC;				// Tabela de bits, com valores de excesso pré-computados,usados para acelar a construção da rmM-tree
		
		/*!
		*	@return retorna a se a < b e b se b <=a. 
		*/
		int min(int a , int b);

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
		void printNode(vector<Node> vector, int i);
		
		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo buscar o excesso (i é adicionado de 1)
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		int fwdBlock(int i,int d,int &dr);

		/*!
		*	@brief Pecorre para trás cada subbloco de tamanho "w" do bloco pertencente à "i".
		*	@param i: Posição a partir da qual devo iniciar a busca para trás do excesso
		*	@param d: Excesso buscado
		*	@param dr: Excesso relativo (atualizado a cada posição que avançamos no bloco)
		*	@return a posição em que ocorre o excesso d ou ou bv.size() caso o excesso não se encontre neste bloco.
		*/
		int bwdBlock(int i,int d,int &dr);

		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i", em busca do menor excesso na área.
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca. Varremos até "j" ou até chegarmos ao limite do bloco de i, o que vier primeiro.
		*	@param d: Excesso relativo.
		*	@return o excesso mínimo no intervalo definido.
		*/
		int minBlock(int i,int j,int &d);

		/*!
		*	@brief Pecorre para frente cada subbloco de tamanho "w" do bloco pertencente à "i", em busca do maior excesso na área.
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca. Varremos até "j" ou até chegarmos ao limite do bloco de i, o que vier primeiro.
		*	@param d: Excesso relativo.
		*	@return o excesso máximo no intervalo definido.
		*/
		int maxBlock(int i,int j,int &d);

		/*!
		*	@brief Contabiliza a quantidade de vezes que o excesso mínimo ocorre no intervalo i e j
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca (limite superior da folha de i).
		*	@param m : excesso mínimo a ser computado.
		*	@param d : excesso relativo (contabilizado a cada bit varrido), parâmetro passado por referência
		*	@return o número de vezes que o excesso mínimo aparece no intervalo definido.
		*/
		int minCountBlock(int i,int j,int m,int &d);

		/*!
		*	@brief Procura a posição em que o excesso mínimo ocorre pela t-th no intervalo i e j
		*	@param i: Posição a partir da qual devo iniciar a busca.
		*	@param j: Intervalo superior da busca (limite superior da folha de i).
		*	@param m : excesso mínimo a ser computado.
		*	@param t: t-th excesso mínimo que quero encontrar
		*	@param d : excesso relativo (contabilizado a cada bit varrido), parâmetro passado por referência
		*	@return a posição p em que o corre o t-th excesso mínimo m.
		*/
		int minSelectBlock(int i,int j,int m,int &t,int &d);

};

#endif