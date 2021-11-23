#include "gtest/gtest.h"
#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../../read_bp/read_bp.h"
#include "../../rmm-tree-classic/rmMTreeClassic.h"
#include "../v2.h"
#include <sdsl/bits.hpp>

/*
* Os testes desabilitados (com excessão de print_tree), estão assim porque a resposta esperada (vetor expected[]) foi construída com base na
* sequência de parenteses balanceadas especifica.
*/


using namespace sdsl;
using namespace std;


class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree_Kary_V2 *t_v2;
        RMMTree_Bin *tBin;
        size_t sizeBlock=32;
        size_t w=16;
        int order=4;
        bp_support_sada<> *bps;
        int_vector<1> v;
        vector<int> argsI;
        vector<int> argsII;
        vector<int> argsSelect;
        vector<int> argsFindClose;
        vector<int> argsFindOpen;
        vector<int> argsDepth;
        vector<int> argsDepthBwd;
        vector<int> argsRmqI;
        vector<int> argsRmqJ;
        
        void SetUp(){
            parentheses_to_bits("../../dataset/data.txt", v);
            t_v2 = new RMMTree_Kary_V2(v,sizeBlock,w,order);
            tBin = new RMMTree_Bin(v,sizeBlock,w);
            bps = new bp_support_sada<>(&(t_v2->bv));
            t_v2->buildingTree();
            tBin->buildingTree();
            GenerateARguments();
        }
        void TearDown(){
            //delete t_v0;
            delete t_v2;
            delete tBin;
            delete bps;
        }

        void GenerateARguments(){
            ArgumentsI();
            ArgumentsII();
		    srand(t_v2->size/18);
            ArgumentsSelect();
            ArgumentsFindClose();
            ArgumentsFindOpen();
            ArgumentsRmq();
        }

        void ArgumentsI(){
        	int k,i=0;
		    srand(t_v2->size/38);
            while(i<(t_v2->size)/2){
                k = rand()%(t_v2->size-2);
                argsI.push_back(k);
                i++;
            }
        }
        
        void ArgumentsII(){
        	int k,i=0;
		    srand(t_v2->size/200);
            while(i<(t_v2->size)/2){
                k = rand()%(t_v2->size-2);
                argsII.push_back(k);
                i++;
            }
        }

        void ArgumentsSelect(){
            int k,i=0;
            while(i<(t_v2->size)/2){
                k = rand()%((v.size()/2)-1);
                argsSelect.push_back(k);
                i++;
            }
        }

        void ArgumentsFindClose(){
        	int k,i=0;
            while(i<(t_v2->size)/2){
                k = rand()%(t_v2->size-1);
                if(t_v2->bv[k]==1){
                    argsFindClose.push_back(k);
                    i++;
                }
            }
        }

        void ArgumentsFindOpen(){
        	int k,i=0;
            int depth, close, excessMax;
            while(i<(t_v2->size)/2){
                k = rand()%(t_v2->size-1);
                if(t_v2->bv[k]==0){
                    argsFindOpen.push_back(k);

                    //para as funções que precisam do argumento de profundidade
                    depth = tBin->depth(argsFindOpen[i]);
                    close = tBin->findClose(argsFindOpen[i]);
                    if(argsFindOpen[i]+1 <= close-1 ){
                        excessMax = tBin->maxExcess(argsFindOpen[i]+1, close-1);
                    }
                    else{
                        excessMax = depth;
                    }
                    argsDepthBwd.push_back((rand()% depth+depth) - depth);
                    argsDepth.push_back( (rand()% excessMax+depth) - depth);

                    i++;
                }
            }
        }

        void ArgumentsRmq(){
        	int k_i,k_j,i=0;
            while(i<(t_v2->size)/2){
                k_i = rand()%(t_v2->size);
                k_j = rand()%(t_v2->size);
                if(k_i < k_j){
                    argsRmqI.push_back(k_i);
                    argsRmqJ.push_back(k_j);
                    i++;
                }
            }
        }
};

TEST_F(RMMTreeFixtureTest, fwdSearch_general){
    for(int i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t_v2->fwdSearch(argsFindOpen[i],argsDepth[i]),tBin->fwdSearch(argsFindOpen[i],argsDepth[i])) << "Resposta errada ao calcular um FwdSearch genérico, para i=" << argsFindOpen[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    for(int i=0;i<argsFindClose.size();i++){
        EXPECT_EQ(t_v2->findClose(argsFindClose[i]),tBin->findClose(argsFindClose[i])) << "Resposta errada ao calcular o find_close de i=" << argsFindClose[i];
    }
}

TEST_F(RMMTreeFixtureTest, bwdSearch_general){ 
    for(int i=0;i<(int)argsFindOpen.size();i++){
       EXPECT_EQ(t_v2->bwdSearch(argsFindOpen[i],argsDepthBwd[i]),tBin->bwdSearch(argsFindOpen[i],argsDepthBwd[i])) << "Resposta errada ao calcular um BwdSearch genérico, para i=" << argsFindOpen[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){
    for(int i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t_v2->findOpen(argsFindOpen[i]),tBin->findOpen(argsFindOpen[i])) << "Resposta errada ao calcular o find_open de i=" << argsFindOpen[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_enclose){
    int k;
    for(int i=0;i<(t_v2->size)/2;i++){
        k = rand()%(t_v2->size);
        EXPECT_EQ(t_v2->enclose(k),tBin->enclose(k)) << "Resposta errada ao calcular o enclose de i=" << k;
    } 
}


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);

    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
    return RUN_ALL_TESTS();
}
