#include "gtest/gtest.h" 
#include <iostream>
#include <vector>
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
		vector<int> argsFindClose;
        vector<int> argsFindOpen;

		void SetUp(){ 
			t = new RMMTree(v,sizeBlock,w); 
			bps = new bp_support_sada<>(&(t->bv)); 
			srand(size);
            ArgumentsFindClose();
            ArgumentsFindOpen();
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

        void ArgumentsFindOpen(){
        	int k,i=0;
            while(i<(t->size)/2){
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
