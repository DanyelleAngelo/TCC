#include "gtest/gtest.h" 
#include <stdio.h>
#include <stdlib.h>
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
	int  size;
        
        void SetUp(){ 
	    t = new RMMTree(v,sizeBlock,w,order); 
	    bps = new bp_support_sada<>(&(t->bv)); 
            size = (int)v.size(); t->buildingTree();
        }
        void TearDown(){
	    delete t;
	    delete bps;
        }
};

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
	int index[] = {0,1,3,4,7,8,9,10,11,15,13,18,19,20,22,23,24,26,27,30,32};
	for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
            EXPECT_EQ(t->findClose(index[i]),bps->find_close(index[i])) << "Resposta errada ao calcular o find_close de i=" << index[i];
    	} 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){
	int index[] =  {3,5,6,12,14,16,17,18,20,21,25,28,29,31,33,34,35,36,38,39};
    	for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){ 
        	EXPECT_EQ(t->findOpen(index[i]),bps->find_open(index[i])) << "Resposta errada ao calcular o find_open de i=" << index[i];
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
