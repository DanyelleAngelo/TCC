#include "gtest/gtest.h" 
#include <iostream>
#include <vector>
#include "read_bp/read_bp.h" 
#include <sdsl/bp_support_sada.hpp>
#include "../rmm-tree-classic/rmMTreeClassic.h"

using namespace sdsl; 
using namespace std; 

int iterations;
int_vector<1> v;
int sizeBlock;
int w;
int order;
    
class RMMTreeFixtureTest : public ::testing::Test{ 
	public: 
		RMMTree *t; 
		bp_support_sada<> *bps; 
		vector<long long int> argsFindClose;
        vector<long long int> argsFindOpen;

		void SetUp(){
			t = new RMMTree(v,sizeBlock,w); 
			bps = new bp_support_sada<>(&(t->bv));
			t->buildingTree(); 
			srand(t->size/sizeBlock);
            ArgumentsFindClose();
            ArgumentsFindOpen();
		}
		
		void TearDown(){
			delete t;
			delete bps;
		}

        void ArgumentsFindClose(){
            long long int  k,i=0;
            while(i<iterations){
                k = rand()%(t->size);
                if(t->bv[k]==1){
                    argsFindClose.push_back(k);
                    i++;
                }
            }
        }

        void ArgumentsFindOpen(){
            int k,i=0;
            while(i<iterations){
                k = rand()%(t->size);
                if(t->bv[k]==0){
                    argsFindOpen.push_back(k);
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

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){ 

    for(int i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t->findOpen(argsFindOpen[i]),bps->find_open(argsFindOpen[i])) << "Resposta errada ao calcular o find_open de i=" << argsFindOpen[i];
    } 
}

int main(int argc, char *argv[]){
	parentheses_to_bits(argv[1],v);
	iterations = atoi(argv[2]);
	sizeBlock= atoi(argv[3]);
	order = atoi(argv[4]);	
	w = sizeBlock/2;

    ::testing::InitGoogleTest(&argc, argv);
    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
	return RUN_ALL_TESTS();
}
