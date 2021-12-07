#include "gtest/gtest.h"
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmMTreeClassic.h"

/*
* Os testes desabilitados (com excessão de print_tree), estão assim porque a resposta esperada (vetor expected[]) foi construída com base na
* sequência de parenteses balanceadas dada como exemplo no livro "Compacta Data Structures" de Gonzalo Navarro.
* int_vector<1> v={1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0};
*/


using namespace sdsl;
using namespace std;


class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree_Bin *t;
        int sizeBlock=32;
        int w=16;
        bp_support_sada<> *bps;
        //int_vector<1> v = {1,1,1,1,0,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,1,0,1,0,0,0};
        int_vector<1> v = {1,1,1,1,1,1,0,1,1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,1,0,0,0,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,0,1,1,0,1,0,1,0,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,1,0,0,0};
        vector<int> argsFindClose;

        RMMTreeFixtureTest(){}
        void SetUp(){
            t = new RMMTree_Bin(v,sizeBlock,w);
            bps = new bp_support_sada<>(&(t->bv));
            t->buildingTree();
		    srand(t->size/2);
            ArgumentsFindClose();
        }
        void TearDown(){
            delete t;
            delete bps;
        }

        void ArgumentsFindClose(){
        	int k,i=0;
            while(i<(t->size)/2){
                k = rand()%(t->size);
                if(t->bv[k]==1){
                    argsFindClose.push_back(k);
                    i++;
                }
            }
        }


};
 
TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    for(int i=0;i<argsFindClose.size();i++){
        EXPECT_EQ(t->findClose(argsFindClose[i]),bps->find_close(argsFindClose[i])) << "Resposta errada ao calcular o find_close de i=" << argsFindClose[i];
    } 
}


TEST_F(RMMTreeFixtureTest, print_tree){
    t->printInfoTree();
    t->printTree();
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    
//    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
    return RUN_ALL_TESTS();
}
