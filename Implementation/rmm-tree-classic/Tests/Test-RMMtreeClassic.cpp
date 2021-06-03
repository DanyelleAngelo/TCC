#include "gtest/gtest.h"

#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support.hpp>
#include "../RMMTreeClassic.h"
#include <sdsl/bits.hpp>
/*
*DISABLED_ desabilitar um teste inserindo esse prefixo em seu nome
*testing::GTEST_FLAG(filter) = "-MyLibrary.TestWriting"; usar o nome do teste precedido de "-" desabilita ele no main,
a ausência desse mesmo elemento "-", torna o teste o único a ser executado.
*/

using namespace sdsl;
using namespace std;


class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree *t;
        int b=4;
        bp_support_sada<> *bps;
        int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
        //RMMTreeFixtureTest_F();
        void SetUp(){
            t = new RMMTree(v,b);
            bps = new bp_support_sada<>(&(t->bv));//suporte para findclosebp_support_sada<> bps(&(t.bv));//suporte para findclose
            t->buildingTree();
        }
        void TearDown(){
            delete t;
        }
};

TEST_F(RMMTreeFixtureTest, GET_INT){
    int index[] = {9,17,30,36};
    int len = 2;
    int expected[] = {3,2,2,1};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(bits::read_int(t->bv.data() + (index[i]>>6), index[i] & 0x3F, len),expected[i])<< "Ocorreu um erro ao tentar devolver o interiro correspondente aos bits [" << index[i] << ","<<(index[i]+len-1)<<"]";
    }
}

TEST_F(RMMTreeFixtureTest, KthLeafInTree){
    int index[] = {2,7,10,14,19,23,26,30,35,37};//indices
    int expected[] = {15,16,17,18,9,10,11,12,13,14};//kth posição na árvore de acordo com os respectivos índices acima
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        int k = ceil((double)(index[i]+1)/t->sizeBlock);
        EXPECT_EQ(t->leafInTree(k),expected[i]) << "Ocorreu um erro ao tentar obterr o índice da RMMTree em que se encontra a " << index[i] <<"-th folha";
    }
}

TEST_F(RMMTreeFixtureTest, NumberLeaf){
    int index[] = {15,16,17,18,9,10,11,12,13,14};//indices
    int expected[] = {1,2,3,4,5,6,7,8,9,10};//número das folhas esperados para as áreas de cobertura acima

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->numLeaf(index[i]),expected[i]) << "Ocorreu um erro ao tentar descobrir a qual folha i=" << index[i] << " pertence";
    }

}
 
TEST_F(RMMTreeFixtureTest, FWDSearch_FindClose){
    int index[] = {2,4,24,30,32,13,27,0,1,8,11,15,19,22,26,27,7,9,10,23};
    //int index[] = {18};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],-1),bps->find_close(index[i])) << "Ocorreu um erro ao calcular o find_close de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, FwdSearchGeneral){
    int index[] = {9,10,17,22,12,21,0,15,19,23,25,0,0,0,36};
    int d[] = {-2,1,-1,-1,-1,-1,2,-3,1,2,0,0,3,5,-1};//profundidade calculada em relação à index
    int expected[] = {21,11,18,35,17,36,2,18,24,27,29,6,9,11,39};//posição em que ocorre a profundidade esperada
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),expected[i]) << "Ocorreu um erro ao calcular um FwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, FwdSearch_AnswerNotFound){
    int index[] = {9,17,22};
    int d[] = {10,-8,57};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),-1) << "Ocorreu um erro ao tentar obter 'answer not found' para FwdSearch para i=" << index[i];
    } 
} 

/* TEST_F(RMMTreeFixtureTest, BwdSearch_FindOpen){ 
    //int index[] = {3,5,6,12,14,16,17,18,20,21,25,28,29,31,33,34,35,36,38,39};
    //int index[] = {3,5,14,20,25,28,29,31,33,34,35,36,38};//funciona
    //int index[] = {18,21,39}; // errado até no teste de mesa
    int index[] = {18};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],0),bps->find_open(index[i])) << "Ocorreu um erro ao calcular o find_open de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, BwdSearch_Enclose){
    int index[] = {2,7,15,19,24,27,32,35,39};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],-2),bps->enclose(index[i])) << "Ocorreu um erro ao calcular o enclose de i=" << index[i];
    } 
} */

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    
    //testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.FWDSearch_FindClose";
    return RUN_ALL_TESTS();
}
