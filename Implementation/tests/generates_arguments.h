#ifndef ARGUMENTS_H
#define ARGUMENTS_H


static int sizeBlock=32;
static int w=16;

/*!
*   @brief : gera uma sequência de iterations índices correspondestes a parênteses de abertura
*/
void ArgumentsParOpen();

/*!
*   @brief : gera uma sequência de iterations índices correspondestes a  parênteses de fechamento
*/
void ArgumentsParClose();

/*!
*   @brief Gera uma sequência aleatória de índices de parenteses, baseados em uma semente específica
*/
void ArgumentsRand_I();

/*!
*   @brief : gera uma sequência de iterations índices correspondestes a parênteses de abertura
*/
void ArgumentsIsLeaf();

/*!
*   @brief : gera uma sequência de iterations*2 índices correspondestes a parênteses.
*   A função isAncestor precisa de dois parâmetros
*/
void ArgumentsIsAncestor();

/*!
*   @brief Gera uma sequência aleatória de índices de parenteses, baseados em uma semente específica
*/
void ArgumentsRand_II();

/*!
*   @brief Chama a função de inicialização do vetor de bits, com o conjunto de dados passados e as demais funções
*   para a criação dos argumetos usados nos testes.
*/
void Initialize(char *file);

#endif