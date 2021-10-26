#include "gtest/gtest.h"
#include <vector>
#include <sdsl/int_vector.hpp>
#include <sdsl/bp_support_sada.hpp>
#include "../rmMTreeOptimized.h"
#include <sdsl/bits.hpp>

/*
* Os testes desabilitados (com excessão de print_tree), estão assim porque a resposta esperada (vetor expected[]) foi construída com base na
* sequência de parenteses balanceadas especifica.
*/


using namespace sdsl;
using namespace std;


class RMMTreeFixtureTest : public ::testing::Test{
    public:
        RMMTree_Kary *t;
        size_t sizeBlock=32;
        size_t w=16;
        int order=4;
        bp_support_sada<> *bps;
        int_vector<1> v = {1,1,1,1,1,1,0,1,1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,1,0,0,0,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,0,1,1,0,1,0,1,0,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,1,1,0,1,1,1,0,1,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,1,1,1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,1,0,0,0};
        //int_vector<1> v = {1,1,1,1,0,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,0,1,0,0,1,1,0,1,0,1,0,1,0,1,0,0,0};
        vector<int> argsFindClose;
        vector<int> argsFindOpen;
        vector<int> argsRmqI;
        vector<int> argsRmqJ;
        
        void SetUp(){
            t = new RMMTree_Kary(v,sizeBlock,w,order);
            bps = new bp_support_sada<>(&(t->bv));
            t->buildingTree();
		    srand(t->size/2);
            ArgumentsFindClose();
            ArgumentsFindOpen();
            ArgumentsRmq();
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

        void ArgumentsRmq(){
        	int k_i,k_j,i=0;
            while(i<(t->size)/2){
                k_i = rand()%(t->size);
                k_j = rand()%(t->size);
                if(k_i < k_j){
                    argsRmqI.push_back(k_i);
                    argsRmqJ.push_back(k_j);
                    i++;
                }
            }
        }
};

TEST_F(RMMTreeFixtureTest, DISABLED_GET_INT){
    int index[] = {0,1};
    int expected[]={64905,64274};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bitsread(index[i]),expected[i])<< "Resposta errada ao tentar devolver o inteiro correspondente aos bits [" << index[i] << ","<<(index[i]+w-1)<<"]";
    }
}


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

TEST_F(RMMTreeFixtureTest, DISABLED_fwdsearch_i_answer_in_the_same_block){ 
    int index[] = {0,0,0,0,5,5,10,12,12,16,16,16,21,28,32};
    int d[] = {1,4,0,5,-1,4,0,1,0,-2,-3,1,3,0,-4};
    int expected[] = {1,10,6,11,6,11,12,13,14,18,21,27,24,30,36};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao buscar a resposta para fwdsearch de  i=" << index[i] << " e d =" << d[i]<< " com a resposta acontecendo na mesma folha de i"; 
    } 
}

TEST_F(RMMTreeFixtureTest, DISABLED_fwdSearch_general){
    int index[] = {14,11,29};
    int d[] = {-2,-4,-1};
    int expected[] = {18,21,34};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->fwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao calcular um FwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, fwdSearch_findClose){
    for(int i=0;i<argsFindClose.size();i++){
        EXPECT_EQ(t->findClose(argsFindClose[i]),bps->find_close(argsFindClose[i])) << "Resposta errada ao calcular o find_close de i=" << argsFindClose[i];
    } 
}

TEST_F(RMMTreeFixtureTest, DISABLED_bwdSearch_i_answer_in_the_same_block){ 
    int index[] = {3,7,13,20,21,26,31};
    int d[] = {-2,0,-5,0,4,-3,-1};
    int expected[] = {-1,5,6,18,15,21,22};
    
    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao buscar a resposta para bwdsearch de  i=" << index[i] << " e d =" << d[i]<< " com a resposta acontecendo na mesma folha de i"; 
    } 
}

TEST_F(RMMTreeFixtureTest, DISABLED_bwdSearch_general){ 
    int index[] = {9,17,22,12,15,19,36,37,18};
    int d[] = {-2,-1,-1,-4,0,2,0,3,-1};
    int expected[] = {7,8,21,6,13,15,6,32,7};

    for(int i=0;i<(int)(sizeof(index)/sizeof(index[0]));i++){
        EXPECT_EQ(t->bwdSearch(index[i],d[i]),expected[i]) << "Resposta errada ao calcular um BwdSearch genérico, para i=" << index[i];
    } 
}

TEST_F(RMMTreeFixtureTest, bwdSearch_findOpen){ 
    for(int i=0;i<argsFindOpen.size();i++){
        EXPECT_EQ(t->findOpen(argsFindOpen[i]),bps->find_open(argsFindOpen[i])) << "Resposta errada ao calcular o find_open de i=" << argsFindOpen[i];
    } 
}

TEST_F(RMMTreeFixtureTest, DISABLED_expected_response_to_rmq){ 
    int m;
    int p;
    
    for(int k=4;k<5;k++){
        m = t->minExcess(argsRmqI[k],argsRmqJ[k]);
        p = t->rmq(argsRmqI[k],argsRmqJ[k]);
        EXPECT_EQ(m,bps->excess(p) - bps->excess(argsRmqI[k]-1)) << "Resposta errada ao encontrar a posição do excecsso mínimo em [" << argsRmqI[k]<< "," << argsRmqJ[k] << "]";
    }
}

TEST_F(RMMTreeFixtureTest, bwdSearch_enclose){
    int k=0;
    for(int i=0;i<(t->size)/2;i++){
        k = rand()%(t->size);
        EXPECT_EQ(t->enclose(k),bps->enclose(k)) << "Resposta errada ao calcular o enclose de i=" << k;
    } 
}

TEST_F(RMMTreeFixtureTest, check_if_i_is_a_leaf){
    int k=0;
    for(int i=0;i<(t->size)/2;i++){
        k = rand()%(t->size);
        EXPECT_EQ(t->isLeaf(k),v[k] && !v[k+1]) << "Resposta errada ao verificar se o nó codificado em [" << k << " , close(i)] é uma folha."; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_check_if_x_is_ancestor_of_y){ 
    int x[] = {0,4,25}; 
    int y[] = {1,6,6}; 
    bool expected[] ={true,true,false}; 
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->isAncestor(x[k],y[k]),expected[k]) << "Resposta errada ao verificar se o nó x= " << x[k] << " é ancestral do nó y= " << y[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_node_depth_x){ 
    int index[] = {2,7,11,17,23,27,34}; 
    int expected[] ={2,1,5,5,3,5,4}; 

    for(int k=0;k<(int)(sizeof(index)/sizeof(index[0]));k++){
        EXPECT_EQ(t->depth(index[k]),expected[k]) << "Resposta errada ao verificar a profunidade do nó codificado em [" << index[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, returns_the_index_j_that_encodes_the_parent_of_node_x){ 
    int k=0;
    for(int i=0;i<(t->size)/2;i++){
        k = rand()%(t->size);
        EXPECT_EQ(t->parent(k),bps->enclose(k)) << "Resposta errada ao buscar o pai do nó codificado em bv[" << k << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_rigth_sibling_of_x){ 
    int x[] = {2,8,13,24,30};

    int expected[] = {4,22,15,26,32};
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->nextSibling(x[k]),expected[k]) << "Resposta errada ao buscar o irmão direito do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_left_sibling_of_x){ 
    int x[] = {13,19,30,37}; 
    
    int expected[] = {11,9,26,7};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->prevSibling(x[k]),expected[k]) << "Resposta errada ao buscar o irmão esquerdo do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_return_lastChild_of_x){ 
    int x[] = {1,7,10,8,23};
    
    int expected[] = {4,22,15,19,32};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->lastChild(x[k]),expected[k]) << "Resposta errada ao buscar o último filho do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_return_firstChild_of_x){ 
    int x[] = {0,22,26,9,23};

    int expected[] = {13,27,10,24};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->firstChild(x[k]),expected[k]) << "Resposta errada ao buscar o primeiro filho do nó codificado em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_subtree_size){ 
    int x[] = {1,2,7,8,9,22,27};
    
    int expected[] = {3,1,15,7,5,7,1};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->subtreeSize(x[k]),expected[k]) << "Resposta errada ao calcular o tamannho da subarvore enraizada em bv[" << x[k] << " , close(i)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_ancestor_of_x_that_is_d_levels_above_x){ 
    int x[] = {19,2,11,15,24,27,37};
    int d[] = {2,2,4,4,2,3,1};  
    
    int expected[] = {7,0,7,7,22,22,0};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->levelAncestor(x[k],d[k]),expected[k]) << "Resposta errada ao calcular o ancestral de bv[" << x[k] << " , close(i)] que está " << d[k] << " níveis acima."; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_expected_level_next){ 
    int x[] = {4,7,10,15};
    int expected[] = {8,37,24,27};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->levelNext(x[k]),expected[k]) << "Resposta errada ao calcular o próximo nó a direita de  " << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_expected_level_prev){ 
    int x[] = {22,8,7,30};
    int expected[] = {8,4,1,26};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->levelPrev(x[k]),expected[k]) << "Resposta errada ao calcular o próximo nó a direita de  " << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_leftmost_node_with_depth_d){ 
    int d[] = {1,3,4,5,6};
    int expected[] = {0,2,9,10,11};
    
    for(int k=0;k<(int)(sizeof(d)/sizeof(d[0]));k++){
        EXPECT_EQ(t->levelLeftMost(d[k]),expected[k]) << "Resposta errada ao calcular o nó mais a esquerda com profundidade  " << d[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_rightmost_node_with_depth_d){ 
    int d[] = {1};
    int expected[] = {0};
/*     int d[] = {1,3,4,5};//exclusivo para a sequência de exemplo de Navarro(20106)
    int expected[] = {0,37,22,23,32};
 */
    for(int k=0;k<(int)(sizeof(d)/sizeof(d[0]));k++){
        EXPECT_EQ(t->levelRightMost(d[k]),expected[k]) << "Resposta errada ao calcular o nó mais a direita com profundidade  " << d[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_number_of_leaves_to_the_left_of_node_x){ 
    int x[] = {0,8,22,24,27,32,37};
    int expected[] = {0,2,6,6,7,9,10};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->leafRank(x[k]),expected[k]) << "Resposta errada ao computar o número de folhas a esquerda do nó codificado em B[" << x[k] << " , close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_tth_leaf_index){ 
    int x[] = {1,2,3,4,5,6,7,8,9,10,11};
    int expected[] = {2,4,11,13,15,19,24,27,30,32,37};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->leafSelect(x[k]),expected[k]) << "Resposta errada ao buscar o índice da " << x[k] << "-th folha"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_leftmost_leaf_of_x){ 
    int x[] = {4,8,22,24,30,37,7,32,27};
    int expected[] = {4,11,24,24,30,37,11,32,27};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->leftMostLeaf(x[k]),expected[k]) << "Resposta errada ao buscar a folha mais a esquerda de B[" << x[k] << ", close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_rightmost_leaf_of_x){ 
    int x[] = {8,22,23,26};
    int expected[] = {19,32,32,27};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->rightMostLeaf(x[k]),expected[k]) << "Resposta errada ao buscar a folha mais a direita de B[" << x[k] << ", close(x)]"; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_pre_rank_x){ 
    int x[] = {11,15};
    int expected[] = {8,10};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->preRank(x[k]),expected[k]) << "Resposta errada ao realizar o percurso pré-ordem de X= " << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_post_rank_x){ 
    int x[] = {11,16};
    int expected[] = {3,5};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->postRank(x[k]),expected[k]) << "Resposta errada ao calcular postrank de x=" << x[k]; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_pre_select){ 
    int x[] = {3,9};
    int expected[] = {2,11};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->preSelect(x[k]),expected[k]) << "Resposta errada ao calcular a posição do " << x[k] << "-th parênteses de abertura."; 
    }
}

TEST_F(RMMTreeFixtureTest, DISABLED_post_select){ 
    int x[] = {8,12,18};
    int expected[] = {9,27,7};
    
    for(int k=0;k<(int)(sizeof(x)/sizeof(x[0]));k++){
        EXPECT_EQ(t->postSelect(x[k]),expected[k]) << "Resposta errada ao calcular o parênteses de abertura correspondente ao  " << x[k] << "-th parênteses de fechamento."; 
    }
}

TEST_F(RMMTreeFixtureTest, print_tree){
    t->printInfoTree();
    t->printTree();
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);

    testing::GTEST_FLAG(filter) = "RMMTreeFixtureTest.*";
    return RUN_ALL_TESTS();
}
