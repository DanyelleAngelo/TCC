#include "gtest/gtest.h"

#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmMTreeOptimized.h"
#include <sdsl/bits.hpp>


using namespace sdsl;
using namespace std;


class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree *t;
        size_t sizeBlock=8;
        size_t w=4;
        int order=16;
        bp_support_sada<> *bps;
        int size;
        int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0};
        //int_vector<1> v = {1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};

        void SetUp(){
            t = new RMMTree(v,sizeBlock,w,order);
            bps = new bp_support_sada<>(&(t->bv));
            size = (int)v.size();
            t->buildingTree();
        }
        void TearDown(){
            delete t;
        }
};


TEST_F(RMMTreeFixtureTest, expected_logM_ceil){
    int index[] = {1,2,3,15,16,17,29,30,31,45,46,47,120,250,597,2550};
    int b[] = {2,4,6,8,10,4,6,8,10,12,14,16,18,20,22,32};
    int expected[] = {0,1,1,2,2,3,2,2,2,2,2,2,2,2,3,3};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->cLog_m(index[i],b[i]),expected[i])<< "Resposta errada ao tentar calcular o teto do logarítmo na base " << b[i] << " de " << index[i];
    }
}

TEST_F(RMMTreeFixtureTest, expected_logM_floor){
    int index[] = {1,2,3,15,16,17,29,30,31,45,46,47,120,250,597,2550};
    int b[] = {2,4,6,8,10,4,6,8,10,12,14,16,18,20,22,32};
    int expected[] = {0,0,0,1,1,2,1,1,1,1,1,1,1,1,2,2};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fLog_m(index[i],b[i]),expected[i])<< "Resposta errada ao tentar calcular o teto do logarítmo na base " << b[i] << " de " << index[i];
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_kth_leaf_index_on_tree){
    int index[] = {0,1,2,3,4};//para b=4,m=4
    int expected[] = {5,6,2,3,4};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->leafInTree(index[i]),expected[i]) << "Resposta errada ao tentar obter o índice da RMMTree em que se encontra a " << index[i] <<"-th folha";
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_index_in_bp_leaf_index_on_tree){
    int index[] = {0,12,19,24,33,47,55,60,64,71};//para b=4,m=4
    int expected[] = {5,5,6,6,2,2,3,3,4,4};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        int k = floor(index[i]/(sizeBlock*order));
        EXPECT_EQ(t->leafInTree(k),expected[i]) << "Resposta errada ao tentar obter o índice do nó RMMTree em que se encontra " << "o indice " << index[i] << " de bp e a "  << k <<"-th folha";
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_given_a_node_v_returns_the_leaf_order){
    int index[] = {2,3,4,5,6};//para b=4
    int expected[] = {2,3,4,0,1};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->numLeaf(index[i]),expected[i]) << "Resposta errada ao tentar descobrir a ordem do nó v=" << index[i];
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_get_key_where_i_is_located){
    int index[] = {0,3,4,6,8,11,14,16,23,27,28,31,32,34,35,36,39};
    int kth_leaf[] = {0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2};
    int expected[] = {0,0,1,1,2,2,3,0,1,2,3,3,0,0,0,1,1};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->numKey(kth_leaf[i],index[i]),expected[i]) << "Resposta errada ao tentar descobrir a que chave " << index[i] << " pertence";
    }
}

TEST_F(RMMTreeFixtureTest, fwdsearch_i_answer_in_the_same_block){ 
    int index[] = {0,0,0,0,5,5,10,12,12,16,16,16,21,28,32};
    int d[] = {1,4,0,5,-1,4,0,1,0,-2,-3,1,3,0,-4};
    int expected[] = {1,10,6,11,6,11,12,13,14,18,21,27,24,30,36};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao buscar a resposta para fwdsearch de  i=" << index[i] << " e d =" << d[i]<< " com a resposta acontecendo na mesma folha de i"; 
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_general){
    int index[] = {14,11,29};
    int d[] = {-2,-4,-1};
    int expected[] = {18,21,34};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao calcular um FwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    int index[] = {0,1,3,4,7,8,9,10,11,15,13,18,19,20,22,23,24,26,27,30,32,33,35,39,40,43,47,49,52,53,55,57,62,63,64,66,70,72,76,77};
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findClose(index[i]),bps->find_close(index[i])) << "Resposta errada ao calcular o find_close de i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdsearch_i_answer_in_the_same_block){ 
    int index[] = {3,7,13,20,21,26,31};
    int d[] = {-2,0,-5,0,4,-3,-1};
    int expected[] = {-1,5,6,18,15,21,22};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao buscar a resposta para bwdsearch de  i=" << index[i] << " e d =" << d[i]<< " com a resposta acontecendo na mesma folha de i"; 
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_general){ 
    int index[] = {9,17,22,12,15,19,36,37,18};
    int d[] = {-2,-1,-1,-4,0,2,0,3,-1};
    int expected[] = {7,8,21,6,13,15,6,32,7};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao calcular um BwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){ 
   int index[] = {3,5,6,12,14,16,17,18,20,21,25,28,29,31,33,34,35,36,38,39,44,47,53,54,55,62,70,74,75,76,77,79};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->findOpen(index[i]),bps->find_open(index[i])) << "Resposta errada ao calcular o find_open de i=" << index[i];
    } 
}


TEST_F(RMMTreeFixtureTest, DISABLED_print_tree){
    t->printInfoTree();
    t->printTree();
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);

    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
    return RUN_ALL_TESTS();
}
