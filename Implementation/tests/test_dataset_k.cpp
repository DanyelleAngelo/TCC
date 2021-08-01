#include "gtest/gtest.h" 
#include <iostream>
#include <vector>
#include "read_bp/read_bp.h" 
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-optimized/rmMTreeOptimized.h"

using namespace sdsl; 
using namespace std; 

int_vector<1> v;
    
class RMMTreeFixtureTest : public ::testing::Test{ 
	public: 
		RMMTree *t; 
		int order=32;
		int sizeBlock=16; 
		int w=8; 
		bp_support_sada<> *bps;
		vector<int> argsFindClose;
                vector<int> argsFindOpen;
        
        void SetUp(){ 
	    t = new RMMTree(v,sizeBlock,w,order); 
	    t->buildingTree();
 	    bps = new bp_support_sada<>(&(t->bv)); 
	    srand(t->size);
            ArgumentsFindClose();
            ArgumentsFindOpen();
        }
        void TearDown(){
	     delete t;
	     delete bps;
        }

        void ArgumentsFindClose(){
            int k,i=0;
            while(i<200){
                k = rand()%(t->size);
                if(t->bv[k]==1){
                    argsFindClose.push_back(k);
                    i++;
                }
            }
        }

        void ArgumentsFindOpen(){
            int k,i=0;
            while(i<200){
                k = rand()%(t->size);
                if(t->bv[k]==0){
                    argsFindOpen.push_back(k);
                    i++;
                }
            }
        }
};

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    for(int i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t->findClose(argsFindClose[i]),bps->find_close(argsFindClose[i])) << "Resposta errada ao calcular o find_close de i=" << argsFindClose[i];
    }
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){ 
    for(int i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t->findOpen(argsFindOpen[i]),bps->find_open(argsFindOpen[i])) << "Resposta errada ao calcular o find_open de i=" << argsFindOpen[i];
    } 
}

int main(int argc, char *argv[]){
	 if(argc<2){
		cout << "Número de parâmetros  incorreto.\n"<<endl;
   	 }
    	parentheses_to_bits(argv[1], v);
    	::testing::InitGoogleTest(&argc, argv);
    	testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
	return RUN_ALL_TESTS();
}
