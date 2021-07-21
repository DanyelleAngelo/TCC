#ifndef READ_BP
#define READ_BP

#include <sdsl/int_vector.hpp>

using namespace sdsl;

/*!
*   @brief Realiza a abertura e leitura de um arquivo, e converte a sequência de parênteses balanceados contidos nele em uma sequência de bits.
*   @param fn: nome do arquivo
*   @param v: vetor onde armazenaremos a sequência de bits.
*/
void parentheses_to_bits(const char* fn, int_vector<1> &v);

#endif