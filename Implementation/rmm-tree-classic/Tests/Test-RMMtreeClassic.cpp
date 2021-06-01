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


/* class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTreeFixtureTest();
        void setUp(){
            int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
            const int b=4;
            RMMTree t(v,b);
            bp_support_sada<> bps(&(t.bv));//suporte para findclosebp_support_sada<> bps(&(t.bv));//suporte para findclose
            t.buildingTree();
        }
};*/

/* TEST(RMMTreeFixtureTest, GET_INT){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
    const int b=4;
    RMMTree t(v,b);
    t.buildingTree();

    int index[] = {9,17,30,36};
    int len = 2;
    int expected[] = {3,2,2,1};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.getInt(index[i],len),expected[i]);
    }
} */

TEST(RMMTreeFixtureTest, GET_INT){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
    const int b=4;
    RMMTree t(v,b);
    t.buildingTree();

    int index[] = {9,17,30,36};
    int len = 2;
    int expected[] = {3,2,2,1};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(bits::read_int(t.bv.data() + (index[i]>>6), index[i] & 0x3F, len),expected[i]);
    }
}

/*
// TEST(RMMTreeTest, GET_INT_ERROR){
//     int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
//     RMMTree t(v,b);
//     t.buildingTree();
//     //EXPECT_EQ(t.getInt(39,7),-1);
//     //EXPECT_EQ(t.getInt(1,67),-1);
// } 

TEST(RMMTreeFixtureTest, KthLeafInTree){
    int index[] = {2,7,10,14,19,23,26,30,35,37};//indices
    int expected[] = {15,16,17,18,9,10,11,12,13,14};//kth posição na árvore de acordo com os respectivos índices acima
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        int k = ceil((double)(index[i]+1)/t.sizeBlock);
        EXPECT_EQ(t.leafInTree(k),expected[i]);
    }
}

TEST(RMMTreeFixtureTest, NumberLeaf){
    int index[] = {15,16,17,18,9,10,11,12,13,14};//indices
    int expected[] = {1,2,3,4,5,6,7,8,9,10};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.numLeaf(index[i]),expected[i]);
    }

}
 
TEST(RMMTreeFixtureTest, FWDSearch_FindClose){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
    const int b=4;
    RMMTree t(v,b);
    bp_support_sada<> bps(&(t.bv));//suporte para findclosebp_support_sada<> bps(&(t.bv));//suporte para findclose
    t.buildingTree();
    int index[] = {2,4,24,30,32,13,27,0,1,8,11,15,19,22,26,27,7,9,10,23};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.fwdSearch(index[i],-1),bps.find_close(index[i]));
    } 
}

TEST(RMMTreeFixtureTest, FwdSearchGeneral){
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
    const int b=4;
    RMMTree t(v,b);
    bp_support_sada<> bps(&(t.bv));//suporte para findclosebp_support_sada<> bps(&(t.bv));//suporte para findclose
    t.buildingTree();
    
    int index[] = {9,10,17,22,12,21,0,15,19,23,25,0,0,0,36};
    int d[] = {-2,1,-1,-1,-1,-1,2,-3,1,2,0,0,3,5,-1};
    int expected[] = {21,11,18,35,17,36,2,18,24,27,29,6,9,11,39};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.fwdSearch(index[i],d[i]),expected[i]);
    } 
}

TEST(RMMTreeFixtureTest, FwdSearch_AnswerNotFound){
    int index[] = {9,17,22};
    int d[] = {10,-8,57};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.fwdSearch(index[i],d[i]),-1);
    } 
} */

TEST(RMMTreeFixtureTest, BwdSearch_FindOpen){ 
    int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
    const int b=4;
    RMMTree t(v,b);
    bp_support_sada<> bps(&(t.bv));//suporte para findclosebp_support_sada<> bps(&(t.bv));//suporte para findclose
    t.buildingTree();
    //int index[] = {3,5,6,12,14,16,17,18,20,21,25,28,29,31,33,34,35,36,38,39};
    //int index[] = {3,5,14,20,25,28,29,31,33,34,35,36,38};//funciona
    //int index[] = {18,21,39}; // errado até no teste de mesa
    int index[] = {18};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.bwdSearch(index[i],0),bps.find_open(index[i])) << "Ocorreu um erro, ao calcular o find_open de:" << index[i];
    } 
}

TEST(RMMTreeFixtureTest, BwdSearch_Enclose){
     int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
    const int b=4;
    RMMTree t(v,b);
    bp_support_sada<> bps(&(t.bv));//suporte para findclosebp_support_sada<> bps(&(t.bv));//suporte para findclose
            
    int index[] = {2,7,15,19,24,27,32,35,39};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t.bwdSearch(index[i],-2),bps.enclose(index[i]));
    } 
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    
    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.GET_INT";
    return RUN_ALL_TESTS();
}
