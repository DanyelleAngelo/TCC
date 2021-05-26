#include "gtest/gtest.h"

#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support.hpp>
#include "../RMMTreeClassic.h"

/*
*DISABLED_ desabilitar um teste inserindo esse prefixo em seu nome
*testing::GTEST_FLAG(filter) = "-MyLibrary.TestWriting"; usar o nome do teste precedido de "-" desabilita ele no main,
a ausência desse mesmo elemento "-", torna o teste o único a ser executado.
*/

using namespace sdsl;
using namespace std;

const int b=4;

TEST(RMMTreeTest, GET_INT){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
    RMMTree t(v,b);
    t.buildingTree();

    int index[] = {9,17,30,36};
    int len = 1;
    int expected[] = {3,0,2,1};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.bitsread(index[i],index[i]+len),expected[i]);
    } 
}

TEST(LeafTest, KthLeafInTree){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

    RMMTree t(v,b);
    t.buildingTree();

    int index[] = {2,7,10,14,19,23,26,30,35,37};//indices
    int expected[] = {15,16,17,18,9,10,11,12,13,14};//kth posição na árvore de acordo com os respectivos índices acima
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        int k = ceil((double)(index[i]+1)/t.sizeBlock);
        EXPECT_EQ(t.leafInTree(k),expected[i]);
    }
}

TEST(LeafTest, NumberLeaf){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

    RMMTree t(v,b);
    t.buildingTree();

    int index[] = {15,16,17,18,9,10,11,12,13,14};//indices
    int expected[] = {1,2,3,4,5,6,7,8,9,10};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.numLeaf(index[i]),expected[i]);
    }

}
 
TEST(FWDSearchTest, FWDSearch_FindClose){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

    RMMTree t(v,b);
    t.buildingTree();
    bp_support_sada<> bps(&(t.bv));//suporte para findclose

    int index[] = {2,4,24,30,32,13,27,0,1,8,11,15,19,22,26,27,7,9,10,23};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.fwdSearch(index[i],-1),bps.find_close(index[i]));
    } 
}

TEST(FWDSearchTest, FwdSearchGeneral){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

    RMMTree t(v,b);
    t.buildingTree();

    int index[] = {9,17,22,12,21,0,15,19,23,25,0};
    int d[] = {-2,-1,-1,-1,-1,2,-3,1,2,0,0};
    int expected[] = {21,18,35,17,36,2,18,24,27,29,6};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.fwdSearch(index[i],d[i]),expected[i]);
    } 
}

TEST(FWDSearchTest, FwdSearch_AnswerNotFound){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

    RMMTree t(v,b);
    t.buildingTree();

    int index[] = {9,17,22};
    int d[] = {10,-8,57};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.fwdSearch(index[i],d[i]),-1);
    } 
}



int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(filter) = "FWDSearchTest.FWDSearch_FindClose";
    return RUN_ALL_TESTS();
}
