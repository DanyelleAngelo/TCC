#include "gtest/gtest.h" 
#include <stdio.h>
#include <stdlib.h>
#include "read_bp/read_bp.h" 
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-classic/rmMTreeClassic.h"

using namespace sdsl; 
using namespace std; 

int_vector<1> v;
    
class RMMTreeFixtureTest : public ::testing::Test{ 
	public: 
	RMMTree *t; 
	int sizeBlock=16; 
	int w=8; 
	bp_support_sada<> *bps; 
	int  size;
        
        void SetUp(){ 
	    t = new RMMTree(v,sizeBlock,w); 
	    bps = new bp_support_sada<>(&(t->bv)); 
            size = (int)v.size(); t->buildingTree();
	    srand(size);
        }
        void TearDown(){
	    delete t;
	    delete bps;
        }
};

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
	for(int i=0;i<20;i++){
	    int k = rand()%size;
	    if(v[k]==1)
            	EXPECT_EQ(t->findClose(k),bps->find_close(k)) << "Resposta errada ao calcular o find_close de i=" << k;
    	} 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){

	for(int i=0;i<20;i++){ 
		int k = rand()%size;
		if(v[k]==0)
        	    EXPECT_EQ(t->findOpen(k),bps->find_open(k)) << "Resposta errada ao calcular o find_open de i=" << k;
   	 } 
}

TEST_F(RMMTreeFixtureTest, DISABLED_expected_response_to_rmq){
	int i[] = {9,1,12,15,17,33,38,39,0,8}; 
    	int j[] = {30,6,26,29,32,39,39,39,39,15}; 

	for(int a=8;a<16;a++){
		cout << "indice= " << a << " excesso= " <<( bps->excess(a) - bps->excess(7)) << "bit=" << t->bv[a]  << "\n";
	}	
	for(int k=9;k<(int)(sizeof(i)/sizeof(i[0]));k++){
		cout <<"teste " << ( bps->excess(bps->rmq(i[k],j[k])) -  bps->excess(i[k]-1)) << "\n";
        	EXPECT_EQ(t->rmq(i[k],j[k]),bps->rmq(i[k],j[k])) << "Resposta errada ao encontrar a  posição do excecsso mínimo em [" << i[k] << "," << j[k] << "]";
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
