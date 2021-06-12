#include "gtest/gtest.h"

#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../RMMTreeClassic.h"
#include <sdsl/bits.hpp>
/*
*DISABLED_ desabilitar um teste inserindo esse prefixo em seu nome
*testing::GTEST_FLAG(filter) = "-MyLibrary.TestWriting"; usar o nome do teste precedido de "-" desabilita ele no main,
a ausência desse mesmo elemento "-", torna o teste o único a ser executado.
*/


/**TODO: adicionar leitura de arquivos. Aumentar nossa capacidade de testes
*/
using namespace sdsl;
using namespace std;


class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree *t;
        int sizeBlock=4;
        int w=2;
        bp_support_sada<> *bps;
        int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

        void SetUp(){
            t = new RMMTree(v,sizeBlock,w);
            bps = new bp_support_sada<>(&(t->bv));
            t->buildingTree();
        }
        void TearDown(){
            delete t;
        }
};

TEST_F(RMMTreeFixtureTest, GET_INT){
    int index[] = {9,17,30,36};
    int len = 2;

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(bits::read_int(t->bv.data() + (index[i]>>6), index[i] & 0x3F, len),t->bitsread(index[i],index[i]+len-1))<< "Ocorreu um erro ao tentar devolver o interiro correspondente aos bits [" << index[i] << ","<<(index[i]+len-1)<<"]";
    }
}

TEST_F(RMMTreeFixtureTest, leaf_index_on_tree){
    /*Dado um índice i no vetor de parênteses, retorna a posição de seu nó na RMM-Tree*/
    
    int index[] = {2,7,10,14,19,23,26,30,35,37};
    int expected[] = {15,16,17,18,9,10,11,12,13,14};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        int k = floor((double)(index[i])/sizeBlock);
        EXPECT_EQ(t->leafInTree(k),expected[i]) << "Ocorreu um erro ao tentar obterr o índice da RMMTree em que se encontra a " << index[i] <<"-th folha";
    }
}

TEST_F(RMMTreeFixtureTest, number_leaf){
    int index[] = {15,16,17,18,9,10,11,12,13,14};
    int expected[] = {1,2,3,4,5,6,7,8,9,10};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->numLeaf(index[i]),expected[i]) << "Ocorreu um erro ao tentar descobrir a qual folha i=" << index[i] << " pertence";
    }

}
 
TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    int index[] = {0,1,3,4,7,8,9,10,11,15,13,18,19,20,22,23,24,26,27,30,32};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findclose(index[i]),bps->find_close(index[i])) << "Ocorreu um erro ao calcular o find_close de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_general){
    int index[] = {9,10,17,22,12,21,0,15,19,23,25,0,0,0,36,18};
    int d[] = {-2,1,-1,-1,-1,-1,2,-3,1,2,0,0,3,5,-1,-1};
    int expected[] = {21,11,18,35,17,36,2,18,24,27,29,6,9,11,39,21};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),expected[i]) << "Ocorreu um erro ao calcular um FwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_answer_not_found){
    int index[] = {9,17,22};
    int d[] = {10,-8,57};
    int size = t->bv.size();
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),size) << "Ocorreu um erro ao tentar obter 'answer not found' para FwdSearch para i=" << index[i];
    } 
} 

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){ 
   int index[] = {3,5,6,12,14,16,17,18,20,21,25,28,29,31,33,34,35,36,38,39};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findopen(index[i]),bps->find_open(index[i])) << "Ocorreu um erro ao calcular o find_open de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_enclose){
    int index[] = {2,5,9,10,12,15,17,18,19,21,22,23,25,30,31,32,36};
  
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->enclose(index[i]),bps->enclose(index[i])) << "Ocorreu um erro ao calcular o enclose de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_general){ 
    int index[] = {9,17,22,12,15,19,36,37,39,18};
    int d[] = {-2,-1,-1,-4,0,2,0,3,2,-1};
    int expected[] = {7,8,21,6,13,15,6,32,37,7};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),expected[i]) << "Ocorreu um erro ao calcular um BwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_answer_not_found){ 
    int index[] = {9,17,22};
    int d[] = {5,10,-4};
    int size = t->bv.size();
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),size) << "Ocorreu um erro ao tentar obter 'answer not found' para BwdSearch  com  i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, minExcess_i_and_j_in_the_same_block){ 
    int i[] = {0,0,8,13,16,20,22,24,25,29,33,36}; 
    int j[] = {3,2,11,15,19,22,23,27,26,30,34,39};
    int expected[] = {1,1,1,0,-3,-2,1,0,-1,-1,-2,-2};

    for(int k=0;k<1;k++){
        EXPECT_EQ(t->minExcess(i[k],j[k]),expected[k]) << "Ocorreu um erro ao contabilizar o excesso mínimo no intervalo [" << i[k] << "," << j[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, minExcess_i_and_j_in_the_different_block){ 
    int i[] = {9,1,12,15,17,33,38,39,0}; 
    int j[] = {30,6,26,29,32,39,39,39,39};
    int expected[] = {-1,0,-4,-3,-3,-5,-2,-1,1};

    for(int k=0;k<(int)(sizeof(i)/sizeof(i[0]));k++){
        EXPECT_EQ(t->minExcess(i[k],j[k]),expected[k]) << "Ocorreu um erro ao contabilizar o excesso mínimo no intervalo [" << i[k] << "," << j[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, expected_response_to_rmq){ 
    int i[] = {9,1,12,15,17,33,38,39,0}; 
    int j[] = {30,6,26,29,32,39,39,39,39};
    
    for(int k=0;k<1;k++){
        EXPECT_EQ(t->rmq(i[k],j[k]),bps->rmq(i[k],j[k])) << "Ocorreu um erro ao encontrar a posição do excecsso mínimo em [" << i[k] << "," << j[k] << "]";
    }
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    
    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.fwdSearch_*";
    return RUN_ALL_TESTS();
}
